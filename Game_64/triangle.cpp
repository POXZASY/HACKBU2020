#include "object.h"

Triangle::Triangle(vector<vector<float>> coor, vector<vector<float>> color){
  this->coor = coor;
  this->color = color;
}

vector<vector<float>> Triangle::getCoor(){
  vector<vector<float>> retval;
  for(int i = 0; i < 3; i++){
    vector<float> temp;
    temp.clear();
    for(int j = 0; j < 3; j++){
      temp.push_back(this->coor[i][j]);
    }
    retval.push_back(temp);
  }
  return retval;
}
