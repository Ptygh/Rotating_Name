#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

/* 
ASCII characters in order of brightness: .,-~:;=!*#$@

T equation:
  -4.5 <= x <= -0.5 {1 <= y <= 2} {-0.5 <= z <= 0.5} top bar
  -3 <= x <= -2 {-2 <= y <= 1} {-0.5 <= z <= 0.5} stem
Y equation:
  -y + 2 <= x <= -y + 3 {0 <= y <= 2} {-0.5 <= z <= 0.5} left arm
  y + 2 <= x <= y + 3 {0 <= y <= 2} {-0.5 <= z <= 0.5} right arm
  2 <= x <= 3 {-2 <= y <= 0} {-0.5 <= z <= 0.5} stem

Dimensions:
-4.5 <= x <= 5
-2 <= y <= 2

*/

float A, B, C;

float calculateX(int i, int j, int k) {
  return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) +
         j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}

float calculateY(int i, int j, int k) {
  return j * cos(A) * cos(C) + k * sin(A) * cos(C) -
         j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) -
         i * cos(B) * sin(C);
}

float calculateZ(int i, int j, int k) {
  return k * cos(A) * cos(B) - j * sin(A) * cos(B) + j * sin(B);
}


const int width = 120, height = 30;
double minX = -4.5 - 0.5, maxX = 5 + 0.5;
double minY = -2 - 0.5, maxY = 2 + 0.5;

double resolution = 1;

char background = ' ';

struct Point3D {
  double x, y, z;
};
vector<Point3D> originalPoints;

bool isSurface(double x, double y, double z) {
  // front and back surfaces of 'T'
  if(x == -0.5 || x == 0.5) {
    // 'T' bar
    if((-4.5 <= y && y <= -0.5) && (1 <= z && z <= 2)) return true;
    // 'T' stem
    if((-3 <= y && y <= -2) && (-2 <= z && z <= 1)) return true;
  } else if(z >= 1) {     // middle surfaces of 'T' top bar
    if(y == -4.5 || y == -0.5) { return true; }
  } else {                // middle surfaces of 'T' stem
    if(y == -3 || y == -2) { return true; }
  }

  // front and back surfaces of 'Y'
  if(x == -0.5 || x == 0.5) {
    // 'Y' left arm
    if(((-z + 2) <= y && y <= (-z + 3)) && (0 <= z && z <= 2)) return true;
    // 'Y' right arm
    if(((z + 2) <= y && y <= (z + 3)) && (0 <= z && z <= 2)) return true;
    // 'Y' stem
    if((2 <= y && y <= 3) && (-2 <= z && z <= 0)) return true;
  } else if(z <= 0) {    // middle surfaces of 'Y' stem 
    if(y == 2 || y == 3) { return true; }
  } else {               // middle surfaces of 'Y' arms
    if(-z + 2 == y || -z + 3 == y) { return true; }
  }
  return false;
}

int main() {
  // for (int row = 0; row < height; row++) {
  //   double y = maxY - row * yStep;
  //   for (int col = 0; col < width; col++) {
  //     double x = minX + col * xStep;

  //     if (isSurface(x, y)) {
  //       buffer[row][col] = '@';
  //     } else {
  //       buffer[row][col] = background;
  //     }
  //   }
  // }

  // for(auto& column : buffer) {
  //   for(auto& row : column) {
  //     cout << row;
  //   }
  //   cout << endl;
  // }
  return 0;
}