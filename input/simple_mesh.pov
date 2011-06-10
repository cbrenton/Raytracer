camera {
   location  <0, 8, 0>
   up        <0,  0,  1>
   right     <1.33333, 0,  0>
   look_at   <0, 0, 0>
}

light_source {<-100, 100, 100> color rgb <1.5, 1.5, 1.5>}

mesh {
   vertex_vectors {
      15,
      <1,1,1>, <1,-1,1>,      // front left
      <0,0,1>,                // front center
      <-1,1,1>, <-1,-1,1>,    // front right
      <1,1,-1>, <1,-1,-1>,      // left back
      <1,0,0>,                // left center
      <1,1,1>, <1,-1,1>                // left front
      <1,1,-1>, <1,-1,-1>,    // back left
      <0,0,-1>,               // back center
      <-1,1,-1>, <-1,-1,-1>,  // back right
   }
   face_indices {
      12,
      <0,1,2>, <2,3,4>, // front
      <0,2,3>, <1,2,4>  // front
      <5,6,7>, <7,8,9>
      <5,7,8>, <6,7,9>
      <10,11,12>, <12,13,14>,
      <10,12,13>, <11,12,14>
   }
   pigment {color rgb <0.2, 0.2, 0.8>}
   finish {ambient 0.2 diffuse 0.5 specular 0.4}
}

plane {<0, 1, 0>, -2
   pigment {color rgb <1.0, 1.0, 0.9412>}
   finish {ambient 0.15 diffuse 0.443 specular 0.63 reflection 0.6}
}
