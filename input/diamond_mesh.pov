camera {
   location  <0, 0, 4>
   up        <0, 1, 0>
   right     <1.33333, 0,  0>
   look_at   <0, 0, 0>
}

light_source {<-100, 100, 100> color rgb <1.5, 1.5, 1.5>}
light_source {<0, 100, 100> color rgb <0.5, 0.5, 0.5>}

mesh { 5
   vertex_vectors {
      6,
      <0,0,1>,    // 0: Origin
      <0,1,0>,    // 1: Top
      <0,-1,0>,   // 2: Bottom
      <-1,0,0>,   // 3: Left
      <1,0,0>,    // 4: Right
      <0,0,-1>    // 5: Back origin
   }
   face_indices {
      8,
      <1,0,4>,    // Top right
      <4,0,2>,    // Bottom right
      <2,0,3>,    // Bottom left
      <3,0,1>,    // Top left
      <1,5,4>,    // Top back right
      <4,5,2>,    // Bottom back right
      <2,5,3>,    // Bottom back left
      <3,5,1>,    // Top back left
   }
   pigment {color rgb <0.2, 0.2, 0.8>}
   finish {ambient 0.2 diffuse 0.5 specular 0.4}
}


plane {<0, 1, 0>, -2
   pigment {color rgb <1.0, 1.0, 0.9412>}
   finish {ambient 0.15 diffuse 0.443 specular 0.63 reflection 0.6}
}
