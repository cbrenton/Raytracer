/**
 * Chris Brenton
 * Mesh object for raytracing.
 * Can be a subdivision surface.
 * 473
 * 4/7/11
 */

#include <cstdlib>
#include <string>
#include "Mesh.h"

#define EXP_ARGS 9

Mesh::Mesh(std::istream& input) : Geometry()
{
   std::string line;
   getline(input, line);
   while (line != "}")
   {
      vec3_t c1, c2, c3;
      int scan = sscanf(line.c_str(), " face { < %f , %f , %f > , < %f , %f , %f > , < %f , %f , %f >",
            &c1.v[0], &c1.v[1], &c1.v[2],
            &c2.v[0], &c2.v[1], &c2.v[2],
            &c3.v[0], &c3.v[1], &c3.v[2]);
      if (scan == EXP_ARGS)
      {
         faces.push_back(new Triangle(c1, c2, c3));
      }
      getline(input, line);
   }
   //findTriLine(input);
   //readOptions(input);
   Material green = Material(0.0, 1.0, 0.0, 1.0,
         0.4f, 0.4f, 0.1f, 1.0,
         0.0, false, 0.0);
   setMats(green);
   boundingBox = bBox();
}

void Mesh::findTriLine(std::istream& input)
{
   string word = "";
   char c = ' ';
   input >> ws;
   input >> c;
   word.append(1, c);
   while (c != ' ' && c != '\n' &&
         c != '}' &&
         word != "//" &&
         word != "triangle")
   {
      input >> c;
      word.append(1, c);
   }
   if (word == "triangle")
   {
      cout << "\tadded triangle to mesh." << endl;
      //faces.push_back(new Triangle(input));
   }
   else if (word.find("//") != string::npos)
   {
      input.ignore(256, '\n');
      findTriLine(input);
      return;
   }
   else if (c != '}')
   {
      findTriLine(input);
      return;
   }
}

bool Mesh::isTriLine(string line, bool *isComment)
{
   int start = line.find_first_not_of("   ");
   unsigned commentIndex = line.find("//", start);
   unsigned triIndex = line.find("triangle", start);
   *isComment = (commentIndex != string::npos);
   cout << "start: " << start << endl;
   if (commentIndex != string::npos)
      cout << "comment: " << commentIndex << endl;
   if (triIndex != string::npos)
      cout << "triIndex: " << triIndex << endl;
   return (triIndex != string::npos) || isComment;
}

Box* Mesh::bBox()
{
   float extrema[3][2];
   for (unsigned i = 0; i < faces.size(); i++)
   {
      vec3_t c1, c2;
      Box *faceBox = faces[i]->bBox();
      c1 = faceBox->getMin();
      c2 = faceBox->getMax();
      cout << "min: " << c1 << endl;
      cout << "max: " << c2 << endl;
      for (int j = 0; j < 3; j++)
      {
         extrema[i][0] = min(extrema[i][0], c1.v[i]);
         extrema[i][0] = max(extrema[i][0], c2.v[i]);
      }
   }
   vec3_t b_c1, b_c2;
   b_c1 = vec3_t(extrema[0][0], extrema[1][0], extrema[2][0]);
   b_c2 = vec3_t(extrema[0][1], extrema[1][1], extrema[2][1]);
   //cout << "min: " << b_c1 << endl;
   //cout << "max: " << b_c2 << endl;
   return new Box(b_c1, b_c2);
}

bool Mesh::hit(Ray ray, float *t, HitData *data, float minT, float maxT)
{
   bool hitFound = false;
   float geomT = -1.0;
   float curDepth = -1.0;
   //HitData *closestData = new HitData();

   for (unsigned i = 0; i < faces.size(); i++)
   {
      Triangle *curObject = faces[i];
      geomT = -1.0;
      HitData tmpData;
      bool intersected = false;
      intersected = curObject->hit(ray, &geomT, &tmpData);
      if (intersected)
      {
         if (geomT >= 0 && (!hitFound || (hitFound && geomT < curDepth)))
         {
            curDepth = geomT;
            //*closestData = tmpData;
            *data = tmpData;
         }
      }
      hitFound |= (intersected && curDepth > 0.0);
   }
   if (hitFound && (curDepth > 0.0))
   {
      //*data = *closestData;
      *t = curDepth;
   }
   return hitFound;
}

vec3_t Mesh::getNormal(vec3_t point)
{
   cout << "NORMAL" << endl;
   return vec3_t(0.0, 0.0, 0.0);
}

void Mesh::setMats(Material matIn)
{
   for (unsigned i = 0; i < faces.size(); i++)
   {
      faces[i]->mat = matIn;
   }
}
