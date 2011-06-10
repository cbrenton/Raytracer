/**
 * Sterling Hirsh
 * Scene Class
 * This has everything a scene needs.
 * 4/3/11
 */

#include "Scene.h"
#include <string>
#include <ctype.h>

#define EPSILON 0.001f
#define USE_PHONG 1
#define IOR_AIR 1.00030942f

using namespace std;

Scene::~Scene()
{
   for (unsigned i = 0; i < geometry_vec.size(); i++)
   {
      delete geometry_vec[i];
   }
   for (unsigned j = 0; j < planes_vec.size(); j++)
   {
      delete planes_vec[j];
   }
}

Scene* Scene::read(istream& input)
{
   vector<Light*> lights_vec;
   //vector<Geometry*> geometry_vec;
   //vector<Plane*> planes_vec;
   //vector<Sphere*> spheres_vec;
   //vector<Triangle*> triangles_vec;

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
               curScene->geometry_vec.push_back(new Box(input));
            }
            else if (curItemName.compare("cone") == 0)
            {
               curScene->geometry_vec.push_back(new Cone(input));
            }
            else if (curItemName.compare("plane") == 0)
            {
               curScene->planes_vec.push_back(new Plane(input));
            }
            else if (curItemName.compare("sphere") == 0)
            {
               curScene->geometry_vec.push_back(new Sphere(input));
            }
            else if (curItemName.compare("semi") == 0)
            {
               curScene->geometry_vec.push_back(new Semi(input));
            }
            else if (curItemName.compare("triangle") == 0)
            {
               curScene->geometry_vec.push_back(new Triangle(input));
            }
            else if (curItemName.compare("mesh") == 0)
            {
               curScene->geometry_vec.push_back(new Mesh(input));
            }

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
   /*
   //curScene->geometry = new Geometry *[curScene->geometry_size];
   curScene->geometry = new Geometry *[curScene->geometry_vec.size()];
   copy(curScene->geometry_vec.begin(), curScene->geometry_vec.end(), curScene->geometry);
   curScene->planes = new Plane *[curScene->planes_size];
   copy(planes_vec.begin(), planes_vec.end(), curScene->planes);
   curScene->spheres = new Sphere *[curScene->spheres_size];
   copy(spheres_vec.begin(), spheres_vec.end(), curScene->spheres);
   curScene->triangles = new Triangle *[curScene->triangles_size];
   copy(triangles_vec.begin(), triangles_vec.end(), curScene->triangles);
   */

   return curScene;
}

void Scene::constructBVH()
{
   sceneBVH = new bvh_node(geometry_vec, 0);
   hasBVH = true;
}

Pixel Scene::seekLight(HitData *data, vec3_t view)
{
   Pixel result = Pixel(0.0, 0.0, 0.0, 1.0);
   for (int i = 0; i < lights_size; i++)
   {
      Light light = *lights[i];
      vec3_t dir = light.location - data->point;
      float dirLen = dir.length();
      dir.normalize();
      Ray feeler;

      vec3_t offset = dir * EPSILON;
      feeler.point = data->point + offset;
      feeler.dir = dir;
      bool isShadow = false;

      HitData tmp;
      isShadow = hit(feeler, &tmp);

      if ((dynamic_cast<Mesh*> (data->object)) != NULL)
      {
         cout << "mesh" << endl;
      }

      // Ambient.
      result.r += (data->object->getAmbient()*data->object->getR())
         * light.r;
      result.g += (data->object->getAmbient()*data->object->getG())
         * light.g;
      result.b += (data->object->getAmbient()*data->object->getB())
         * light.b;
      if (!isShadow && data->t <= dirLen)
      {
         // Diffuse.
         vec3_t n = data->object->getNormal(data->point);
         n.normalize();
         vec3_t l = light.location - data->point;
         l.normalize();
         float nDotL = n.dot(l);
         nDotL = min(nDotL, 1.0f);
         if (nDotL > 0)
         {
            result.r += data->object->getDiffuse()*data->object->getR()
               * nDotL * light.r;
            result.g += data->object->getDiffuse()*data->object->getG()
               * nDotL * light.g;
            result.b += data->object->getDiffuse()*data->object->getB()
               * nDotL * light.b;
         }

         // Specular.
         //if (data->object->getReflect() == 0 && !data->object->getRefract())
         // Phong.
         if (USE_PHONG)
         {
            vec3_t r = reflect(l, n);
            r.normalize();
            vec3_t v = view;
            v.normalize();
            float rDotV = r.dot(v);
            rDotV = (float)pow(rDotV, 1.0f / data->object->getRoughness());
            rDotV = min(rDotV, 1.0f);
            if (rDotV > 0)
            {
               result.r += data->object->getSpecular()*data->object->getR()
                  * rDotV * light.r;
               result.g += data->object->getSpecular()*data->object->getG()
                  * rDotV * light.g;
               result.b += data->object->getSpecular()*data->object->getB()
                  * rDotV * light.b;
            }
         }
         // Blinn.
         else
         {
            vec3_t v = view;
            v.normalize();
            vec3_t h = l + v;
            h.normalize();
            float nDotH = n.dot(h);
            nDotH = (float)pow(nDotH, data->object->getRoughness());
            nDotH = min(nDotH, 1.0f);
            if (nDotH > 0)
            {
               result.r += data->object->getSpecular()*data->object->getR()
                  * nDotH * light.r;
               result.g += data->object->getSpecular()*data->object->getG()
                  * nDotH * light.g;
               result.b += data->object->getSpecular()*data->object->getB()
                  * nDotH * light.b;
            }
         }
      }
      result.clamp();
   }
   return result;
}

bool Scene::hit(Ray ray, HitData *data)
{
   int hitObject = -1;
   float t = -1;
   bool hitFound = false;
   float curDepth = -1.0;
   HitData *closestData = new HitData();
   if (hasBVH)
   {
      bool bvhHit = sceneBVH->hit(ray, &t, data);
      for (unsigned i = 0; i < planes_vec.size(); i++)
      {
         Plane *curObject = planes_vec[i];
         float planeT = -1.0;
         HitData tmpData;
         bool intersected = false;
         intersected = curObject->hit(ray, &planeT, &tmpData);
         if (intersected)
         {
            if (planeT >= 0 && (!hitFound || (hitFound && planeT < curDepth)))
            {
               curDepth = planeT;
               hitObject = i;
               *closestData = tmpData;
            }
         }
         hitFound |= (intersected && curDepth > 0.0);
      }
      if (hitFound && (curDepth < t || !bvhHit))
      {
         *data = *closestData;
         t = curDepth;
      }
      delete closestData;
      return (bvhHit || hitFound);
   }
   else
   {
      bool geomHit = false;
      bool planeHit = false;
      float geomT = -1.0;
      float planeT = -1.0;

      for (unsigned i = 0; i < geometry_vec.size(); i++)
      {
         Geometry *curObject = geometry_vec[i];
         geomT = -1.0;
         HitData tmpData;
         bool intersected = false;
         intersected = curObject->hit(ray, &geomT, &tmpData);
         if (intersected)
         {
            if (geomT >= 0 && (!hitFound || (hitFound && geomT < curDepth)))
            {
               curDepth = geomT;
               hitObject = i;
               *closestData = tmpData;
            }
         }
         geomHit |= (intersected && curDepth > 0.0);
      }
      if (geomHit && (curDepth < t || t < 0))
      {
         *data = *closestData;
         t = curDepth;
      }
      for (unsigned i = 0; i < planes_vec.size(); i++)
      {
         Plane *curObject = planes_vec[i];
         planeT = -1.0;
         HitData tmpData;
         bool intersected = false;
         intersected = curObject->hit(ray, &planeT, &tmpData);
         if (intersected)
         {
            if (planeT >= 0 && (!hitFound || (hitFound && planeT < curDepth)))
            {
               curDepth = planeT;
               hitObject = i;
               *closestData = tmpData;
            }
         }
         planeHit |= (intersected && curDepth > 0.0);
      }
      // If the ray intersects a plane at a point closer than any geometry
      // intersections, return plane intersection data.
      if (planeHit && (curDepth < t || t < 0))
      {
         *data = *closestData;
         t = curDepth;
         delete closestData;
         return true;
      }
      // Else if geometry was intersected, return geometry intersection data.
      else
      {
         delete closestData;
         return geomHit;
      }
   }
}

Pixel Scene::getIntersect(Ray ray, int depth)
{
   ray.dir.normalize();
   Pixel result = Pixel();
   bool success = false;
   HitData data;
   success = hit(ray, &data);
   if (!success)
   {
      result.setClear();
      return result;
   }
   result = seekLight(&data, ray.dir);
   float alpha = 0;
   vec3_t n = data.object->getNormal(data.point);
   n.normalize();
   vec3_t r;
   vec3_t t;
   vec3_t offset = n * EPSILON;
   vec3_t point = data.point;
   if (ray.dir.dot(n) < 0)
   {
      point -= offset;
   }
   else
   {
      point += offset;
   }
   if (depth > 0)
   {
      Pixel reflectPix = Pixel();
      Pixel refractPix = Pixel();
      // Reflection.
      if (data.object->getReflect() > 0)
      {
         Ray reflectRay;
         // If entering an object.
         if (ray.dir.dot(n) < 0)
         {
            r = reflect(ray.dir, n);
         }
         else
         {
            r = reflect(ray.dir, n * -1);
         }
         r.normalize();
         reflectRay.dir = r;
         vec3_t rayOffset = reflectRay.dir * EPSILON;
         reflectRay.point = data.point + rayOffset;
         reflectPix = getIntersect(reflectRay, depth - 1);
         reflectPix.multiply(data.object->getReflect());
      }
      // Refraction.
      if (data.object->getRefract())
      {
         alpha = data.object->getF();
         bool shouldRefract = false;
         Ray refractRay;
         // If entering an object.
         if (ray.dir.dot(n) < 0)
         {
            t = refract(ray.dir, n, IOR_AIR, data.object->getIOR(),
                  &shouldRefract);
            t.normalize();
            vec3_t rayOffset = n * -EPSILON;
            refractRay.point = data.point + rayOffset;
            refractRay.dir = t;
         }
         // If exiting an object.
         else
         {
            t = refract(ray.dir, n * -1, data.object->getIOR(),
                  IOR_AIR, &shouldRefract);
            t.normalize();
            vec3_t rayOffset = n * EPSILON;
            refractRay.point = data.point + rayOffset;
            refractRay.dir = t;
         }
         if (shouldRefract)
         {
            if (ray.dir.dot(n) < 0)
            {
               //refractPix = getIntersect(refractRay, depth);
               refractPix = getIntersect(refractRay, depth - 1);
            }
            else
            {
               refractPix = getIntersect(refractRay, depth - 1);
            }
         }
         else
         {
            alpha = 0;
         }
      }
      result.multiply(1.0f - data.object->getReflect() - alpha);
      result.a = 1.0f;
      result.add(reflectPix);
      refractPix.multiply(alpha);
      result.add(refractPix);
   }
   return result;
}

vec3_t Scene::reflect(vec3_t d, vec3_t n)
{
   float cosD = -d.dot(n);
   vec3_t r = n * (2 * cosD);
   r += d;
   return r;
}

vec3_t Scene::refract(vec3_t d, vec3_t n, float n1, float n2, bool *success)
{
   vec3_t t;
   float iorRatio = n1 / n2;
   float cosD = -d.dot(n);
   float sin2T = (iorRatio*iorRatio) * (1.0f - (cosD*cosD));
   float root = 1.0f - sin2T;
   if (root < 0.0f)
   {
      *success = false;
      return t;
   }
   t = d * iorRatio;
   vec3_t t1 = n * (iorRatio * cosD - (float)sqrt(root));
   t += t1;
   *success = true;
   return t;
}
