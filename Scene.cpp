/**
 * Sterling Hirsh
 * Scene Class
 * This has everything a scene needs.
 * 4/3/11
 */

#include <string>
#include <ctype.h>
#include "Scene.h"
#include "Geometry.h"

using namespace std;

Scene* Scene::read(std::istream& input)
{
   std::vector<light_t> lights_vec;
   //std::vector<geom_t*> geometry_vec;
   std::vector<plane_t> planes_vec;
   std::vector<sphere_t> spheres_vec;

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
               makeCamera(curScene->camera, input);
            }
            else if (curItemName.compare("light_source") == 0)
            {
               //light_t tmp;
               //= light_t(input);
               //(input);
               //makeLight(tmp, input);
               //lights_vec.push_back(tmp);
               lights_vec.push_back(light_t(input));
               curScene->lights_size++;
            }
            /*
               else if (curItemName.compare("box") == 0)
               {
               box_t tmp;
               makeBox(tmp, input);
               geometry_vec.push_back(tmp);
               curScene->geometry_size++;
               }
               else if (curItemName.compare("cone") == 0)
               {
               cone_t tmp;
               makeCone(tmp, input);
               geometry_vec.push_back(tmp);
               curScene->geometry_size++;
               }
               */
            else if (curItemName.compare("plane") == 0)
            {
               plane_t tmp = plane_t(input);
               planes_vec.push_back(tmp);
               curScene->planes_size++;
            }
            else if (curItemName.compare("sphere") == 0)
            {
               sphere_t *tmp = new sphere_t();
               //tmp.makeSphere(input);
               makeSphere(tmp, input);
               printf("sphere\n");
               spheres_vec.push_back(*tmp);
               curScene->spheres_size++;
            }
            /*
               else if (curItemName.compare("semi") == 0)
               {
               semi_t tmp;
               makeSemi(tmp, input);;
               geometry_vec.push_back(tmp);
               curScene->geometry_size++;
               }
               else if (curItemName.compare("triangle") == 0)
               {
               tri_t tmp;
               makeTriangle(tmp, input);
               geometry_vec.push_back(tmp);
               curScene->geometry_size++;
               }
               */
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

   curScene->lights = new light_t [curScene->lights_size];
   copy(lights_vec.begin(), lights_vec.end(), curScene->lights);
   //curScene->geometry = new geom_t *[curScene->geometry_size];
   //copy(geometry_vec.begin(), geometry_vec.end(), curScene->geometry);
   curScene->planes = new plane_t [curScene->planes_size];
   copy(planes_vec.begin(), planes_vec.end(), curScene->planes);
   curScene->spheres = new sphere_t [curScene->spheres_size];
   copy(spheres_vec.begin(), spheres_vec.end(), curScene->spheres);
   return curScene;
}

pixel_t *Scene::seekLight(HitData *data, vec3_t view)
{
   pixel_t *result = new pixel_t();
   /*
      for (int i = 0; i < lights_size; i++)
      {
      vec3_t dir = lights[i].location - data->point;
      float dirLen = dir.length();
      dir.normalize();
      ray_t *feeler = new ray_t(dir * 0.001f + data->point, dir);
      float t = -1.0;
      bool hit = false;
      for (int j = 0; j < geometry_size; j++)
      {
      bool intersect = geometry[j].hit(*feeler, &t);
      hit |= (intersect && t > 0 && t <= dirLen);
      }
      for (int j = 0; j < planes_size; j++)
      {
      float t = planes[j].hit(*feeler);
      hit |= (t > 0 && t <= dirLen);
      }
      for (int j = 0; j < spheres_size; j++)
      {
      float t = spheres[j].hit(*feeler);
      hit |= (t > 0 && t <= dirLen);
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
   //specular
   vec3_t d = data->point - view;
   float dDotN = d.dot(n);
   vec3_t r = n * (2.0f * dDotN);
   r += d;
   float vDotR = view.dot(r);
   pow(vDotR, data->object->getRoughness());
   if (dDotN > 0)
   {
   result->r += data->object->getSpecular()*data->object->getR() * vDotR * lights[i]->r;
   result->g += data->object->getSpecular()*data->object->getG() * vDotR * lights[i]->g;
   result->b += data->object->getSpecular()*data->object->getB() * vDotR * lights[i]->b;
   }
   result->r = std::min(result->r, 1.0f);
   result->g = std::min(result->g, 1.0f);
   result->b = std::min(result->b, 1.0f);
   }
   }
   */
   if (data->hit)
   {
      int i = 0;
      printf("lights r: %f\n", lights[i].r);
      result->r = (data->object->getMat().amb * data->object->getMat().r) * lights[i].r;
      printf("amb: %f\n", data->object->getMat().amb);
      printf("object: %d\n", data->object == NULL);
      printf("mat r: %f\n", data->object->getMat().r);
      printf("mat g: %f\n", data->object->getMat().g);
      printf("mat b: %f\n", data->object->getMat().b);
      printf("result r: %f\n", result->r);
      result->g = (data->object->getMat().amb * data->object->getMat().g) * lights[i].g;
      result->b = (data->object->getMat().amb * data->object->getMat().b) * lights[i].b;
      //result->g = (data->object->getAmbient()*data->object->getG()) * lights[i].g;
      //result->b = (data->object->getAmbient()*data->object->getB()) * lights[i].b;
      result->a = 1.0;
   }
   else
   {
      printf("no hit\n");
   }
   return result;
}

HitData* Scene::getIntersect(ray_t ray)
{
   HitData *data = new HitData();
   geom_t *hitObject = new geom_t();
   float t;
   bool hitFound = false;
   float curDepth = 0.0;
   /*
      for (int i = 0; i < geometry_size; i++)
      {
      geom_t *curObject = geometry[i];
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
      */
   for (int i = 0; i < planes_size; i++)
   {
      plane_t *curObject = &planes[i];
      geom_t tmp;
      tmp.type = PLANE_TYPE;
      tmp.plane = curObject;
      t = hit(ray, tmp);
      if (t > 0)
      {
         if (t >= 0 && (!hitFound || (hitFound && t < curDepth)))
         {
            curDepth = t;
            hitObject = &tmp;
         }
      }
      hitFound |= (t > 0 && curDepth > 0);
   }
   for (int i = 0; i < spheres_size; i++)
   {
      sphere_t *curObject = &spheres[i];
      geom_t tmp;
      tmp.type = SPHERE_TYPE;
      tmp.sphere = curObject;
      t = hit(ray, tmp);
      if (t > 0)
      {
         if (t >= 0 && (!hitFound || (hitFound && t < curDepth)))
         {
            curDepth = t;
            hitObject = &tmp;
         }
      }
      hitFound |= (t > 0 && curDepth > 0);
   }
   if (hitFound)
   {
      data->hit = true;
      data->point = ray.dir * curDepth;
      data->point += ray.point;
      data->object = hitObject;
      printf("mat r: %f\n", data->object->getMat().r);
      printf("mat g: %f\n", data->object->getMat().g);
      printf("mat b: %f\n", data->object->getMat().b);
   }
   else
   {
      data->hit = false;
   }
   return data;
}
