camera {
   location  <0, 0, 4>
   up        <0, 1, 0>
   right     <1.33333, 0,  0>
   look_at   <0, 0, 0>
}

light_source {<-100, 100, 100> color rgb <1.5, 1.5, 1.5>}
light_source {<0, 100, 100> color rgb <0.5, 0.5, 0.5>}

mesh {
   vertex_vectors {
      8,
      <-1,-1,0>,  // 0: Left bottom back
      <-1,1,0>,   // 1: Left top back
      <1,-1,0>,   // 2: Right bottom back
      //<1,1,0>,    // 2: Right bottom back
      <-1,-1,-1>, // 3: Left bottom back
      <-1,-1,0>,  // 4: Left bottom front
      <-1,1,0>,   // 5: Left top front
   }
   face_indices {
      3,
      <0,1,2>//, <0,3,2>,
      //<0,3,1>,
      //<3,4,5>
   }
   pigment {color rgb <0.2, 0.2, 0.8>}
   finish {ambient 0.2 diffuse 0.5 specular 0.4}
}

plane {<0, 1, 0>, -2
   pigment {color rgb <1.0, 1.0, 0.9412>}
   finish {ambient 0.15 diffuse 0.443 specular 0.63 reflection 0.6}
}
