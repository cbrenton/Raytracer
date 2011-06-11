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

vector<Triangle*> Mesh::getAdj(Triangle *tri)
{
   vector<Triangle*> adj;
   for (int i = 0; i < (int)faces.size(); i++)
   {
      if (faces[i]->isNeighbor(tri))
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
               /*
                  for (int j = 0; j < (int)adj.size(); j++)
                  {
               // IF adjacent edges vector already contains p1
               //if (closeEnough(*p1, *adj[j]))
               if (*p1 == *adj[j])
               {
               isFound = true;
               }
               }
               */
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

   // Generate new points.

   // Generate new face points.              /* ALREADY DONE */
   // FOR each face in mesh                  /* ALREADY DONE */
   // Add facePoint at center of face        /* ALREADY DONE */
   // ENDFOR                                 /* ALREADY DONE */

   // Generate new edge points.
   // Make list of edges in mesh.
   vector<vec3_t*> edgeList;
   // FOR each face in mesh
   for (int faceNdx = 0; faceNdx < (int)faces.size(); faceNdx++)
   {
      Triangle *curFace = faces[faceNdx];
      // FOR each edge in current face
      for (int edgeNdx = 0; edgeNdx < 3; edgeNdx++)
      {
         // Edge start point.
         vec3_t point1 = curFace->getPoint(edgeNdx);
         // Edge end point.
         vec3_t point2 = curFace->getPoint((edgeNdx + 1) % 3);
         // Create temporary edge point variable (newEdgePt).
         vec3_t *newEdgePt = new vec3_t();
         // Add midpoint of current edge to newEdgePt.
         vec3_t mid = point2 + point1;
         mid /= 2;
         *newEdgePt += mid;

         // Create temporary facePoint average variable (facePtTot).
         vec3_t facePtTot = vec3_t();
         // Create variable neighborCount to track number of neighboring faces.
         int neighborCount = 0;
         // FOR each face in mesh
         for (int neighborNdx = 0; neighborNdx < (int)faces.size(); neighborNdx++)
         {
            // IF current face contains current edge
            if (faces[neighborNdx]->contains(point1, point2))
            {
               // Add current face's facePoint to facePtTot.
               vec3_t curFacePt = faces[neighborNdx]->getFacePoint();
               facePtTot += curFacePt;
               // Increment neighborCount.
               neighborCount++;
            }
            // ENDIF
         }
         // ENDFOR


         // Add facePtTot to newEdgePt.
         *newEdgePt += facePtTot;
         // Divide newEdgePt by (neighborCount + 1).
         *newEdgePt /= (float)(neighborCount + 1);
         //cout << "neighborCount: " << neighborCount << endl;
         // Add newEdgePt to current face's edgePts vector.
         curFace->edgePts.push_back(newEdgePt);
      }
      // ENDFOR
   }
   // ENDFOR

   // Generate new vertex points.
   // FOR each face in mesh
   for (int faceNdx2 = 0; faceNdx2 < (int)faces.size(); faceNdx2++)
   {
      Triangle *curFace = faces[faceNdx2];

      // Find the average of new face points adjacent to current face (Q).
      vec3_t Q = vec3_t();
      vector<Triangle*> adjFaces = getAdj(curFace);
      for (int adjNdx = 0; adjNdx < (int)adjFaces.size(); adjNdx++)
      {
         vec3_t curFacePt = adjFaces[adjNdx]->getFacePoint();
         Q += curFacePt;
      }
      Q /= (float)adjFaces.size();


      /*
      // Find the average of old edge midpoints adjacent to current face (R).
      vec3_t R = vec3_t();
      vector<vec3_t*> adjEdges = getAdjEdges();
      */



      // FOR each point in current face
      for (int pointNdx = 0; pointNdx < 3; pointNdx++)
      {
         vec3_t curPoint = curFace->getPoint(pointNdx);
         ///*
         // Find the average of old edge midpoints adjacent to current face (R).
         vec3_t R = vec3_t();
         vector<vec3_t*> adjEdges = getAdjEdges(curFace->getPoint(pointNdx));
         //*/
         for (int adjEdgeNdx = 0; adjEdgeNdx < (int)adjEdges.size();
               adjEdgeNdx++)
         {
            vec3_t edgeMid = curFace->getPoint(pointNdx);
            edgeMid += *adjEdges[adjEdgeNdx];
            edgeMid /= 2;
            R += edgeMid;
            //cout << "edge #" << adjEdgeNdx << endl;
         }
         R /= (float)adjEdges.size();
         /*
            for (int i = 0; i < (int)adjFaces.size(); i++)
            {
            vec3_t faceMidAvg = adjFaces[i]->getMidAvg(curPoint);
            R += faceMidAvg;
            }
            R /= (float)adjFaces.size();
            */

         // Create new temporary vertex point variable (newVert).
         vec3_t newVert = vec3_t();
         // Find old vertex point (S).
         vec3_t S = curFace->getPoint(pointNdx);
         // Find number of edges incident to current face (n).
         //int n = adjFaces.size();
         int n = adjEdges.size();
         // Calculate Q / n.
         vec3_t Q2 = Q / (float)n;
         //cout << "\tF / n: " << Q2 << endl;
         // Calculate 2R / n.
         vec3_t R2 = R * 2.0;
         R2 /= (float)n;
         //cout << "\t2R / n: " << R2 << endl;
         // Calculate S(n - 3) / n.
         vec3_t S2 = S * (float)(n - 3);
         S2 /= (float)n;
         //cout << "\tS(n - 3) / n: " << S2 << endl;
         // Calculate new vertex point (newVert).
         newVert = Q2 + R2 + S2;

         // Connect new points.
         // Make a new triangle from the current face point, an adjacent edge
         // point, and the current vertex point.
         vec3_t curFacePt = curFace->getFacePoint();
         //cout << "--------\nfaceNdx2: " << faceNdx2 << endl;
         //cout << "original point: " << curPoint << endl;
         //cout << "F: " << Q << endl;
         //cout << "R: " << R << endl;
         //cout << "S: " << S << endl;
         //cout << "n: " << n << ", n - 3: " << (n - 3) << endl;
         //cout << "P: " << newVert << endl;
         vec3_t nextFacePt = faces[faceNdx2]->getFacePoint();
         vec3_t curEdgePt = *curFace->edgePts[pointNdx % curFace->edgePts.size()];
         vec3_t nextEdgePt = *curFace->edgePts[(pointNdx + 2) % curFace->edgePts.size()];
         //vec3_t curEdgePt = *curFace->edgePts[(pointNdx + 2) % curFace->edgePts.size()];
         //Triangle *newTri1 = new Triangle(curFacePt, curEdgePt, newVert);
         //Triangle *newTri1 = new Triangle(curPt, curEdgePt, newVert);
         //Triangle *newTri1 = new Triangle(curFacePt, curEdgePt, nextEdgePt);
         //Triangle *newTri2 = new Triangle(newVert, curEdgePt, nextEdgePt);


         //if (adjEdges.size() > 3)
         //if (pointNdx == 1)
         //{
            //Triangle *newTri1 = new Triangle(newVert, curEdgePt, curFacePt);
            Triangle *newTri1 = new Triangle(newVert, curEdgePt, curFacePt);
            //Triangle *newTri1 = new Triangle(vec3_t(0, 0, 0), curEdgePt, curFacePt);
            //Triangle *newTri2 = new Triangle(newVert, curEdgePt, nextEdgePt);
            Triangle *newTri2 = new Triangle(newVert, nextEdgePt, curFacePt);
            //Triangle *newTri1 = new Triangle(curFacePt, curEdgePt, nextEdgePt);
            // Add the new triangle to fullFaces.
            fullFaces.push_back(newTri1);
            fullFaces.push_back(newTri2);
         //}


         //fullFaces.push_back(newTri2);
      }
      // ENDFOR
   }
   // ENDFOR

   /*
      for (int i = 0; i < (int)faces.size(); i++)
      {
         vec3_t curFacePt = faces[i]->getFacePoint();
         vec3_t curEdgePt = *faces[i]->edgePts[0];
         vec3_t prevEdgePt = *faces[i]->edgePts[1];
         vec3_t nextEdgePt = *faces[i]->edgePts[2];
         //vec3_t curEdgePt = *curFace->edgePts[i];
         //vec3_t nextEdgePt = *faces[i]->edgePts[(i + 1) % faces[i]->edgePts.size()];
         //Triangle *newTri1 = new Triangle(curFacePt, curEdgePt, nextEdgePt);
         //Triangle *newTri2 = new Triangle(curFacePt, curEdgePt, prevEdgePt);
         // Add the new triangle to fullFaces.
         //fullFaces.push_back(newTri1);
         //fullFaces.push_back(newTri2);
      }
      */

   // Connect new points.
   // Make a new triangle from each face point

   cout << "number of faces: " << faces.size() << " -> " << fullFaces.size() << endl;
   // Clear faces.
   faces.clear();
   // Insert fullFaces into faces.
   faces.insert(faces.begin(), fullFaces.begin(), fullFaces.end());
   //randMats();
   delete boundingBox;
   boundingBox = bBox();
   setMats(mat);
   // TODO: Free vectors and pointers.
}
