/**
 * Chris Brenton
 * Camera Class
 * 4/11/11
 */

#include <stdio.h>
#include "Camera.h"

Camera::Camera(std::istream& input) {
   std::string line;
   // Skip the first blank line
   getline(input, line);
   getline(input, line);
   sscanf(line.c_str(), " location <%f, %f, %f>",
         &location.v[0], &location.v[1], &location.v[2]);
   getline(input, line);
   sscanf(line.c_str(), " up <%f, %f, %f>",
         &up.v[0], &up.v[1], &up.v[2]);
   getline(input, line);
   sscanf(line.c_str(), " right <%f, %f, %f>",
         &right.v[0], &right.v[1], &right.v[2]);
   getline(input, line);
   sscanf(line.c_str(), " look_at <%f, %f, %f>",
         &look_at.v[0], &look_at.v[1], &look_at.v[2]);
   // Debug.
   //printf("Camera: <%f, %f, %f>\n", location.x, location.y, location.z);
}
