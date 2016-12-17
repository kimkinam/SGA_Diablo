xof 0303txt 0032
template Frame {
 <3d82ab46-62da-11cf-ab39-0020af71e433>
 [...]
}

template Matrix4x4 {
 <f6f23f45-7686-11cf-8f52-0040333594a3>
 array FLOAT matrix[16];
}

template FrameTransformMatrix {
 <f6f23f41-7686-11cf-8f52-0040333594a3>
 Matrix4x4 frameMatrix;
}

template Vector {
 <3d82ab5e-62da-11cf-ab39-0020af71e433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template MeshFace {
 <3d82ab5f-62da-11cf-ab39-0020af71e433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template Mesh {
 <3d82ab44-62da-11cf-ab39-0020af71e433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

template MeshNormals {
 <f6f23f43-7686-11cf-8f52-0040333594a3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template Coords2d {
 <f6f23f44-7686-11cf-8f52-0040333594a3>
 FLOAT u;
 FLOAT v;
}

template MeshTextureCoords {
 <f6f23f40-7686-11cf-8f52-0040333594a3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}


Frame Model {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Mesh {
  9;
  -60.000000;0.000000;60.000000;,
  0.000000;0.000000;60.000000;,
  0.000000;0.000000;0.000000;,
  -60.000000;0.000000;0.000000;,
  60.000000;0.000000;60.000000;,
  60.000000;0.000000;0.000000;,
  0.000000;0.000000;-60.000000;,
  -60.000000;0.000000;-60.000000;,
  60.000000;0.000000;-60.000000;;
  8;
  3;0,1,2;,
  3;0,2,3;,
  3;1,4,5;,
  3;1,5,2;,
  3;3,2,6;,
  3;3,6,7;,
  3;2,5,8;,
  3;2,8,6;;

  MeshNormals {
   9;
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;;
   8;
   3;0,1,2;,
   3;0,2,3;,
   3;1,4,5;,
   3;1,5,2;,
   3;3,2,6;,
   3;3,6,7;,
   3;2,5,8;,
   3;2,8,6;;
  }

  MeshTextureCoords {
   9;
   0.000000;0.000000;,
   0.500000;0.000000;,
   0.500000;0.500000;,
   0.000000;0.500000;,
   1.000000;0.000000;,
   1.000000;0.500000;,
   0.500000;1.000000;,
   0.000000;1.000000;,
   1.000000;1.000000;;
  }
 }
}