#ifndef OBJECT
#define OBJECT

#include <cstdlib>
#include <iostream>
#include <vector>
#include <cmath>

#define x_coor coordinates[0]
#define y_coor coordinates[1]
#define z_coor coordinates[2]
#define width dim[0]
#define height dim[1]
#define length dim[2]
using namespace std;

class Object{
public:
  float min_X, max_X, min_Y, max_Y, min_Z, max_Z;

  Object(){
    min_X = max_X = min_Y = max_Y = min_Z = max_Z = 0;
  }
  //TODO constructor with length and starting pos

};

/*************************************************************************************************************/
class Triangle : public Object{
public:
  vector<vector<float>> coor;
  vector<vector<float>> color;
  Triangle(vector<vector<float>>, vector<vector<float>>);
  vector<vector<float>> getCoor();
};

/*************************************************************************************************************/
class Square : public Object{
public:
  vector<vector<float>> coor; // 4 coords 
  vector<vector<float>> color; //color[i] corresponds to coor[i]'s color
  Square();
  vector<vector<float>> getCoor();

};

/*************************************************************************************************************/
class Cube : public Object {
public:
  vector<vector<float>> coor;
  vector<vector<float>> color;
  vector<float> dimensions; //@TODO
  vector<float> pos; //@TODO
  vector<float> all_triangles;
  vector<float> colors;
  float min_X, min_Y, min_Z, max_X, max_Y, max_Z; //@TODO initialize
  Cube();
  Cube(vector<vector<float>> coords, vector<vector<float>> colors) : Object(){
    this->coor = coords;
    this->color = colors;


  }
  //Change to (starting point, dimensions, colors of faces)

  vector<vector<float>> getCoor(){
    vector<vector<float>> retval;
    for(int i = 0; i < 8; i++){
      vector<float> temp;
      temp.clear();
      for(int j = 0; j < 3; j++){
        temp.push_back(this->coor[i][j]);
      }
      retval.push_back(temp);
    }
    return retval;
  }

  void makeCube(vector<float> dimensions, vector<float> position){
    
  }
  
}; 

/*************************************************************************************************************/
bool isCollision(Object a, Object b){
  return (a.min_X <= b.max_X && a.max_X >= b.min_X) && 
  (a.min_Z <= b.max_Z && a.max_Z >= b.min_Z) &&
  (a.min_Y <= b.max_Y && a.max_Y >= b.min_Y);
}

void setBounds(Cube &c){
  for(vector<float> point : c.coor){
    if(point[0] < c.min_X) c.min_X = point[0];
    if(point[0] > c.max_X) c.max_X = point[0];

    if(point[1] < c.min_Y) c.min_Y = point[1];
    if(point[1] > c.max_Y) c.max_Y = point[1];

    if(point[2] < c.min_Z) c.min_Z = point[2];
    if(point[2] > c.max_Z) c.max_Z = point[2];
  }
  if(abs(c.min_X - c.max_X) != c.dimensions[0] || abs(c.min_Y - c.max_Y) != c.dimensions[1] || abs(c.min_Z- c.max_Z) != c.dimensions[2]){
    cout<<"Error setting bounds, length != min-max\n";
  }

}

void createTriangles(Cube &c, vector<float> front_face, vector<float> back_face, vector<float> left_face, vector<float> right_face,
vector<float> top_face, vector<float> bottom_face){
  vector<float> coordinates = c.pos;
  vector<float> dim = c.dimensions;
  
 //vector<float> 
 c.all_triangles = {
  //Front 1
  x_coor, y_coor, z_coor, x_coor, y_coor + height, z_coor, x_coor + width, y_coor, z_coor,
  //Front 2
  x_coor+width, y_coor+height, z_coor,x_coor, y_coor+height, z_coor, x_coor+width, y_coor, z_coor,
  //Back 1
  x_coor, y_coor, z_coor+length, x_coor, y_coor+height, z_coor+length, x_coor+width, y_coor, z_coor+length,
  //Back 2
  x_coor+width, y_coor+height, z_coor+length,x_coor, y_coor+height, z_coor+length, x_coor+width, y_coor, z_coor+length,
  //Left 1
  x_coor, y_coor, z_coor + length, x_coor, y_coor+height, z_coor+length, x_coor, y_coor, z_coor,
  //Left 2
  x_coor, y_coor, z_coor + length, x_coor, y_coor + height, x_coor, y_coor, z_coor,
  //Right 1
  x_coor+width, y_coor, z_coor + length, x_coor + width, y_coor+height, z_coor+length, x_coor + width, y_coor, z_coor,
  //Right 2
  x_coor + width, y_coor, z_coor + length, x_coor + width, y_coor + height, x_coor + width, y_coor, z_coor,
  //Top 1
  x_coor, y_coor + height, z_coor, x_coor, y_coor + height, z_coor + length, x_coor + width, y_coor + height,z_coor,
  //Top 2
  x_coor + width, y_coor + height, z_coor + length, x_coor, y_coor + height, z_coor + length, x_coor + width, y_coor + height, z_coor,
  //Bottom 1
  x_coor, y_coor, z_coor, x_coor, y_coor, z_coor + length, x_coor + width, y_coor,z_coor,
  //Bottom 2
  x_coor + width, y_coor, z_coor + length, x_coor, y_coor, z_coor + length, x_coor + width, y_coor, z_coor
  };
  
  //vector<float> 
  c.colors = {
  //Front Face  
  front_face[0], front_face[1], front_face[2], front_face[0], front_face[1], front_face[2], front_face[0], front_face[1], front_face[2], 
  front_face[0], front_face[1], front_face[2], front_face[0], front_face[1], front_face[2], front_face[0], front_face[1], front_face[2],
  //Back Face
  back_face[0], back_face[1], back_face[2], back_face[0], back_face[1], back_face[2], back_face[0], back_face[1], back_face[2],
  back_face[0], back_face[1], back_face[2], back_face[0], back_face[1], back_face[2], back_face[0], back_face[1], back_face[2],
  //Left Face
  left_face[0], left_face[1], left_face[2], left_face[0], left_face[1], left_face[2], left_face[0], left_face[1], left_face[2], 
  left_face[0], left_face[1], left_face[2], left_face[0], left_face[1], left_face[2], left_face[0], left_face[1], left_face[2],
  //Right Face
  right_face[0], right_face[1], right_face[2], right_face[0], right_face[1], right_face[2], right_face[0], right_face[1], right_face[2],
  right_face[0], right_face[1], right_face[2], right_face[0], right_face[1], right_face[2], right_face[0], right_face[1], right_face[2],
  //Top Face
  top_face[0], top_face[1], top_face[2], top_face[0], top_face[1], top_face[2], top_face[0], top_face[1], top_face[2],
  top_face[0], top_face[1], top_face[2], top_face[0], top_face[1], top_face[2], top_face[0], top_face[1], top_face[2],
  //Bottom Face
  bottom_face[0], bottom_face[1], bottom_face[2], bottom_face[0], bottom_face[1], bottom_face[2], bottom_face[0], bottom_face[1], bottom_face[2],
  bottom_face[0], bottom_face[1], bottom_face[2], bottom_face[0], bottom_face[1], bottom_face[2], bottom_face[0], bottom_face[1], bottom_face[2]
  };
}


#endif
