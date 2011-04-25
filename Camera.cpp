/**
 * Chris Brenton
 * Camera Class
 * 4/11/11
 */

#include "Camera.h"

Camera::Camera(std::istream& input) {
   std::string line;
   // Skip the first blank line
   getline(input, line);
   getline(input, line);
   sscanf(line.c_str(), " location <%f, %f, %f>",
         &location.x, &location.y, &location.z);
   getline(input, line);
   sscanf(line.c_str(), " up <%f, %f, %f>",
         &up.x, &up.y, &up.z);
   getline(input, line);
   sscanf(line.c_str(), " right <%f, %f, %f>",
         &right.x, &right.y, &right.z);
   getline(input, line);
   sscanf(line.c_str(), " look_at <%f, %f, %f>",
         &look_at.x, &look_at.y, &look_at.z);
   // Debug.
   //printf("Camera: <%f, %f, %f>\n", location.x, location.y, location.z);
}
