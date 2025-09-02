#include <cmath>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
using namespace std;

// Variables used for rotation
float A = 0.0f, B = 0.0f, C = 0.0f;
// Dimensions of the 3D formulas for the letters
float minX = -5.0f, maxX = 5.5f;
float minY = -2.5f, maxY = 2.5f;
float minZ = -0.5f, maxZ = 0.5f;

const int WIDTH = 120, HEIGHT = 50;
float stepSpeed = 0.1f;
char background = ' ';
char buffer[HEIGHT][WIDTH];
float zBuffer[HEIGHT][WIDTH];
const float LARGE_NUM = 1e9f;

const char* shades = ".,-~:;=!*#$@";
const int numShades = 12;
float shadeRange = (maxX - minX) / numShades;  // Determine shade ranges based on largest distance (width)

struct Point3D {
  float x, y, z;
};
vector<Point3D> originalPoints;

struct Point2D {
  int x, y;
};
// Use ANSI escape sequences
void clearScreen() {
  cout << "\x1b[2J\x1b[H";
}
// Check whether the point in 3D space lies within the bounds of the letters
bool isSurface(float x, float y, float z) {
  if(-0.5f <= z && z <= 0.5f) {
    // 'T' shape:
    if((-4.5f <= x && x <= -0.5f) && (1.0f <= y && y <= 2.0f)) return true;         // top bar of 'T'
    else if((-3.0f <= x && x <= -2.0f) && (-2.0f <= y && y <= 1.0f)) return true;   // stem of 'T'
    // 'Y' shape:
    else if(((-y + 2.0f) <= x && x <= (-y + 3.0f)) && (0 <= y && y <= 2.0f)) return true;   // left arm of 'Y'
    else if(((y + 2.0f) <= x && x <= (y + 3)) && (0 <= y && y <= 2.0f)) return true;        // right arm of 'Y'
    else if((2.0f <= x && x <= 3.0f) && (-2 <= y && y <= 0)) return true;                   // stem of 'Y'
  }
  return false;
}
// Iterate through points in 3D space and check their position against letter bounds. If inside, add the point to points vector
void generatePoints() {
  originalPoints.clear();
  for(float x = minX; x <= maxX; x += stepSpeed) {
    for(float y = minY; y <= maxY; y += stepSpeed) {
      for(float z = minZ; z <= maxZ; z += stepSpeed) {
        if(isSurface(x, y, z)) originalPoints.push_back({x, y, z});
      }
    }
  }
}

// Standard 3D rotation (X, then Y, then Z)
void rotatePoint(float x, float y, float z, float A, float B, float C, float& outX, float& outY, float& outZ) {
  // X axis
  float x1 = x;
  float y1 = y * cos(A) - z * sin(A);
  float z1 = y * sin(A) + z * cos(A);

  // Y axis
  float x2 = x1 * cos(B) + z1 * sin(B);
  float y2 = y1;
  float z2 = -x1 * sin(B) + z1 * cos(B);

  // Z axis
  outX = x2 * cos(C) - y2 * sin(C);
  outY = x2 * sin(C) + y2 * cos(C);
  outZ = z2;
}

Point2D projectPoint(float x, float y, float z) {
  float distance = 20.0f; // Camera distance
  float scale = 7.0f;   // Zoom factor

  float factor = distance / (z + distance);  // Avoid div by zero

  int screenX = static_cast<int>(x * factor * scale + WIDTH / 2);
  int screenY = static_cast<int>(-y * factor * scale + HEIGHT / 2);

  return {screenX, screenY};
}

void clearBuffers() {
  for(int x = 0; x < WIDTH; ++x) {
    for(int y = 0; y < HEIGHT; ++y) {
      buffer[y][x] = background;
      zBuffer[y][x] = LARGE_NUM;
    }
  }
}

void renderBuffer() {
  for(int y = 0; y < HEIGHT; ++y) {
    for(int x = 0; x < WIDTH; ++x) {
      cout << buffer[y][x];
    }
    cout << '\n';
  }
}
// Assign character representation based on depth
char brightness(float in) {
  for(int i = 1; i <= numShades; i++) {
    if(in <= (minX + (i * shadeRange))) return shades[numShades - i];
  }

  return ' ';
}


int main() {
  generatePoints();

  while(true) {
    A += 0.04f;
    B += 0.02f;
    C += 0.03f;

    clearBuffers();
    // Rotate each point in the vector
    for(const Point3D& p : originalPoints) {
      float rotatedX, rotatedY, rotatedZ;
      rotatePoint(p.x, p.y, p.z, A, B, C, rotatedX, rotatedY, rotatedZ);
      Point2D screen = projectPoint(rotatedX, rotatedY, rotatedZ);
      // Check that the point fits on the 'screen'
      if(screen.x >= 0 && screen.x < WIDTH && screen.y >= 0 && screen.y < HEIGHT) {
        float depth = rotatedZ + 20.0f;  // same value as 'distance' in projectPoint()
        // Check if the point is in front of what is stored at that position
        if(depth < zBuffer[screen.y][screen.x]) {
          zBuffer[screen.y][screen.x] = depth;
          buffer[screen.y][screen.x] = brightness(rotatedZ);
        }
      }
    }
    this_thread::sleep_for(chrono::milliseconds(60));
    clearScreen();
    renderBuffer();
  }
  return 0;
}