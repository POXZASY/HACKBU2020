#ifndef OBJECT
#define OBJECT

#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

class Triangle{
public:
  vector<vector<float>> coor;
  vector<vector<float>> color;
  Triangle(vector<vector<float>>, vector<vector<float>>);
  vector<vector<float>> getCoor();
};

class Square{
public:
  vector<vector<float>> coor;
  vector<vector<float>> color;
  Square();
  vector<vector<float>> getCoor();

};

class Cube{
public:
  vector<vector<float>> coor;
  vector<vector<float>> color;
  Cube();
  vector<vector<float>> getCoor();
}

#endif
