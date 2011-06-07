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
   if (line == "{")
   {
      braceCount++;
      getline(input, line);
   }
   istringstream lineSS;
   if (line.find("vertex_vectors {") != string::npos)
   {
      getline(input, line);
      // Find the index of the first comment.
      unsigned comment = line.find("//");
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
         unsigned comment = line.find("//");
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
      unsigned comment = line.find("//");
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
         unsigned comment = line.find("//");
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
                  faces.push_back(new Triangle(*points[p1], *points[p2], *points[p3]));
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
   //setMats(mat);
   randMats();
   boundingBox = bBox();
   subdivide();
}

Mesh::~Mesh()
{
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
      extrema[dim][0] = faces[0]->bBox()->getMin().v[dim];
      extrema[dim][1] = faces[0]->bBox()->getMax().v[dim];
   }
   for (unsigned i = 0; i < faces.size(); i++)
   {
      vec3_t c1, c2;
      Box *faceBox = faces[i]->bBox();
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

   for (unsigned i = 0; i < faces.size(); i++)
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
   for (unsigned i = 0; i < faces.size(); i++)
   {
      faces[i]->mat = matIn;
   }
}

void Mesh::randMats()
{
   for (unsigned i = 0; i < faces.size(); i++)
   {
      faces[i]->mat.random();
   }
}

void Mesh::subdivide()
{
   vector<Triangle*> fullFaces;
   for (unsigned i = 0; i < faces.size(); i++)
   {
      faces[i]->findAdj(faces);
      //cout << faces.size() << endl;
      vector<Triangle*> newFaces = faces[i]->subdivide();
      fullFaces.insert(fullFaces.end(), newFaces.begin(), newFaces.end());
   }
   /*
   vector<vec3_t*> facePts;
   vector<vec3_t*> fullPts;
   vector<Triangle*> fullFaces;
   for (unsigned i = 0; i < faces.size(); i++)
   {
      vec3_t facePoint = faces[i]->facePt;
      facePts.push_back(&facePoint);
      vec3_t edgePts[3];
      // TODO: comment this better
      edgePts[0] = faces[i]->location + faces[i]->corner2 + faces[i]->facePt;
      edgePts[1] = faces[i]->corner2 + faces[i]->corner3 + faces[i]->facePt;
      edgePts[2] = faces[i]->location + faces[i]->corner3 + faces[i]->facePt;
      // Find first edge point (location -> corner2).
      for (unsigned j = 0; j < faces.size(); j++)
      {
         if (i != j && faces[j]->isNeighbor(faces[i]->location, faces[i]->corner2))
         {
            edgePts[0] += faces[j]->facePt;
            cout << i << "[0]" << endl;
         }
         if (i != j && faces[j]->isNeighbor(faces[i]->corner2, faces[i]->corner3))
         {
            edgePts[1] += faces[j]->facePt;
            cout << i << "[1]" << endl;
         }
         if (i != j && faces[j]->isNeighbor(faces[i]->location, faces[i]->corner3))
         {
            edgePts[2] += faces[j]->facePt;
            cout << i << "[2]" << endl;
         }
      }
      // TODO: move original points
      vec3_t newLoc = faces[i]->location;
      vec3_t newC2 = faces[i]->corner2;
      vec3_t newC3 = faces[i]->corner3;
      // TODO: comment this better
      fullFaces.push_back(new Triangle(faces[i]->facePt, edgePts[0], newLoc));
      fullFaces.push_back(new Triangle(faces[i]->facePt, edgePts[0], newC2));
      fullFaces.push_back(new Triangle(faces[i]->facePt, edgePts[1], newC2));
      fullFaces.push_back(new Triangle(faces[i]->facePt, edgePts[1], newC3));
      fullFaces.push_back(new Triangle(faces[i]->facePt, edgePts[2], newLoc));
      fullFaces.push_back(new Triangle(faces[i]->facePt, edgePts[2], newC3));
   }
   */

   faces.clear();
   faces.insert(faces.end(), fullFaces.begin(), fullFaces.end());
   cout << faces.size() << " total faces." << endl;
   randMats();
   //setMats(mat);
   // TODO: Free vectors and pointers.
}
