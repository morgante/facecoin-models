// reading a text file
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "mesh.h"

using namespace std;


int main (int argc, char *argv[]) {

  Mesh out_mesh;

  picture front, back;
  unsigned error = lodepng::decode(front.image, front.width, front.height, argv[1]);
   error = lodepng::decode(back.image, back.width, back.height, argv[2]);

  if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

  Mesh::makeCoin(front, back, &out_mesh);

  out_mesh.write(argv[3]);

  return 0;
} 