// cs174, assignment 1 sample file (RIGHT HANDED)

camera {
   location  <0, 0, 7>
   up        <0,  1,  0>
   right     <1.33333, 0,  0>
   look_at   <0, 0, 0>
}

light_source {<-100, 100, 100> color rgb <1.5, 1.5, 1.5>}

mesh {
   face { <-.001,1 ,0 >, <-.001, -.3,3 >, <1,-.3 ,0 > }

   face { <0,1 ,0 >, <-1, -.3,0 >, <0,-.3 ,3 > }

   face { <-1, -.3,0 >, <0,-1.3 ,0 >, <0,-.3 ,3 > }

   face { <0,-.3 ,2 >, <0,-1.3 ,0 >, <1, -.3,0 > }
}


plane {<0, 1, 0>, -4
   pigment {color rgb <0.2, 0.2, 0.8>}
   finish {ambient 0.3 diffuse 0.8}
}

