// cs174, assignment 1 sample file (RIGHT HANDED)

camera {
  location  <0, 0, 14>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}


light_source {<-100, 100, 100> color rgb <1.5, 1.5, 1.5>}

sphere { <0, 0, 0>, 3
  pigment { color rgb <1.0, 0.0, 0.0>}
  finish {ambient 0.2 diffuse 0.4 specular 0.5 reflection 0.8 refraction 1.0 ior 1.33}
  translate <0, 0, 0>
}

sphere { <-3, 3, -.5>, 2
  pigment { color rgb <0.0, 1.0, 0.0>}
  finish {ambient 0.2 diffuse 0.4 specular 0.5 reflection 0.8 refraction 1.0 ior 1.33}
  translate <0, 0, 0>
}

sphere { <3, 3, -.5>, 2
  pigment { color rgb <0.0, 0.0, 1.0>}
  finish {ambient 0.2 diffuse 0.4 specular 0.5 reflection 0.8 refraction 1.0 ior 1.33}
  translate <0, 0, 0>
}

plane {<0, 1, 0>, -3.5
      pigment {color rgb <0.1, 0.1, 0.1>}
      finish {ambient 0.4 diffuse 0.8 reflection 0.8}
}
