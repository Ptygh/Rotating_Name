#include <cmath>
#include <iostream>

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


const int width = 160, height = 40;

int x, y;
double minX = -4.5, maxX = 5;       // +/- to add buffers along edges
double minY = -2, maxY = 2;     // ^^
double xStep = (abs(minX + maxX)) / width;
double yStep = (abs(minY + maxY)) / height;


char buffer[width][height];
char background = '-';

bool isSurface(double x, double y) {
  // 'T' bar
  if((-4.5 <= x && x <= 0.5) && (1 <= y && y <= 2)) {
    return true;
  }
  // 'T' stem
  else if((-3 <= x && x <= -2) && (-2 <= y && y <= 1)) {
    return true;
  }
  // 'Y' left arm
  else if(((-y + 2) <= x && x <= (-y + 3)) && (0 <= y && y <= 2)) {
    return true;
  }
  // 'Y' right arm
  else if(((y + 2) <= x && x <= (y + 3)) && (0 <= y && y <= 2)) {
    return true;
  }
  // 'Y' stem
  else if((2 <= x && x <= 3) && (-2 <= y && y <= 0)) {
    return true;
  }
  else { return false; }
}

int main() {
  int bufferX = 0, bufferY = 0;
  int charcount = 0;

  x = minX;
  y = maxY;
  for(bufferY; bufferY < height; bufferY++) {
    for(bufferX; bufferX < width; bufferX++) {
      if(!isSurface(x, y)) {
        buffer[bufferX][bufferY] = '@';
        cout << '@';
      } else {
        buffer[bufferX][bufferY] = background;
        cout << background;
      }
      x += xStep;
      charcount++;
    }
    x = minX;
    cout << endl;
    y -= yStep;
  }
  cout << charcount;

  // for(auto& column : buffer) {
  //   for(auto& row : column) {
  //     cout << row;
  //   }
  //   cout << endl;
  // }

  //while(1) {
  //  cout << "\033[2J\033[H";
  //}
  return 0;
}