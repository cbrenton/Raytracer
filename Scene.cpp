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

Scene* Scene::read(istream& input)
{
   vector<Light*> lights_vec;
   vector<Geometry*> geometry_vec;
   vector<Plane*> planes_vec;
   vector<Sphere*> spheres_vec;
   vector<Triangle*> triangles_vec;

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
               triangles_vec.push_back(new Triangle(input));
               curScene->triangles_size++;
            }
            // Next do the other ones.

            curItemName = "";
         }
      }
      else if (curItemName.compare("/") == 0 && (char) curChar == '/')
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
   curScene->triangles = new Triangle *[curScene->triangles_size];
   copy(triangles_vec.begin(), triangles_vec.end(), curScene->triangles);
   return curScene;
}

Pixel *Scene::seekLight(HitData *data, vec3_t view)
{
   Pixel *result = new Pixel(0.0, 0.0, 0.0, 0.0);
   for (int i = 0; i < lights_size; i++)
   {
      vec3_t dir = lights[i]->location - data->point;
      float dirLen = dir.length();
      dir.normalize();
      //Ray *feeler = new Ray(dir * 0.001f + data->point, dir);
      Ray *feeler = new Ray(data->point, dir);
      float t = -1.0;
      bool hit = false;
      float threshhold = 0.01f;
      for (int j = 0; j < geometry_size && !hit; j++)
      {
         Geometry *curObject = geometry[j];
         bool intersect = curObject->hit(*feeler, &t);
         hit |= (intersect && t > threshhold && t <= dirLen);
      }
      for (int j = 0; j < planes_size && !hit; j++)
      {
         Plane *curObject = planes[j];
         bool intersect = curObject->hit(*feeler, &t);
         hit |= (intersect && t > threshhold  && t <= dirLen);
      }
      for (int j = 0; j < spheres_size && !hit; j++)
      {
         Sphere *curObject = spheres[j];
         bool intersect = curObject->hit(*feeler, &t);
         hit |= (intersect && t > threshhold && t <= dirLen);
      }
      for (int j = 0; j < triangles_size && !hit; j++)
      {
         Triangle *curObject = triangles[j];
         bool intersect = curObject->hit(*feeler, &t);
         hit |= (intersect && t > threshhold && t <= dirLen);
      }
      // Ambient.
      result->r = (data->object->getAmbient()*data->object->getR())
         * lights[i]->r;
      result->g = (data->object->getAmbient()*data->object->getG())
         * lights[i]->g;
      result->b = (data->object->getAmbient()*data->object->getB())
         * lights[i]->b;
      result->a = 1.0;
      if (!hit)
      {
         // Diffuse.
         vec3_t n = data->object->getNormal(data->point);
         n.normalize();
         vec3_t l = lights[i]->location - data->point;
         l.normalize();
         float nDotL = n.dot(l);
         nDotL = min(nDotL, 1.0f);
         if (nDotL > 0)
         {
            result->r += data->object->getDiffuse()*data->object->getR()
               * nDotL * lights[i]->r;
            result->g += data->object->getDiffuse()*data->object->getG()
               * nDotL * lights[i]->g;
            result->b += data->object->getDiffuse()*data->object->getB()
               * nDotL * lights[i]->b;
         }
         // Specular.
         /*
         // Blinn.
         vec3_t v = view;
         v.normalize();
         vec3_t h = l + v;
         h.normalize();
         float nDotH = n.dot(h);
         pow(nDotH, data->object->getRoughness());
         nDotH = max(nDotH, 0.0f);
         nDotH = min(nDotH, 1.0f);
         result->r += data->object->getSpecular()*data->object->getR()
          * nDotH * lights[i]->r;
          result->g += data->object->getSpecular()*data->object->getG()
          * nDotH * lights[i]->g;
          result->b += data->object->getSpecular()*data->object->getB()
          * nDotH * lights[i]->b;
          */
         // Phong.
         float lDotN = l.dot(n);
         lDotN = max(lDotN, 0.0f);
         lDotN = min(lDotN, 1.0f);
         vec3_t r = n * (2 * lDotN) - l;
         r.normalize();
         vec3_t v = view;
         v.normalize();
         float rDotV = r.dot(v);
         pow(rDotV, 1.0f / data->object->getRoughness());
         rDotV = max(rDotV, 0.0f);
         rDotV = min(rDotV, 1.0f);
         result->r += data->object->getSpecular()*data->object->getR()
            * rDotV * lights[i]->r;
         result->g += data->object->getSpecular()*data->object->getG()
            * rDotV * lights[i]->g;
         result->b += data->object->getSpecular()*data->object->getB()
            * rDotV * lights[i]->b;
         /*
         */

         result->r = max(result->r, 0.0f);
         result->g = max(result->g, 0.0f);
         result->b = max(result->b, 0.0f);
         result->r = min(result->r, 1.0f);
         result->g = min(result->g, 1.0f);
         result->b = min(result->b, 1.0f);
      }
   }
   return result;
}

//HitData* Scene::getIntersect(Ray ray)
Pixel* Scene::getIntersect(Ray ray, int depth)
{
   Pixel *result = new Pixel();
   HitData *data = new HitData();
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
   for (int i = 0; i < triangles_size; i++)
   {
      Triangle *curObject = triangles[i];
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
      data->point += ray.point;
      data->t = curDepth;
      data->object = hitObject;

      //result = seekLight(data, ray.dir);
      result = seekLight(data, ray.dir + ray.point);
      if (depth > 0)
      {
         result->multiply(1.0f - data->object->getReflect());
         vec3_t n = data->object->getNormal(data->point);
         n.normalize();
         //vec3_t l = lights[0]->location - data->point;
         vec3_t l = ray.dir;
         l *= -1;
         l.normalize();
         float lDotN = l.dot(n);
         lDotN = max(lDotN, 0.0f);
         lDotN = min(lDotN, 1.0f);
         vec3_t r = n * (2 * lDotN) - l;
         Ray *reflectRay = new Ray();
         //reflectRay.dir = data->object->getNormal(data->point);
         reflectRay->dir = r;
         reflectRay->point = data->point;
         vec3_t rayOffset = reflectRay->dir * 0.001f;
         reflectRay->point += rayOffset;
         Pixel *reflectPix = getIntersect(*reflectRay, depth - 1);
         reflectPix->multiply(data->object->getReflect());
         result->add(reflectPix);
         //result->multiply(1.0f / (float)depth);
         delete reflectRay;
      }
   }
   //return data;
   return result;
}
