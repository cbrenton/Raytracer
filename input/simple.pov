// cs174, assignment 1 sample file (RIGHT HANDED)

camera {
   location  <0, 0, 14>
   up        <0,  1,  0>
   right     <1.33333, 0,  0>
   look_at   <0, 0, 0>
}


light_source {<-100, 100, 100> color rgb <1.5, 1.5, 1.5>}
//light_source {<100, 0, 0> color rgb <1.5, 1.5, 1.5>}

   //finish {ambient 0.8 diffuse 0.6 reflection 0.4 refraction 0.4 ior 1.333}
//box { <3, 2, 2>, <4, 7, 7>
sphere {<-5, 5, 5>, 3
   pigment { color rgb <1.0, 1.0, 1.0>}
   finish {ambient 0.4 diffuse 0.8}
}

sphere { <5, -2, -4>, 2
   pigment { color rgb <1.0, 0.0, 1.0>}
   finish {ambient 0.2 diffuse 0.4 reflection 0.2 refraction 1.0 ior 1.333}
   translate <0, 0, 0>
}

//Short box
//box { <-0.5, -0.5, -0.5>, <0.5, 0.5, 0.5>
//finish {ambient 0.2 diffuse 0.4 reflection 0.2}
//box { <-2, -2, 2>, <-3, -3, 3>
box { <3.5, -0.5, -0.5>, <5.5, 0.5, 0.5>
   pigment { color rgb <0.0, 1.0, 0.0>}
   finish {ambient 0.2 diffuse 0.4 reflection 0.2 refraction 1.0 ior 1.333}
}

//plane {<0, 1, 0>, -4
   //pigment {color rgb <0.2, 0.2, 0.8>}
   //finish {ambient 0.4 diffuse 0.8}
//}
