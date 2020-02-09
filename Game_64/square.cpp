#include "object.h"

Square::Square(vector<vector<float>> coor, vector<vector<float>> color){
  this->coor = coor;
  this->color = color;
}

Square::getCoor(){
  vector<vector<float>> retval;
  for(int i = 0; i < 4; i++){
    vector<float> temp;
    temp.clear();
    for(int j = 0; j < 3; j++){
      temp.push_back(this->coor[i][j]);
    }
    retval.push_back(temp);
  }
  return retval;
}