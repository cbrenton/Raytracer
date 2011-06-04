// cs174, assignment 1 sample file (RIGHT HANDED)

camera {
   location  <0, 2, 11>
      up        <0,  1,  0>
      right     <1.33333, 0,  0>
      look_at   <0, 0, 0>
}


//light_source {<-25, 15, 80> color rgb <1, 0, 0>}

//light_source {<25, 15, 80> color rgb <0, 1, 0>}

light_source {<0, 500, 0> color rgb <.5, .5, .5>}

sphere { <2.5, 1, -2>, 1
   pigment { color rgb <.35, 0.46, 0.69, .2>}
   finish {ambient 0.2 diffuse 0.86 specular 0.6 roughness .2 reflection 0.4}
   translate <0, 0, 0>
}

sphere { <2.7, 1, -5>, 1
   pigment { color rgb <.45, 0.5, 0.39, .2>}
   finish {ambient 0.2 diffuse 0.86 specular 0.6 roughness .2 reflection 0.4}
   translate <0, 0, 0>
}

sphere { <0, 1, -8>, 1
   pigment { color rgb <.85, 0.5, 0.64, .2>}
   finish {ambient 0.2 diffuse 0.86 specular 0.6 roughness .2 reflection 0.4}
   translate <0, 0, 0>
}

sphere { <-2.5, 1, -2>, 1
   pigment { color rgb <.45, 0.3, 0.2, .2>}
   finish {ambient 0.2 diffuse 0.86 specular 0.6 roughness .2 reflection 0.4}
   translate <0, 0, 0>
}

sphere { <-2.7, 1, -5>, 1
   pigment { color rgb <.485, 0.34, 0.69, .2>}
   finish {ambient 0.2 diffuse 0.86 specular 0.6 roughness .2 reflection 0.4}
   translate <0, 0, 0>
}

plane {<0, 1, 0>, 0
   pigment {color rgb <0.2, 0.2, 0.8, .2>}
   finish { ambient 0.4 diffuse 0.9 specular 0.4 roughness .2 reflection 0.1}
}
