/**
 * Sterling Hirsh
 * Light Class
 * 4/3/11
 */

#include "Light.h"
#include <string>

Light::Light(std::istream& input) {
   // Get the rest of the line.
   // sscanf to get specific values.
   std::string line;
   getline(input, line);
   sscanf(line.c_str(), "{<%f, %f, %f> color rgb <%f, %f, %f>}",
         &location.v[0], &location.v[1], &location.v[2],
         &r, &g, &b);
   //location.print();
   // Debug
   //printf("Light: (r,g,b) (%f, %f, %f)\n", r, g, b);
}

