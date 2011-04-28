/**
 * Sterling Hirsh
 * Scene Class
 * This has everything a scene needs.
 * 4/3/11
 */

#include "Scene.h"
#include <string>
#include <ctype.h>

using namespace std;

Scene* Scene::read(std::istream& input)
{
   Scene* curScene = new Scene();
   // Read file.

   // This will hold the name of our item.
   string curItemName = "";
   int curChar;

   while (!input.eof())
   {
      curChar = input.get();
      if (curChar == -1)
      {
         break;
      }

      if (isspace(curChar))
      {
         if (curItemName.length() == 0)
         {
            continue;
         }
         else
         {
            // Create items.

            if (curItemName.compare("camera") == 0)
            {
               // Create camera.
               curScene->camera = new Camera(input);
            }
            else if (curItemName.compare("light_source") == 0)
            {
               curScene->lights.push_back(new Light(input));
            }
            else if (curItemName.compare("box") == 0)
            {
               curScene->geometry.push_back(new Box(input));
            }
            else if (curItemName.compare("cone") == 0)
            {
               curScene->geometry.push_back(new Cone(input));
            }
            else if (curItemName.compare("plane") == 0)
            {
               curScene->geometry.push_back(new Plane(input));
            }
            else if (curItemName.compare("sphere") == 0)
            {
               curScene->geometry.push_back(new Sphere(input));
            }
            else if (curItemName.compare("semi") == 0)
            {
               curScene->geometry.push_back(new Semi(input));
            }
            else if (curItemName.compare("triangle") == 0)
            {
               curScene->geometry.push_back(new Triangle(input));
            }
            // Next do the other ones.

            curItemName = "";
         }
      }
      else if (curItemName.compare("//") == 0)
      {
         // Ignore the rest of the line.
         input.ignore(999, '\n');
         curItemName = "";
      }
      else
      {
         curItemName += (char) curChar;
      }
   }

   return curScene;
}

Pixel *Scene::seekLight(vec3_t origin, Geometry *hitObject)
{
   Pixel *result = new Pixel(0.0, 0.0, 0.0, 0.0);
   for (unsigned i = 0; i < lights.size(); i++)
   {
      vec3_t dir = lights[i]->location - origin;
      float dirLen = dir.length();
      dir.normalize();
      //vec3_t point = dir * 0.0001f + origin;
      //point += origin;
      Ray *feeler = new Ray(dir * 0.0001f + origin, dir);
      float t = -1.0;
      bool hit = false;
      for (unsigned int j = 0; j < geometry.size(); j++)
      {
         Geometry *curObject = geometry[j];
         bool intersect = curObject->hit(*feeler, &t);
         hit |= (intersect && t > 0 && t <= dirLen);
      }
      result->r = (hitObject->getAmbient()*hitObject->getR()) * lights[i]->r;
      result->g = (hitObject->getAmbient()*hitObject->getG()) * lights[i]->g;
      result->b = (hitObject->getAmbient()*hitObject->getB()) * lights[i]->b;
      result->a = 1.0;
      if (!hit)
      {
         vec3_t n = hitObject->getNormal(origin);
         n.normalize();
         vec3_t l = lights[i]->location - origin;
         l.normalize();
         float nDotL = n.dot(l);
         result->r += hitObject->getDiffuse()*hitObject->getR() * nDotL * lights[i]->r;
         result->g += hitObject->getDiffuse()*hitObject->getG() * nDotL * lights[i]->g;
         result->b += hitObject->getDiffuse()*hitObject->getB() * nDotL * lights[i]->b;
         result->r = std::min(result->r, 1.0f);
         result->g = std::min(result->g, 1.0f);
         result->b = std::min(result->b, 1.0f);
      }
   }
   return result;
}

Pixel* Scene::getIntersect(Ray ray)
{
   Pixel *result = new Pixel();
   Geometry *hitObject = new Geometry();
   float t;
   bool hitFound = false;
   float curDepth = 0.0;
   for (unsigned int i = 0; i < geometry.size(); i++)
   {
      Geometry *curObject = geometry[i];
      t = -1.0;
      bool intersected = curObject->hit(ray, &t);
      if (intersected)
      {
         if (t >= 0 && (!hitFound || (hitFound && t < curDepth)))
         {
            curDepth = t;
            hitObject = curObject;
         }
      }
      hitFound |= (intersected && curDepth > 0.0);
   }
   if (hitFound)
   {
      vec3_t rayDisp = ray.dir * curDepth;
      vec3_t hitPoint = ray.point + rayDisp;
      result = seekLight(hitPoint, hitObject);
   }
   return result;
}
