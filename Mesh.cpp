/**
 * Chris Brenton
 * Mesh object for raytracing.
 * Can be a subdivision surface.
 * 473
 * 4/7/11
 */

#include <cstdlib>
#include <string>
//#include <iostream>
#include <sstream>
#include "Mesh.h"

#define PT_ARGS 3
#define FACE_ARGS 3

Mesh::Mesh(istream& input) : Geometry()
{
   int braceCount = 0;
   string line;
   //char end[256];
   char tmp;
   getline(input, line);
   if (line[0] == '{')
   {
      braceCount++;
      sscanf(line.c_str(), "{ %d", &subDepth);
      cout << "sub_depth: " << subDepth << endl;
      getline(input, line);
   }
   istringstream lineSS;
   if (line.find("vertex_vectors {") != string::npos)
   {
      getline(input, line);
      // Find the index of the first comment.
      size_t comment = line.find("//");
      // Remove data after comments, if any.
      if (comment != string::npos)
      {
         line.erase(comment);
      }
      // Assign the line to lineSS.
      lineSS.clear();
      lineSS.str(line);
      // Skip whitespace.
      lineSS >> ws;
      // Get number of vertices.
      lineSS >> nVertices;
      cout << "#v: " << nVertices << endl;
      int vCount = 0;
      // While line contains vertices.
      while (line.find("}") == string::npos)
      {
         if (vCount > nVertices)
         {
            cerr << "Too many vertices." << endl;
            exit(EXIT_FAILURE);
         }
         // Get the next line.
         getline(input, line);
         // Find the index of the first comment.
         size_t comment = line.find("//");
         // Remove data after comments, if any.
         if (comment != string::npos)
         {
            line.erase(comment);
         }
         // Assign the line to lineSS.
         lineSS.clear();
         lineSS.str(line);
         // While lineSS has data.
         while (!lineSS.eof())
         {
            vec3_t *pt = new vec3_t();
            if (lineSS >> tmp && tmp == '<' &&
                  lineSS >> pt->v[0] &&
                  lineSS >> tmp && tmp == ',' &&
                  lineSS >> pt->v[1] &&
                  lineSS >> tmp && tmp == ',' &&
                  lineSS >> pt->v[2] &&
                  lineSS >> tmp && tmp == '>')
            {
               vCount++;
               points.push_back(pt);
               if (!vCount == nVertices)
               {
                  lineSS >> tmp;
               }
            }
            else
            {
               delete pt;
            }
         }
      }
   }
   // Skip the line with the closing curly brace.
   getline(input, line);

   // Get faces.
   if (line.find("face_indices {") != string::npos)
   {
      getline(input, line);
      // Find the index of the first comment.
      size_t comment = line.find("//");
      // Remove data after comments, if any.
      if (comment != string::npos)
      {
         line.erase(comment);
      }
      // Assign the line to lineSS.
      lineSS.clear();
      lineSS.str(line);
      // Skip whitespace.
      lineSS >> ws;
      // Get number of vertices.
      lineSS >> nFaces;
      cout << "#f: " << nFaces << endl;
      int fCount = 0;
      // While line contains vertices.
      while (line.find("}") == string::npos)
      {
         if (fCount > nFaces)
         {
            cerr << "Too many faces." << endl;
            exit(EXIT_FAILURE);
         }
         // Get the next line.
         getline(input, line);
         // Find the index of the first comment.
         size_t comment = line.find("//");
         // Remove data after comments, if any.
         if (comment != string::npos)
         {
            line.erase(comment);
         }
         // Assign the line to lineSS.
         lineSS.clear();
         lineSS.str(line);
         // While lineSS has data.
         while (!lineSS.eof())
         {
            int p1, p2, p3;
            p1 = p2 = p3 = -1;
            if (lineSS >> tmp && tmp == '<' &&
                  lineSS >> p1 &&
                  lineSS >> tmp && tmp == ',' &&
                  lineSS >> p2 &&
                  lineSS >> tmp && tmp == ',' &&
                  lineSS >> p3 &&
                  lineSS >> tmp && tmp == '>')
            {
               fCount++;
               // Add new triangle.
               // Represent neighboring faces somehow.
               if (p1 >= (int)points.size() || p1 < 0)
               {
                  cerr << "Error: vertex index out of range (" << p1 << ")." << endl;
                  exit(EXIT_FAILURE);
               }
               if (p2 >= (int)points.size() || p2 < 0)
               {
                  cerr << "Error: vertex index out of range (" << p2 << ")." << endl;
                  exit(EXIT_FAILURE);
               }
               if (p3 >= (int)points.size() || p3 < 0)
               {
                  cerr << "Error: vertex index out of range (" << p3 << ")." << endl;
                  exit(EXIT_FAILURE);
               }
               else
               {
                  vec3_t pt1 = *points[p1];
                  vec3_t pt2 = *points[p2];
                  vec3_t pt3 = *points[p3];
                  faces.push_back(new Triangle(pt1, pt2, pt3));
               }
               if (!fCount == nFaces)
               {
                  lineSS >> tmp;
               }
            }
         }
      }
   }
   readOptions(input);
   // Set all mesh face materials to the mesh's material.
   setMats(mat);
   //randMats();
   boundingBox = bBox();
   for (int subLevel = 0; subLevel < subDepth; subLevel++)
   {
      subdivide();
   }
}

Mesh::~Mesh()
{
   //cout << "deleting mesh" << endl;
   for (int i = 0; i < (int)faces.size(); i++)
   {
      delete faces[i];
   }
   for (int j = 0; j < (int)points.size(); j++)
   {
      delete points[j];
   }
}

Box* Mesh::bBox()
{
   float extrema[3][2];
   if (faces.size() == 0)
   {
      return new Box();
   }
   for (int dim = 0; dim < 3; dim++)
   {
      Box *fBox = faces[0]->boundingBox;
      extrema[dim][0] = fBox->getMin().v[dim];
      extrema[dim][1] = fBox->getMax().v[dim];
   }
   for (int i = 0; i < (int)faces.size(); i++)
   {
      vec3_t c1, c2;
      Box *faceBox = faces[i]->boundingBox;
      c1 = faceBox->getMin();
      c2 = faceBox->getMax();
      for (int j = 0; j < 3; j++)
      {
         extrema[j][0] = min(extrema[j][0], c1.v[j]);
         extrema[j][1] = max(extrema[j][1], c2.v[j]);
      }
   }
   vec3_t b_c1, b_c2;
   b_c1 = vec3_t(extrema[0][0], extrema[1][0], extrema[2][0]);
   b_c2 = vec3_t(extrema[0][1], extrema[1][1], extrema[2][1]);
   return new Box(b_c1, b_c2);
}

bool Mesh::hit(Ray ray, float *t, HitData *data, float minT, float maxT)
{
   bool hitFound = false;
   float geomT = -1.0;
   float curDepth = -1.0;

   for (int i = 0; i < (int)faces.size(); i++)
   {
      Triangle *curObject = faces[i];
      geomT = -1.0;
      HitData tmpData;
      bool intersected = curObject->hit(ray, &geomT, &tmpData);
      if (intersected)
      {
         if (geomT >= 0 && (!hitFound || (hitFound && geomT < curDepth)))
         {
            curDepth = geomT;
            *data = tmpData;
         }
      }
      hitFound |= (intersected && curDepth > 0.0);
   }
   if (hitFound && (curDepth > 0.0))
   {
      *t = curDepth;
   }
   return hitFound;
}

void Mesh::setMats(Material matIn)
{
   for (int i = 0; i < (int)faces.size(); i++)
   {
      faces[i]->mat = matIn;
   }
}

void Mesh::randMats()
{
   for (int i = 0; i < (int)faces.size(); i++)
   {
      faces[i]->mat.random();
   }
}

vector<Triangle*> Mesh::getAdj(vec3_t pt)
{
   vector<Triangle*> adj;
   for (int i = 0; i < (int)faces.size(); i++)
   {
      if (faces[i]->contains(pt))
      {
         adj.push_back(faces[i]);
      }
   }
   return adj;
}

vector<Triangle*> Mesh::getMidAdj(vec3_t pt)
{
   vector<Triangle*> adj;
   for (int i = 0; i < (int)faces.size(); i++)
   {
      if (faces[i]->edgeContains(pt))
      {
         adj.push_back(faces[i]);
      }
   }
   return adj;
}

vector<vec3_t*> Mesh::getAdjEdges(vec3_t pt)
{
   vector<vec3_t*> adj;
   // FOR each face in mesh
   for (int i = 0; i < (int)faces.size(); i++)
   {
      bool isFound = false;
      // IF current face contains pt
      if (faces[i]->contains(pt))
      {
         // FOR other two points on current face
         for (int offset = 0; offset < 3; offset++)
         {
            // Try first point.
            vec3_t *p1 = faces[i]->points[(offset + 1) % 3];
            if (*p1 == pt)
            {
               isFound = true;
            }
            else
            {
               isFound = false;
               // FOR each point in adj
               for (int j = 0; j < (int)adj.size(); j++)
               {
                  // IF adjacent edges vector already contains p1
                  //if (closeEnough(*p1, *adj[j]))
                  if (*p1 == *adj[j])
                  {
                     isFound = true;
                  }
               }
               // IF p1 is not already in adj
               if (!isFound)
               {
                  // Push p1 to adj
                  adj.push_back(p1);
               }
            }
         }
      }
   }
   //cout << "Returning vector of adjacent edges of size " << adj.size() << endl;
   return adj;
}

vector<Triangle*> Mesh::getAdj(vec3_t p1, vec3_t p2)
{
   vector<Triangle*> adj;
   for (int i = 0; i < (int)faces.size(); i++)
   {
      if (faces[i]->isNeighbor(p1, p2))
      {
         adj.push_back(faces[i]);
      }
   }
   return adj;
}

void Mesh::subdivide()
{
   vector<Triangle*> fullFaces;
   // FOR each face in the mesh.
   for (int i = 0; i < (int)faces.size(); i++)
   {
      Triangle* cur = faces[i];

      // Construct list of edgePoints.
      vector<vec3_t *> midPts;
      // FOR each edge in the current triangle.
      for (int edge = 0; edge < 3; edge++)
      {
         vec3_t curEdge[2] = {cur->getPoint(edge), cur->getPoint((edge + 2) % 3)};
         vec3_t *mid = new vec3_t(curEdge[1] + curEdge[0]);
         *mid /= 2;
         midPts.push_back(mid);
      }
      vector<vec3_t *>newPts;
      // FOR each edge point.
      // Hardcode this as 3? Because you'll be adding to the vector.
      //for (int midNdx = 0; midNdx < 3; midNdx++)
      for (int midNdx = 0; midNdx < (int)midPts.size(); midNdx++)
      {
         vec3_t *curPt = midPts[midNdx];
         cout << "point: " << *curPt << endl;
         // Find the opposite points.
         vector<Triangle*> adjFaces = getMidAdj(*curPt);
         for (int i = 0; i < (int)adjFaces.size(); i++)
         {
            adjFaces[i]->debug();
         }
         cout << "neighbors: " << adjFaces.size() << endl;
         if ((int)adjFaces.size() != 2)
         {
            cerr << "Invalid mesh. " << adjFaces.size() << " neighbors encountered.\n";
            exit(EXIT_FAILURE);
         }
         cout << endl << endl;
         cout << "point: " << *curPt << endl;
         vec3_t *newPt = new vec3_t();
         // FOR each adjacent triangle.
         for (int adjNdx = 0; adjNdx < (int)adjFaces.size(); adjNdx++)
         {
            Triangle *curFace = adjFaces[adjNdx];
            curFace->debug();
            // Get the index of the opposite point.
            int oppositeNdx = -1;
            curFace->edgeContains(*curPt, &oppositeNdx);
            // FOR each point in the current triangle.
            for (int ptNdx = 0; ptNdx < 3; ptNdx++)
            {
               vec3_t *curFacePt = curFace->points[ptNdx];
               // Weight the new point accordingly.
               if (ptNdx == oppositeNdx)
               {
                  vec3_t contrib = *curFacePt * 1.f/8.f;
                  cout << "\t" << *newPt << " + " << contrib;
                  *newPt += contrib;
                  cout << " = " << *newPt << endl;
               }
               // Only count the shared points once.
               else if (adjNdx == 0)
               {
                  vec3_t contrib = *curFacePt * 3.f/8.f;
                  cout << "\t" << *newPt << " + " << contrib;
                  *newPt += contrib;
                  cout << " = " << *newPt << endl;
               }
            }
         }
         cout << "final: " << endl;
         cout << "\tcurPt: " << *curPt << endl;
         cout << "\tnewPt: " << *newPt << endl;
         // Add the new midpoints to a new vector.
         newPts.push_back(newPt);

         //fullFaces.push_back(new Triangle(*midPts[midNdx], *midPts[(midNdx + 1) % 3], cur->getPoint(midNdx)));
      }
      midPts = newPts;
      for (int midNdx = 0; midNdx < (int)midPts.size(); midNdx++)
      {
         vec3_t last = cur->getPoint(midNdx);
         fullFaces.push_back(new Triangle(*midPts[midNdx], *midPts[(midNdx + 1) % 3], last));
      }
      //fullFaces.push_back(new Triangle(*midPts[0], *midPts[1], *midPts[2]));
      for (int i = 0; i < (int)midPts.size(); i++)
      {
         delete midPts[i];
      }
   }
   // ENDFOR
   faces.clear();
   faces.insert(faces.end(), fullFaces.begin(), fullFaces.end());
   cout << faces.size() << " total faces." << endl;
   randMats();
   delete boundingBox;
   boundingBox = bBox();
   //setMats(mat);
   // TODO: Free vectors and pointers.
}
/*
   void Mesh::subdivide()
   {
   vector<Triangle*> fullFaces;
// FOR each face in the mesh.
for (int i = 0; i < (int)faces.size(); i++)
{
Triangle* cur = faces[i];

int neighbors = 0;
neighbors += getAdj(cur->getPoint(0), cur->getPoint(1)).size();
neighbors += getAdj(cur->getPoint(1), cur->getPoint(2)).size();
neighbors += getAdj(cur->getPoint(2), cur->getPoint(0)).size();
//cout << "neighbors: " << neighbors << endl;
neighbors -= 3;
//cout << "neighbors (after): " << neighbors << endl;
// Find facePoint.
vec3_t facePt = cur->getFacePoint();
// Construct list of edgePoints.
vector<vec3_t*> edgePts;
for (int edge = 0; edge < 3; edge++)
{
vec3_t curEdge[2] = {cur->getPoint(edge), cur->getPoint((edge + 2) % 3)};
vector<Triangle*> adjFaces = getAdj(curEdge[0], curEdge[1]);
if (adjFaces.size() > 1)
{
vec3_t *curEdgePt = new vec3_t();

int adjCount = 0;

// Add face points of each neighboring face.
for (int adjNdx = 0; adjNdx < (int)adjFaces.size(); adjNdx++)
{
vec3_t adjPt = adjFaces[adjNdx]->getFacePoint();
 *curEdgePt += adjPt;
 adjCount++;
 }
 *curEdgePt /= (float)adjCount;
 cout << "facePt average: " << (*curEdgePt) << endl;

// Add edge's original midpoints to the current edge point.
vec3_t mid = curEdge[1] + curEdge[0];
mid /= 2;
// *curEdgePt += curEdge[0];
//adjCount++;
// *curEdgePt += curEdge[1];
//adjCount++;
cout << "mid: " << mid << endl;
 *curEdgePt += mid;
 cout << "curEdgePt: " << (*curEdgePt) << endl;
 *curEdgePt /= 2.0;
 cout << "curEdgePt / 2.0: " << (*curEdgePt) << endl;

// *curEdgePt /= (float)adjCount;
cout << "current edge point (" << edge << ", " << ((edge + 1) % 3) << "): " << *curEdgePt << endl;
edgePts.push_back(curEdgePt);
}
}

// Construct list of vertex points.
vector<vec3_t*> vPts;

// FOR each point in current face.
for (int pt = 0; pt < (int)edgePts.size(); pt++)
{
vec3_t point = cur->getPoint(pt);
// Find list of faces touching current point.
vector<Triangle*> adjFaces = getAdj(point);
// Find list of edges touching current point.
vector<vec3_t*> adjEdges = getAdjEdges(point);
// Set n to the number of faces touching current point.
int n = (int)adjEdges.size();
cout << "n = " << n << endl;
// Calculate F.
vec3_t F = vec3_t();
// FOR each face in adjFaces
for (int j = 0; j < (int)adjFaces.size(); j++)
{
   vec3_t fP = adjFaces[j]->getFacePoint();
   F += fP;
}
// Set F as the average of the facePoints of all adjacent faces.
F /= (float)adjFaces.size();
cout << "F: " << F << endl;
// Calculate R.
vec3_t R = vec3_t();
// FOR each edge in adjEdges
for (int k = 0; k < (int)adjEdges.size(); k++)
{
   // Calculate the midpoint of the edge between point and the current
   // edge point.
   vec3_t midPt = point;
   midPt -= *adjEdges[k];
   midPt /= 2;
   // Add the midpoint to R.
   R += midPt;
}
R /= (float)adjEdges.size();
cout << "n = " << n << endl;
if (n > 3)
{
   vec3_t r2 = R * 2;
   cout << "2R: " << r2 << endl;
   // Calculate new vertex point P.
   vec3_t P = r2 + F;
   vec3_t nMinus3P = point;
   nMinus3P *= ((float)n - 3);
   //nMinus3P *= 0.0;
   P += nMinus3P;
   P /= (float)n;
   //P /= (float)3;
   //cout << "original point: " << point << endl;
   vec3_t fP = cur->getFacePoint();
   //cout << "face point: " << fP << endl;
   //cout << "n: " << n << endl;
   cout << "new vertex point: " << P << endl << endl;
   //cout << "\t\tpt: " << pt << endl;
   //cout << "--------edgePts.size() = " << edgePts.size() << ", pt = " << pt << endl;
   //fullFaces.push_back(new Triangle(P, *edgePts[pt], *edgePts[(pt + 1) % edgePts.size()]));


   fullFaces.push_back(new Triangle(P, *edgePts[pt], *edgePts[(pt + 1) % edgePts.size()]));



   //fullFaces.push_back(new Triangle(point, *edgePts[pt], *edgePts[(pt + 1) % edgePts.size()]));
   //fullFaces.push_back(new Triangle(P, *edgePts[pt], facePt));
   //fullFaces.push_back(new Triangle(point, *edgePts[(pt + 1) % edgePts.size()], facePt));

   //fullFaces.push_back(new Triangle(*edgePts[0], *edgePts[1], P));
}
fullFaces.push_back(new Triangle(facePt, *edgePts[pt], *edgePts[(pt + 1) % edgePts.size()]));
}
// ENDFOR
}
// ENDFOR
faces.clear();
faces.insert(faces.end(), fullFaces.begin(), fullFaces.end());
cout << faces.size() << " total faces." << endl;
randMats();
delete boundingBox;
boundingBox = bBox();
setMats(mat);
// TODO: Free vectors and pointers.
}
*/
