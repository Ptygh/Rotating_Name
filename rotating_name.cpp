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

int main() {
  cout << "`e[2J`[H";

  return 0;
}