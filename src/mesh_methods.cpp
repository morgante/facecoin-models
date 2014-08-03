#include "mesh.h"
#define RES 500
#define THICKNESS 5.0
#define DIAMETER 25.0
#define MAGNIT(a) (a - DIAMETER/2)*(a - DIAMETER/2)
#define INSIDE(x) ((MAGNIT(x(0)) + MAGNIT(x(1))) < (DIAMETER*4))
#define CENTER(x) (x-Eigen::Vector3d(DIAMETER/2, DIAMETER/2,0))


void build_circle(int i, int j, int k, int* indexBuffer, Eigen::Vector3d* vertexBuffer,
		 int& counter, std::vector<int>& edge_points)
{
	int a_in = INSIDE(vertexBuffer[i]);
	int b_in = INSIDE(vertexBuffer[j]);
	int c_in = INSIDE(vertexBuffer[k]);

	if ((a_in + b_in + c_in) == 3)
	{	
		//top side
		indexBuffer[3*counter] = i;
		indexBuffer[3*counter+1] = j;
		indexBuffer[3*counter+2] = k;
		counter++;
	}
	else if ((a_in + b_in+c_in) > 0 && i%2 == 0)
	{
		if (a_in &&  std::find(edge_points.begin(), edge_points.end(), i)==edge_points.end() )
			edge_points.push_back(i);
		if (b_in &&  std::find(edge_points.begin(), edge_points.end(), j)==edge_points.end())
			edge_points.push_back(j);
		if (c_in &&  std::find(edge_points.begin(), edge_points.end(), k)==edge_points.end())
			edge_points.push_back(k);
	}
}

void Mesh::makeCoin(picture front, picture back, Mesh* out_mesh)
{
	out_mesh->vertex_count = RES*RES*2;
	out_mesh->face_count = 0;

	int EST_PERIMETER = RES*3.1415+1;

	out_mesh->vertexBuffer = new Eigen::Vector3d[RES*RES*2];
	out_mesh->indexBuffer = new int[(RES-1)*(RES-1)*6*2+EST_PERIMETER*2];

	for (int i = 0; i < RES; ++i)
		for(int j = 0; j < RES; ++j)
		{
			out_mesh->vertexBuffer[(i*RES+j)*2] = Eigen::Vector3d(i,j,0)*(DIAMETER/RES);
			out_mesh->vertexBuffer[(i*RES+j)*2+1] = Eigen::Vector3d(i,j,THICKNESS)*(DIAMETER/RES);

			int i_index = min(front.height, front.width)*i/RES;
			int j_index = min(front.height, front.width)*j/RES;
			int i_index2 = min(back.height, back.width)*i/RES;
			int j_index2 = min(back.height, back.width)*j/RES;

			unsigned char front_pixel = front.image[(i_index*front.width+j_index)*4]; 
			unsigned char back_pixel = back.image[(i_index2*back.width+j_index2)*4]; 

			double front_relief = (4-front_pixel/64)*0.2;
			double back_relief = (4-back_pixel/64)*0.2;

			out_mesh->vertexBuffer[(i*RES+j)*2+1](2) += front_relief;
			out_mesh->vertexBuffer[(i*RES+j)*2](2) -= back_relief;
		}

	int k = 0;

	std::vector<int> edge_points;

	for (int i = 0; i < RES-1; ++i)
		for(int j = 0; j < RES-1; ++j)
		{
			build_circle((i*RES+j)*2, ((i+1)*RES+j)*2, (i*RES+j+1)*2, out_mesh->indexBuffer, out_mesh->vertexBuffer,
		 	k, edge_points);
			build_circle((i*RES+j+1)*2, ((i+1)*RES+j)*2, ((i+1)*RES+j+1)*2, out_mesh->indexBuffer, out_mesh->vertexBuffer,
		 	k, edge_points);
		 	build_circle((i*RES+j)*2+1, (i*RES+j+1)*2+1, ((i+1)*RES+j)*2+1, out_mesh->indexBuffer, out_mesh->vertexBuffer,
		 	k, edge_points);
			build_circle((i*RES+j+1)*2+1, ((i+1)*RES+j+1)*2+1, ((i+1)*RES+j)*2+1, out_mesh->indexBuffer, out_mesh->vertexBuffer,
		 	k, edge_points);
		}

		//sort edge points by angle
    int edge_size = edge_points.size();
	std::vector<int> sorted_edge_points;


    for (int i = 0; i < edge_size; ++i)
    {
    	int smallest = 1;
    	Eigen::Vector3d anchor = CENTER(out_mesh->vertexBuffer[edge_points[0]]);
    	for (int j = 0; j < edge_points.size(); ++j)
    	{
    		if (CENTER(out_mesh->vertexBuffer[edge_points[smallest]]).cross(CENTER(out_mesh->vertexBuffer[edge_points[j]]))(2) < 0
    				&& CENTER(out_mesh->vertexBuffer[edge_points[smallest]]).cross(anchor)(2) > 0)
    		{
    			smallest = j;
    		}
    	}
    	//cout << out_mesh->vertexBuffer[edge_points[smallest]](0) << " " << out_mesh->vertexBuffer[edge_points[smallest]](1) << endl;
    	//cout <<  CENTER(out_mesh->vertexBuffer[edge_points[smallest]])(1)/CENTER(out_mesh->vertexBuffer[edge_points[smallest]])(0)<<endl;
    	sorted_edge_points.push_back(edge_points[smallest]);
    	if (edge_points.size() > 1)
  		  	edge_points.erase(edge_points.begin()+smallest);
    }

	for (int i = 0; i < sorted_edge_points.size(); i++)
	{
		out_mesh->indexBuffer[3*k] = sorted_edge_points[i];
		out_mesh->indexBuffer[3*k+1] = sorted_edge_points[i]+1;
		out_mesh->indexBuffer[3*k+2] = sorted_edge_points[(i+1)%edge_size];
		k++;

		out_mesh->indexBuffer[3*k] = sorted_edge_points[i]+1;
		out_mesh->indexBuffer[3*k+1] = sorted_edge_points[(i+1)%edge_size]+1;
		out_mesh->indexBuffer[3*k+2] = sorted_edge_points[(i+1)%edge_size];
		k++;
	}

	out_mesh->face_count = k;
}

