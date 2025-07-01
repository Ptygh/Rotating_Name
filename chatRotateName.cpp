#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <thread>

// Define the screen size
const int WIDTH = 80;
const int HEIGHT = 30;
const float PI = 3.14159265f;

// Basic 3D point struct
struct Point3D {
    float x, y, z;
};

// Projects a 3D point to 2D screen space
void project(const Point3D& p, int& sx, int& sy, float viewerDist = 3.0f) {
    float factor = viewerDist / (viewerDist + p.z);
    sx = static_cast<int>((p.x * factor + 1) * WIDTH / 2);
    sy = static_cast<int>((-p.y * factor + 1) * HEIGHT / 2);
}

// Creates a box centered at (cx, cy, cz) with given size
std::vector<Point3D> createBox(float cx, float cy, float cz, float w, float h, float d) {
    float hw = w / 2, hh = h / 2, hd = d / 2;
    return {
        {cx - hw, cy - hh, cz - hd},
        {cx + hw, cy - hh, cz - hd},
        {cx + hw, cy + hh, cz - hd},
        {cx - hw, cy + hh, cz - hd},
        {cx - hw, cy - hh, cz + hd},
        {cx + hw, cy - hh, cz + hd},
        {cx + hw, cy + hh, cz + hd},
        {cx - hw, cy + hh, cz + hd}
    };
}

// Builds the 3D "T" shape
std::vector<Point3D> createLetterT() {
    std::vector<Point3D> points;

    // Top bar
    auto top = createBox(0.0f, 0.5f, 0.0f, 2.0f, 0.4f, 0.4f);
    points.insert(points.end(), top.begin(), top.end());

    // Stem
    auto stem = createBox(0.0f, -0.5f, 0.0f, 0.4f, 1.0f, 0.4f);
    points.insert(points.end(), stem.begin(), stem.end());

    return points;
}

// Rotates a point around Y axis
Point3D rotateY(const Point3D& p, float angle) {
    float c = cos(angle), s = sin(angle);
    return { c * p.x + s * p.z, p.y, -s * p.x + c * p.z };
}

// Clears the terminal using ANSI escape codes
void clearScreen() {
    std::cout << "\x1b[2J\x1b[H";
}

int main() {
    auto basePoints = createLetterT();
    float angle = 0.0f;

    while (true) {
        char screen[HEIGHT][WIDTH];
        for (int y = 0; y < HEIGHT; ++y)
            for (int x = 0; x < WIDTH; ++x)
                screen[y][x] = ' ';

        for (const auto& p : basePoints) {
            Point3D r = rotateY(p, angle);
            int sx, sy;
            project(r, sx, sy);
            if (sx >= 0 && sx < WIDTH && sy >= 0 && sy < HEIGHT)
                screen[sy][sx] = '#';
        }

        clearScreen();
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x)
                std::cout << screen[y][x];
            std::cout << '\n';
        }

        angle += 0.10f;
        std::this_thread::sleep_for(std::chrono::milliseconds(20)); // ~30 FPS
    }

    return 0;
}
