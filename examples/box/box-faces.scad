face_1_points = [
  [0,10,0],[0,0,0],[0,0,10], // Triangle 1 / 2
  [0,10,10],[0,10,0],[0,0,10],
];
face_1_faces = [
  [0,1,2], // Triangle 1 / 2
  [3,4,5],
];

face_2_points = [
  [10,0,10],[10,0,0],[10,10,0], // Triangle 1 / 2
  [10,0,10],[10,10,0],[10,10,10],
];
face_2_faces = [
  [0,1,2], // Triangle 1 / 2
  [3,4,5],
];

face_3_points = [
  [0,0,0],[10,0,0],[10,0,10], // Triangle 1 / 2
  [0,0,10],[0,0,0],[10,0,10],
];
face_3_faces = [
  [0,1,2], // Triangle 1 / 2
  [3,4,5],
];

face_4_points = [
  [10,10,10],[10,10,0],[0,10,0], // Triangle 1 / 2
  [10,10,10],[0,10,0],[0,10,10],
];
face_4_faces = [
  [0,1,2], // Triangle 1 / 2
  [3,4,5],
];

face_5_points = [
  [0,0,0],[0,10,0],[10,10,0], // Triangle 1 / 2
  [10,0,0],[0,0,0],[10,10,0],
];
face_5_faces = [
  [0,1,2], // Triangle 1 / 2
  [3,4,5],
];

face_6_points = [
  [10,10,10],[0,10,10],[0,0,10], // Triangle 1 / 2
  [10,10,10],[0,0,10],[10,0,10],
];
face_6_faces = [
  [0,1,2], // Triangle 1 / 2
  [3,4,5],
];

module crazy_colors() {
color("Violet")
polyhedron(face_1_points, face_1_faces);
color("red")
polyhedron(face_2_points, face_2_faces);
color("blue")
polyhedron(face_3_points, face_3_faces);
color("LawnGreen")
polyhedron(face_4_points, face_4_faces);
color("Orange")
polyhedron(face_5_points, face_5_faces);
color("DeepPink")
polyhedron(face_6_points, face_6_faces);
}
function add_offset(vec,ofs) = [for (x=vec) x + [ofs,ofs,ofs]];
module solid_object() {
  tmp1_points = face_1_points;
  tmp1_faces =  face_1_faces;

  tmp2_points = concat(tmp1_points, face_2_points);
  tmp2_faces =  concat(tmp1_faces,add_offset(face_2_faces,len(tmp1_points)));

  tmp3_points = concat(tmp2_points, face_3_points);
  tmp3_faces =  concat(tmp2_faces,add_offset(face_3_faces,len(tmp2_points)));

  tmp4_points = concat(tmp3_points, face_4_points);
  tmp4_faces =  concat(tmp3_faces,add_offset(face_4_faces,len(tmp3_points)));

  tmp5_points = concat(tmp4_points, face_5_points);
  tmp5_faces =  concat(tmp4_faces,add_offset(face_5_faces,len(tmp4_points)));

  tmp6_points = concat(tmp5_points, face_6_points);
  tmp6_faces =  concat(tmp5_faces,add_offset(face_6_faces,len(tmp5_points)));

  polyhedron (tmp6_points, tmp6_faces);
}


if ($preview) {;
  crazy_colors();
} else {
  solid_object();
}
