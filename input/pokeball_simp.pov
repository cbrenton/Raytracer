// cs174, assignment 1 sample file (RIGHT HANDED)

camera {
   location  <0, 0, 14>
   up        <0,  1,  0>
   right     <1.33333, 0,  0>
   look_at   <0, 0, 0>
}

//light_source {<50, 100, 100> color rgb <1.5, 1.5, 1.5>}
light_source {<100, 100, 100> color rgb <1.5, 1.5, 1.5>}
//light_source {<0, 100, 0> color rgb <1.5, 1.5, 1.5>}

semi { <0, -0.10, 3>, 1.55, 0
   pigment { color rgb <0.0, 1.0, 1.0>}
   finish {ambient 0.2 diffuse 0.9}
   translate <0, 0, 0>
}
