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

#include <cmath>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
using namespace std;


const float PI = 3.14159265;
float A, B, C;

float minX = -4.5 - 0.5, maxX = 5 + 0.5;
float minY = -2 - 0.5, maxY = 2 + 0.5;
float minZ = -0.5, maxZ = 0.5;

const int WIDTH = 120, HEIGHT = 30;
float stepSpeed = 0.2;
char background = '.';
char buffer[HEIGHT][WIDTH];

struct Point3D {
  float x, y, z;
};
vector<Point3D> originalPoints;

struct Point2D {
  int x, y;
};

void clearScreen() {
  cout << "\x1b[2J\x1b[H";
}

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

bool isSurface(float x, float y, float z) {
  // front and back surfaces of 'T'
  if(z == -0.5 || z == 0.5) {
    // 'T' bar
    if((-4.5 <= x && x <= -0.5) && (1 <= y && y <= 2)) return true;
    // 'T' stem
    if((-3 <= x && x <= -2) && (-2 <= y && y <= 1)) return true;
  } else if(y >= 1) {     // middle surfaces of 'T' top bar
    if(x == -4.5 || x == -0.5) { return true; }
  } else {                // middle surfaces of 'T' stem
    if(x == -3 || x == -2) { return true; }
  }

  // front and back surfaces of 'Y'
  if(z == -0.5 || z == 0.5) {
    // 'Y' left arm
    if(((-y + 2) <= x && x <= (-y + 3)) && (0 <= y && y <= 2)) return true;
    // 'Y' right arm
    if(((y + 2) <= x && x <= (y + 3)) && (0 <= y && y <= 2)) return true;
    // 'Y' stem
    if((2 <= x && x <= 3) && (-2 <= y && y <= 0)) return true;
  } else if(y <= 0) {    // middle surfaces of 'Y' stem 
    if(x == 2 || x == 3) { return true; }
  } else {               // middle surfaces of 'Y' arms
    if(-y + 2 == x || -y + 3 == x) { return true; }
  }
  return false;
}

void generatePoints() {
  for (float x = minX; x <= maxX; x += stepSpeed) {
    for (float y = minY; y <= maxY; y += stepSpeed) {
      for (float z = minZ; z <= maxZ; z += stepSpeed) {
        if (isSurface(x, y, z)) {
          originalPoints.push_back({x, y, z});
        }
      }
    }
  }
};

// Projects a 3D point to 2D terminal coordinates
Point2D projectPoint(float x, float y, float z) {
  float distance = 3.0; // camera distance
  float scale = 10.0;   // zoom factor

  float factor = distance / (z + distance);  // Avoid div by zero

  int screenX = (x * factor * scale) + WIDTH / 2;
  int screenY = (-y * factor * scale) + HEIGHT / 2;


  return {screenX, screenY};
}

void clearBuffer() {
  for (int x = 0; x < WIDTH; ++x)
    for (int y = 0; y < HEIGHT; ++y)
      buffer[y][x] = background;
}

void renderBuffer() {
  for (int y = 0; y < HEIGHT; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      cout << buffer[y][x];
    }
    cout << '\n';
  }
}


void debugRenderOnce() {
  clearBuffer();

  for (const Point3D& p : originalPoints) {
    // No rotation — use original coordinates
    float x = p.x;
    float y = p.y;
    float z = p.z;

    // Project to screen
    Point2D screen = projectPoint(x, y, z);

    // Bounds check
    if (screen.x >= 0 && screen.x < WIDTH && screen.y >= 0 && screen.y < HEIGHT) {
      buffer[screen.y][screen.x] = '#';
    }
  }

  clearScreen();
  renderBuffer();
}





int main() {
  
  generatePoints();


  while(true) {
    A += 0.04;
    B += 0.02;
    C += 0.03;

    clearBuffer();

    for (const Point3D& p : originalPoints) {
      // Apply rotation
      float rotatedX = calculateX(p.x, p.y, p.z);  // this is new depth
      float rotatedY = calculateY(p.x, p.y, p.z);  // this is new width
      float rotatedZ = calculateZ(p.x, p.y, p.z);  // this is new height

      // Project rotated point
      Point2D screen = projectPoint(rotatedX, rotatedY, rotatedZ);

      // Bounds check
      if (screen.x >= 0 && screen.x < WIDTH && screen.y >= 0 && screen.y < HEIGHT) {
      buffer[screen.y][screen.x] = '#';
      }
    }

    clearScreen();
    renderBuffer();
    // wait for a little bit
    this_thread::sleep_for(chrono::milliseconds(50));
  }

  return 0;
}