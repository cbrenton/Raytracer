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
   std::vector<Light*> lights_vec;
   std::vector<Geometry*> geometry_vec;
   std::vector<Plane*> planes_vec;
   std::vector<Sphere*> spheres_vec;

   Scene* curScene = new Scene();
   curScene->lights_size = curScene->geometry_size = curScene->planes_size = curScene->spheres_size = 0;
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
               lights_vec.push_back(new Light(input));
               curScene->lights_size++;
            }
            else if (curItemName.compare("box") == 0)
            {
               geometry_vec.push_back(new Box(input));
               curScene->geometry_size++;
            }
            else if (curItemName.compare("cone") == 0)
            {
               geometry_vec.push_back(new Cone(input));
               curScene->geometry_size++;
            }
            else if (curItemName.compare("plane") == 0)
            {
               planes_vec.push_back(new Plane(input));
               curScene->planes_size++;
            }
            else if (curItemName.compare("sphere") == 0)
            {
               spheres_vec.push_back(new Sphere(input));
               curScene->spheres_size++;
            }
            else if (curItemName.compare("semi") == 0)
            {
               geometry_vec.push_back(new Semi(input));
               curScene->geometry_size++;
            }
            else if (curItemName.compare("triangle") == 0)
            {
               geometry_vec.push_back(new Triangle(input));
               curScene->geometry_size++;
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

   curScene->lights = new Light *[curScene->lights_size];
   copy(lights_vec.begin(), lights_vec.end(), curScene->lights);
   curScene->geometry = new Geometry *[curScene->geometry_size];
   copy(geometry_vec.begin(), geometry_vec.end(), curScene->geometry);
   curScene->planes = new Plane *[curScene->planes_size];
   copy(planes_vec.begin(), planes_vec.end(), curScene->planes);
   curScene->spheres = new Sphere *[curScene->spheres_size];
   copy(spheres_vec.begin(), spheres_vec.end(), curScene->spheres);
   return curScene;
}

//Pixel *Scene::seekLight(vec3_t origin, Geometry *hitObject)
Pixel *Scene::seekLight(HitData *data)
{
   Pixel *result = new Pixel(0.0, 0.0, 0.0, 0.0);
   printf("origin: ");
   data->point.print();
   for (int i = 0; i < lights_size; i++)
   {
      vec3_t dir = lights[i]->location - data->point;
      float dirLen = dir.length();
      dir.normalize();
      Ray *feeler = new Ray(dir * 0.0001f + data->point, dir);
      float t = -1.0;
      bool hit = false;
      for (int j = 0; j < geometry_size; j++)
      {
         Geometry *curObject = geometry[j];
         bool intersect = curObject->hit(*feeler, &t);
         hit |= (intersect && t > 0 && t <= dirLen);
      }
      for (int j = 0; j < planes_size; j++)
      {
         Plane *curObject = planes[j];
         bool intersect = curObject->hit(*feeler, &t);
         hit |= (intersect && t > 0 && t <= dirLen);
      }
      for (int j = 0; j < spheres_size; j++)
      {
         Sphere *curObject = spheres[j];
         bool intersect = curObject->hit(*feeler, &t);
         hit |= (intersect && t > 0 && t <= dirLen);
      }
      result->r = (data->object->getAmbient()*data->object->getR()) * lights[i]->r;
      result->g = (data->object->getAmbient()*data->object->getG()) * lights[i]->g;
      result->b = (data->object->getAmbient()*data->object->getB()) * lights[i]->b;
      result->a = 1.0;
      if (!hit)
      {
         vec3_t n = data->object->getNormal(data->point);
         n.normalize();
         vec3_t l = lights[i]->location - data->point;
         l.normalize();
         float nDotL = n.dot(l);
         result->r += data->object->getDiffuse()*data->object->getR() * nDotL * lights[i]->r;
         result->g += data->object->getDiffuse()*data->object->getG() * nDotL * lights[i]->g;
         result->b += data->object->getDiffuse()*data->object->getB() * nDotL * lights[i]->b;
         result->r = std::min(result->r, 1.0f);
         result->g = std::min(result->g, 1.0f);
         result->b = std::min(result->b, 1.0f);
      }
      else
      {
         printf("hit\n");
      }
   }
   return result;
}

//Pixel* Scene::getIntersect(Ray ray)
HitData* Scene::getIntersect(Ray ray)
{
   HitData *data = new HitData();
   //Pixel *result = new Pixel();
   Geometry *hitObject = new Geometry();
   float t;
   bool hitFound = false;
   float curDepth = 0.0;
   for (int i = 0; i < geometry_size; i++)
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
   for (int i = 0; i < planes_size; i++)
   {
      Plane *curObject = planes[i];
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
   for (int i = 0; i < spheres_size; i++)
   {
      Sphere *curObject = spheres[i];
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
      data->hit = true;
      data->point = ray.dir * curDepth;
      data->t = curDepth;
      data->object = hitObject;
   }
   return data;
}
