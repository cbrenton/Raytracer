/**
 * Chris Brenton
 * Triangle object for raytracing.
 * 473
 * 4/7/11
 */

#include <cstdlib>
#include <string>
#include "Triangle.h"
#include "Matrix4.h"

#define EXP_ARGS 9

Triangle::Triangle()
{
   facePt = getFacePoint();
   points[0] = &location;
   points[1] = &corner2;
   points[2] = &corner3;
}

Triangle::Triangle(std::istream& input) : Geometry()
{
   std::string line;
   getline(input, line);
   int scan = 0;
   //bool isDone = false;
   int braceCount = 0;
   //for (size_t i = 0; i < line.size(); i++)
   size_t start = line.find("triangle {");
   if (start == string::npos)
   {
      if (line[0] == '{')
      {
         start = 1;
      }
      else
      {
         start = 0;
      }
   }
   for (size_t i = start; i < line.size(); i++)
   {
      if (line[i] == '{')
      {
         braceCount++;
      }
      else if (line[i] == '}')
      {
         braceCount--;
      }
   }
   //isDone = (braceCount < 0);
   //while (line == "")
   while (line.empty())
   {
      getline(input, line);
   }
   // If the line is only an opening curly brace, skip it.
   if (line == "{" || line == " {")
   {
      // Get the good stuff.
      getline(input, line);
      scan = sscanf(line.c_str(), " < %f , %f , %f > , < %f , %f , %f > , < %f , %f , %f >",
            &location.v[0], &location.v[1], &location.v[2],
            &corner2.v[0], &corner2.v[1], &corner2.v[2],
            &corner3.v[0], &corner3.v[1], &corner3.v[2]);
   }
   else
   {
      scan = sscanf(line.c_str(), " { < %f , %f , %f > , < %f , %f , %f > , < %f , %f , %f >",
            &location.v[0], &location.v[1], &location.v[2],
            &corner2.v[0], &corner2.v[1], &corner2.v[2],
            &corner3.v[0], &corner3.v[1], &corner3.v[2]);
      /*
         if (scan == 0)
         {
         scan = sscanf(line.c_str(), " < %f , %f , %f > , < %f , %f , %f > , < %f , %f , %f >",
         &location.v[0], &location.v[1], &location.v[2],
         &corner2.v[0], &corner2.v[1], &corner2.v[2],
         &corner3.v[0], &corner3.v[1], &corner3.v[2]);
         }
         */
   }
   if (scan < EXP_ARGS)
   {
      printf("Invalid triangle format: expected %d, found %d.\n", EXP_ARGS, scan);
      std::cout << "\tline: " << line << std::endl;
      exit(EXIT_FAILURE);
   }
   /*
      if (!isDone)
      {
   //matSet = readOptions(input);
   readOptions(input);
   }
   */
   readOptions(input);
   boundingBox = bBox();
   facePt = getFacePoint();
   points[0] = &location;
   points[1] = &corner2;
   points[2] = &corner3;
}

Triangle::Triangle(vec3_t c1, vec3_t c2, vec3_t c3) : Geometry()
{
   location = c1;
   corner2 = c2;
   corner3 = c3;
   boundingBox = bBox();
   facePt = getFacePoint();
   points[0] = &location;
   points[1] = &corner2;
   points[2] = &corner3;
}

Triangle::~Triangle()
{
   delete boundingBox;
}

Box* Triangle::bBox()
{
   float minX, minY, minZ, maxX, maxY, maxZ;
   minX = min(min(location.x(), corner2.x()), corner3.x());
   maxX = max(max(location.x(), corner2.x()), corner3.x());
   minY = min(min(location.y(), corner2.y()), corner3.y());
   maxY = max(max(location.y(), corner2.y()), corner3.y());
   minZ = min(min(location.z(), corner2.z()), corner3.z());
   maxZ = max(max(location.z(), corner2.z()), corner3.z());
   vec3_t c1 = vec3_t(minX, minY, minZ);
   vec3_t c2 = vec3_t(maxX, maxY, maxZ);
   return new Box(c1, c2);
}

bool Triangle::hit(Ray ray, float *t, HitData *data, float minT, float maxT)
{
   float result = -1;
   float bBeta, bGamma, bT;

   Matrix4 A = Matrix4(
         location.x()-corner2.x(), location.x()-corner3.x(),
         ray.dir.x(), 0,
         location.y()-corner2.y(), location.y()-corner3.y(),
         ray.dir.y(), 0,
         location.z()-corner2.z(), location.z()-corner3.z(),
         ray.dir.z(), 0,
         0, 0, 0, 1);
   float detA = A.det3();

   Matrix4 baryT = Matrix4(
         location.x()-corner2.x(), location.x()-corner3.x(),
         location.x()-ray.point.x(), 0,
         location.y()-corner2.y(), location.y()-corner3.y(),
         location.y()-ray.point.y(), 0,
         location.z()-corner2.z(), location.z()-corner3.z(),
         location.z()-ray.point.z(), 0,
         0, 0, 0, 1);

   bT = baryT.det3() / detA;
   //if (bT < 0 || bT > F_MAX)
   if (bT < 0)
   {
      result = 0;
   }
   else
   {
      Matrix4 baryGamma = Matrix4(
            location.x()-corner2.x(), location.x()-ray.point.x(),
            ray.dir.x(), 0,
            location.y()-corner2.y(), location.y()-ray.point.y(),
            ray.dir.y(), 0,
            location.z()-corner2.z(), location.z()-ray.point.z(),
            ray.dir.z(), 0,
            0, 0, 0, 1);

      bGamma = baryGamma.det3() / detA;
      if (bGamma < 0 || bGamma > 1)
      {
         result = 0;
      }
      else
      {
         Matrix4 baryBeta = Matrix4(
               location.x()-ray.point.x(), location.x()-corner3.x(),
               ray.dir.x(), 0,
               location.y()-ray.point.y(), location.y()-corner3.y(),
               ray.dir.y(), 0,
               location.z()-ray.point.z(), location.z()-corner3.z(),
               ray.dir.z(), 0,
               0, 0, 0, 1);

         bBeta = baryBeta.det3() / detA;
         if (bBeta < 0 || bBeta > 1 - bGamma)
         {
            result = 0;
         }
      }
   }

   if (result != 0)
   {
      result = bT;
   }
   *t = result;
   if (result > 0.01 && data)
   {
      data->hit = true;
      data->point = ray.dir * (*t);
      data->point += ray.point;
      data->t = (*t);
      data->object = this;
   }
   return (result > 0.01);
}

bool Triangle::hit(vec3_t pt)
{
   Ray *tmp = new Ray(pt, vec3_t(0, 0, 0));
   float t = 0;
   bool ret = hit(*tmp, &t);
   delete tmp;
   return ret;
}

vec3_t Triangle::getNormal(vec3_t point)
{
   vec3_t s1 = corner2 - location;
   vec3_t s2 = corner3 - location;
   vec3_t normal;
   normal.cross(s1, s2);
   return normal;
}

vec3_t Triangle::getPoint(int pt)
{
   switch(pt)
   {
   case 0:
      return location;
      break;
   case 1:
      return corner2;
      break;
   case 2:
      return corner3;
      break;
   }
   return vec3_t(0, 0, 0);
}

bool Triangle::contains(vec3_t pt)
{
   return (location == pt || corner2 == pt || corner3 == pt);
}

bool Triangle::edgeContains(vec3_t pt, int *opposite)
{
   for (int i = 0; i < 3; i++)
   {
      vec3_t curPt = *points[i];
      vec3_t nextPt = *points[(i + 1) % 3];
      vec3_t edgeVec = nextPt - curPt;
      vec3_t a_to_b = pt - curPt;
      vec3_t a_to_c = nextPt - pt;
      if (closeEnough(edgeVec.length(), a_to_b.length() + a_to_c.length()))
      {
         if (opposite)
         {
            *opposite = (i + 2) % 3;
         }
         return true;
      }
   }
   return false;
}

bool Triangle::isNeighbor(Triangle *other)
{
   int samePts = 0;
   if (other->contains(location))
   {
      samePts++;
   }
   if (other->contains(corner2))
   {
      samePts++;
   }
   if (other->contains(corner3))
   {
      samePts++;
   }
   return (samePts == 2);
}

bool Triangle::isNeighbor(vec3_t c1, vec3_t c2)
{
   return (contains(c1) && contains(c2));
}

void Triangle::findAdj(vector<Triangle*> tris)
{
   for (size_t i = 0; i < tris.size(); i++)
   {
      if (isNeighbor(tris[i]))
      {
         adj.push_back(tris[i]);
      }
   }
}

vector<Triangle*> Triangle::subdivide()
{
   /*
      if (adj.size() != 3)
      {
      cerr << "Mesh error." << endl;
      }
      */
   vector<Triangle*> lC2Adj;
   vector<Triangle*> lC3Adj;
   vector<Triangle*> c2C3Adj;
   for (size_t i = 0; i < adj.size(); i++)
   {
      if (adj[i]->isNeighbor(location, corner2))
      {
         lC2Adj.push_back(adj[i]);
      }
      if (adj[i]->isNeighbor(location, corner3))
      {
         lC3Adj.push_back(adj[i]);
      }
      if (adj[i]->isNeighbor(corner2, corner3))
      {
         c2C3Adj.push_back(adj[i]);
      }
   }
   //cout << facePt << endl;
   vector<Triangle*> ret;
   vector<vec3_t*> edgePts;
   // TODO: Comment this better.
   vec3_t q = vec3_t(0, 0, 0);
   vec3_t lC2Edge = location + corner2;
   vec3_t lC3Edge = location + corner3;
   vec3_t c2C3Edge = corner2 + corner3;
   cout << lC2Edge << endl;
   cout << lC3Edge << endl;
   cout << c2C3Edge << endl;
   lC2Edge += facePt;
   lC3Edge += facePt;
   c2C3Edge += facePt;
   for (size_t j = 0; j < lC2Adj.size(); j++)
   {
      vec3_t facePt = lC2Adj[j]->getFacePoint();
      lC2Edge += facePt;
      q += facePt;
      //cout << "Q: + " << facePt << " = " << q << endl;
   }
   for (size_t k = 0; k < lC3Adj.size(); k++)
   {
      vec3_t facePt = lC3Adj[k]->getFacePoint();
      lC3Edge += facePt;
      q += facePt;
      //cout << "Q: + " << facePt << " = " << q << endl;
   }
   for (size_t l = 0; l < c2C3Adj.size(); l++)
   {
      vec3_t facePt = c2C3Adj[l]->getFacePoint();
      c2C3Edge += facePt;
      q += facePt;
      //cout << "Q: + " << facePt << " = " << q << endl;
   }
   q /= (float)adj.size();
   cout << "Q: " << q << endl;
   lC2Edge /= 3 + (float)lC2Adj.size();
   lC3Edge /= 3 + (float)lC3Adj.size();
   c2C3Edge /= 3 + (float)c2C3Adj.size();
   if (c2C3Adj.size() == 0)
   {
      ret.push_back(new Triangle(facePt, lC2Edge, lC3Edge));
      ret.push_back(new Triangle(q, lC2Edge, lC3Edge));
   }
   if (lC2Adj.size() == 0)
   {
      ret.push_back(new Triangle(facePt, c2C3Edge, lC3Edge));
      ret.push_back(new Triangle(q, c2C3Edge, lC3Edge));
   }
   if (lC3Adj.size() == 0)
   {
      ret.push_back(new Triangle(facePt, c2C3Edge, lC2Edge));
      ret.push_back(new Triangle(q, c2C3Edge, lC2Edge));
   }
   return ret;
}

vec3_t Triangle::getFacePoint()
{
   vec3_t ret;
   ret = location + corner2 + corner3;
   ret /= 3;
   return ret;
}

vec3_t Triangle::getMidAvg(vec3_t pt)
{
   vec3_t avg = vec3_t();
   if (location != pt)
   {
      vec3_t mid = (pt + location);
      mid /= 2;
      avg += mid;
   }
   if (corner2 != pt)
   {
      vec3_t mid = (pt + corner2);
      mid /= 2;
      avg += mid;
   }
   if (corner3 != pt)
   {
      vec3_t mid = (pt + corner3);
      mid /= 2;
      avg += mid;
   }
   avg /= 2;
   return avg;
}

triangle_dev_t Triangle::getStruct()
{
   float c1[3] = {location.v[0], location.v[1], location.v[2]};
   float c2[3] = {corner2.v[0], corner2.v[1], corner2.v[2]};
   float c3[3] = {corner3.v[0], corner3.v[1], corner3.v[2]};
   triangle_dev_t ret;
   ret.c1[0] = c1[0];
   ret.c1[1] = c1[1];
   ret.c1[2] = c1[2];
   ret.c2[0] = c2[0];
   ret.c2[1] = c2[1];
   ret.c2[2] = c2[2];
   ret.c3[0] = c3[0];
   ret.c3[1] = c3[1];
   ret.c3[2] = c3[2];
   return ret;
}

void Triangle::debug()
{
   printf("Triangle: {\n");
   printf("  <%f, %f, %f>,\n",
         location.v[0], location.v[1], location.v[2]);
   printf("  <%f, %f, %f>,\n",
         corner2.v[0], corner2.v[1], corner2.v[2]);
   printf("  <%f, %f, %f>\n",
         corner3.v[0], corner3.v[1], corner3.v[2]);
}
