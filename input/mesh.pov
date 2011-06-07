   //location  <0, 0, 14>
   //up        <0,  1,  0>
   //location  <0, 14, 0>
   //up        <0, 0, 1>
camera {
   location  <0, 0, 14>
   up        <0,  1,  0>
   right     <1.33333, 0,  0>
   look_at   <0, 0, 0>
}

light_source {<-100, 100, 100> color rgb <1.5, 1.5, 1.5>}

mesh {
   vertex_vectors {
      8,
      <-1,-1,-1>, <-1,1,-1>,
      <1,-1,-1>, <1,1,-1>,
      <-1,-1,1>, <-1,1,1>,
      <1,-1,1>, <1,1,1>
   }
   face_indices {
      12,
      <2,1,0>, <2,3,1>, // back
      <4,5,0>, <0,5,1>, // left
      <7,3,2>, <6,7,2>, // right
      <4,2,0>, <4,6,2>, // bottom
      <5,3,1>, <3,5,7>, // top
      <6,5,4>, <6,7,5>  // front
   }
   pigment {color rgb <0.2, 0.2, 0.8>}
   finish {ambient 0.2 diffuse 0.5 specular 0.4}
}

//sphere {<-4, 3, 2>, 1
sphere {<2, 0, 0>, 0.5
   pigment {color rgb <0.7, 0.6, 0.1>}
   finish {ambient 0.4 diffuse 0.8}
}

plane {<0, 1, 0>, -2
   pigment {color rgb <1.0, 1.0, 0.9412>}
   finish {ambient 0.15 diffuse 0.443 specular 0.63 reflection 0.6}
}
