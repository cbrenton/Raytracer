// cs174, assignment 1 sample file (RIGHT HANDED)

camera {
   location  <0, 0, 14>
   up        <0,  1,  0>
   right     <1.33333, 0,  0>
   look_at   <0, 0, 0>
}


light_source {<-100, 100, 100> color rgb <1.5, 1.5, 1.5>}

sphere { <10, 0, 0>, 2
   pigment { color rgb <1.0, 0.0, 1.0>}
   finish {ambient 0.2 diffuse 0.4}
   translate <0, 0, 0>
}

plane {<0, 1, 0>, -4
   pigment {color rgb <0.2, 0.2, 0.8>}
   finish {ambient 0.4 diffuse 0.8}
}

cone { <0, 0, 1>, 2, <0, 0, -3>, 0
   pigment {color rgb <0.5, 0.5, 0.8>}
   finish {ambient 0.2 diffuse 0.2}
   rotate <0, -70, 0>
   translate <1, 0.51, 0>
}

// Triangles
triangle {
   <-0.57735, 0.18759, 0.7946499999999999>,
   <-0.35682, -0.49112, 0.7946499999999999>,
   <0.35682, -0.49112, 0.7946499999999999>
   scale <2, 2, 2>
   rotate <-20, 0, 0>
   rotate <0, 20, 0>
   pigment {color rgb <0.9, 0.9, 0.0>}
   finish {ambient 0.4 diffuse 0.1}
}
