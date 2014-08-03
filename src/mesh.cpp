#include "mesh.h"

Mesh::Mesh(){
  vertex_count = 0;
  face_count = 0;
  vertexBuffer = NULL;
  indexBuffer = NULL;
}

void Mesh::writeSTL(char* out_file)
{
  ofstream stream(out_file);

  stream << "solid mesh" << endl;

  for (int i = 0; i < face_count; ++i)
  {
    stream << "facet normal 0 0 0" << endl;
    stream << "outer loop" << endl;
      for (int j = 0; j < 3; ++j)
        stream << "vertex " << vertexBuffer[indexBuffer[3*i+j]+1](0) << " " <<
             vertexBuffer[indexBuffer[3*i+j]+1](1) << " "<< vertexBuffer[indexBuffer[3*i+j]+1](2) << endl;
    stream << "end loop" << endl;
    stream << "end facet" << endl;
  }

  stream << "endsolid mesh" << endl;
  stream.close();
}

void Mesh::write(char* out_file){
  ofstream stream(out_file);

  for (int i = 0; i < vertex_count; ++i)
  {
    stream << "v " << vertexBuffer[i](0) << " " << vertexBuffer[i](1) << " "<< vertexBuffer[i](2) << endl;
  }

  for (int i = 0; i < face_count; ++i)
    stream << "f " << indexBuffer[3*i] << " " << indexBuffer[3*i+1] << " "<< indexBuffer[3*i+2] << endl;

  stream.close();
}
Mesh::~Mesh(){
  if (vertexBuffer){
    delete[] vertexBuffer;
    vertexBuffer = NULL;
  }
  if (indexBuffer){
    delete[] indexBuffer;
    indexBuffer = NULL;
  }
}
