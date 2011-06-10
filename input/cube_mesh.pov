camera {
   location  <0, 0, 4>
   up        <0, 1, 0>
   right     <1.33333, 0,  0>
   look_at   <0, 0, 0>
}

light_source {<-100, 100, 100> color rgb <1.5, 1.5, 1.5>}
light_source {<0, 100, 100> color rgb <0.5, 0.5, 0.5>}

mesh { 1
   vertex_vectors {
      8,
      <-1,-1,-1>, // 0: Left bottom back
      <-1,1,-1>,  // 1: Left top back
      <1,-1,-1>,  // 2: Right bottom back
      <1,1,-1>,   // 3: Right top back
      <-1,-1,1>,  // 4: Left bottom front
      <-1,1,1>,   // 5: Left top front
      <1,-1,1>,   // 6: Right bottom front
      <1,1,1>     // 7: Right top front
   }
   face_indices {
      12,
      <0,1,2>, <1,3,2>, // Back
      <5,4,6>, <6,7,5>, // Front
      <4,0,1>, <1,5,4> // Left
      <7,3,2>, <6,7,2> // Right
      <4,0,2>, <2,6,4> // Bottom
      <3,1,5>, <5,7,3> // Top
   }
   pigment {color rgb <0.2, 0.2, 0.8>}
   finish {ambient 0.2 diffuse 0.5 specular 0.4}
}

//sphere {<-4, 3, 2>, 1
   //pigment {color rgb <0.7, 0.6, 0.1>}
   //finish {ambient 0.4 diffuse 0.8}
//}

//triangle {
   //<-2, -2, -3>, <2, -2, -3>, <2, 2, -3>
   //pigment {color rgb <0.7, 0.6, 0.1>}
   //finish {ambient 0.4 diffuse 0.8}
//}

plane {<0, 1, 0>, -2
   pigment {color rgb <1.0, 1.0, 0.9412>}
   finish {ambient 0.15 diffuse 0.443 specular 0.63 reflection 0.6}
}
