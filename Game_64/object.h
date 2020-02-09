#ifndef OBJECT
#define OBJECT

#include <cstdlib>
#include <iostream>
#include <vector>
#include <cmath>

#define x_coor coordinates[0]
#define y_coor coordinates[1]
#define z_coor coordinates[2]
#define width_ dim[0]
#define height_ dim[1]
#define length dim[2]
#define RoomDim(i) init_position[i]
#define THICKNESS 5
using namespace std;


/*
  Objects like tables, doors, windows(?), buttons, pressure plates, floor/plate that teleports you on collision [TELEPORT FUNCTION],
  global array of points and colors of points
*/
class Object{
public:
  float min_X, max_X, min_Y, max_Y, min_Z, max_Z;

  Object(){
    min_X = max_X = min_Y = max_Y = min_Z = max_Z = 0;
  }
  vector<float> all_triangles;
  vector<float> colors;

  //person function ONLY
  void updatePerson(float x, float y, float z){
    min_X = x - .5;
    min_Y = y - 5.7;
    min_Z = z - .5;
    max_X = x + .5;
    max_Y = y + .3;
    max_Z = z + .5;
  }
};

vector<Object> AllObjects;

class Cube : public Object {
public:
  vector<float> dimensions; // vector of lengths of form <width_, height_, length>
  vector<float> pos; //@TODO initialize <x, y ,z>
  vector<float> all_triangles;
  vector<float> colors;
  float min_X, min_Y, min_Z, max_X, max_Y, max_Z; 
  Cube();
  Cube(vector<float> init_position, vector<float> WHL, vector<float> front_face, vector<float> back_face, vector<float> left_face, vector<float> right_face,
  vector<float> top_face, vector<float> bottom_face) : Object(){
    this->pos = init_position;
    this->dimensions = WHL;
    this->pos = init_position;
    min_X = this->pos[0];
    min_Y = this->pos[1];
    min_Z = this->pos[2];
    max_X = this->pos[0] + this->dimensions[0];
    max_Y = this->pos[1] + this->dimensions[1];
    max_Z = this->pos[2] + this->dimensions[2];
    createTriangles(front_face, back_face, left_face, right_face, top_face, bottom_face);
  }
  void createTriangles(vector<float> front_face, vector<float> back_face, vector<float> left_face, vector<float> right_face,
  vector<float> top_face, vector<float> bottom_face){
    vector<float> coordinates = this->pos;
    vector<float> dim = this->dimensions;

    //vector<float>
    this->all_triangles = {
      //Front 1
      x_coor, y_coor, z_coor, x_coor, y_coor + height_, z_coor, x_coor + width_, y_coor, z_coor,
      //Front 2
      x_coor+width_, y_coor+height_, z_coor,x_coor, y_coor+height_, z_coor, x_coor+width_, y_coor, z_coor,
      //Back 1
      x_coor, y_coor, z_coor+length, x_coor, y_coor+height_, z_coor+length, x_coor+width_, y_coor, z_coor+length,
      //Back 2
      x_coor+width_, y_coor+height_, z_coor+length,x_coor, y_coor+height_, z_coor+length, x_coor+width_, y_coor, z_coor+length,
      //Left 1
      x_coor, y_coor, z_coor + length, x_coor, y_coor, z_coor, x_coor, y_coor+height_, z_coor+length,
      //Left 2
      x_coor, y_coor + height_, z_coor, x_coor, y_coor + height_, z_coor + length, x_coor, y_coor, z_coor,
      //Right 1
      x_coor + width_, y_coor, z_coor + length, x_coor + width_, y_coor, z_coor, x_coor + width_, y_coor+height_, z_coor+length,
      //Right 2
      x_coor + width_, y_coor + height_, z_coor, x_coor + width_, y_coor + height_, z_coor + length, x_coor + width_, y_coor, z_coor,
      //Top 1
      x_coor, y_coor + height_, z_coor, x_coor, y_coor + height_, z_coor + length, x_coor + width_, y_coor + height_,z_coor,
      //Top 2
      x_coor + width_, y_coor  + height_ , z_coor + length, x_coor, y_coor + height_, z_coor + length, x_coor + width_, y_coor + height_, z_coor,
      //Bottom 1
      x_coor, y_coor, z_coor, x_coor, y_coor, z_coor + length, x_coor + width_, y_coor,z_coor,
      //Bottom 2
      x_coor + width_, y_coor, z_coor + length, x_coor, y_coor, z_coor + length, x_coor + width_, y_coor, z_coor
    };

    //vector<float>
    this->colors = {
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
};

/*************************************************************************************************************/
bool isCollision(Cube a, Object b){
  return (a.min_X <= b.max_X && a.max_X >= b.min_X) &&
  (a.min_Z <= b.max_Z && a.max_Z >= b.min_Z) &&
  (a.min_Y <= b.max_Y && a.max_Y >= b.min_Y);
}

class Room : public Object{
public:
  Cube FrontWall;
  Cube BackWall;
  Cube LeftWall;
  Cube RightWall;
  Cube Ceiling;
  Cube Floor;
  vector<float> all_triangles;
  vector<float> colors;
  Room();
  Room(vector<float> init_position, vector<float> dim, vector<float> front_face, vector<float> back_face, vector<float> left_face, vector<float> right_face,
  vector<float> top_face, vector<float> bottom_face){
    this->FrontWall = Cube(init_position, {dim[0], dim[1], THICKNESS}, front_face, front_face, front_face, front_face, front_face,front_face);
    this->BackWall = Cube({RoomDim(0), RoomDim(1), RoomDim(2)+dim[2]}, {dim[0], dim[1], THICKNESS}, back_face, back_face, back_face, back_face, back_face, back_face);
    mergeArray_colors(FrontWall.colors);
    mergeArray_colors(BackWall.colors);
    mergeArray_coords(FrontWall.all_triangles);
    mergeArray_coords(BackWall.all_triangles);

    this->LeftWall = Cube(init_position, {THICKNESS, dim[1], dim[2]}, left_face, left_face, left_face, left_face, left_face, left_face);
    this->RightWall = Cube({RoomDim(0)+dim[0], RoomDim(1), RoomDim(2)}, {THICKNESS, dim[1], dim[2]}, right_face, right_face, right_face, right_face, right_face, right_face);
    
    mergeArray_colors(LeftWall.colors);
    mergeArray_colors(RightWall.colors);
    mergeArray_coords(LeftWall.all_triangles);
    mergeArray_coords(RightWall.all_triangles);
    this->Ceiling = Cube({RoomDim(0), RoomDim(1)+dim[1], RoomDim(2)}, {dim[0], THICKNESS, dim[2]}, top_face, top_face, top_face, top_face, top_face, top_face);
    this->Floor = Cube(init_position, {dim[0], -THICKNESS, dim[2]}, bottom_face, bottom_face, bottom_face, bottom_face, bottom_face, bottom_face);

    mergeArray_colors(Ceiling.colors);
    mergeArray_colors(Floor.colors);
    mergeArray_coords(Ceiling.all_triangles);
    mergeArray_coords(Floor.all_triangles);
  }

  void mergeArray_colors(vector<float> obj_color){
    vector<float> temp = this->colors;
    temp.insert(temp.end(), obj_color.begin(), obj_color.end());
    this->colors = temp;
  }
  void mergeArray_coords(vector<float> obj_position){
    vector<float> temp;
    temp.insert(temp.end(), obj_position.begin(),obj_position.end());
    this->all_triangles = temp;
}
};

class Button: public Cube{
public:
  bool IsPressed;
  Button();
  Button(vector<float> init_position, vector<float> WHL, vector<float> front_face, vector<float> back_face, vector<float> left_face, vector<float> right_face,
  vector<float> top_face, vector<float> bottom_face):Cube(init_position, WHL, front_face, back_face, left_face, right_face, top_face, bottom_face){
    IsPressed = false;
  }
};
#endif
