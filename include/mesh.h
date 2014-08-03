#include <Eigen/Core>
#include <Eigen/Geometry>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <algorithm>
#include "lodepng.h"
#define CONST_PI 3.1415926
#define res 500
#define RANDOM (rand()%100)*0.01

using namespace std;

struct picture{
  std::vector<unsigned char> image; //the raw pixels
  unsigned width, height;
};

class Mesh
{
  Eigen::Vector3d* vertexBuffer;
  int* indexBuffer;

  int vertex_count;
  int face_count;

public:

  Mesh();
  ~Mesh();

  void write(char* out_obj_file);
  void writeSTL(char* out_file);

  static void makeCoin(picture front, picture back, Mesh* out_mesh);


};
