#ifndef RAY_CASTING_H
#define RAY_CASTING_H

#include <vector>

struct RayResult {
    int h;
    bool is_ver;
    double a_i;
    double dist;
};

struct Point {
  double x;
  double y;
};

void getRows(const std::vector<bool>& grid, int n, int m, double angle, double x0, double y0, int total_height, int halfRaysN, std::vector<RayResult>& results);

Point step(const std::vector<bool>& grid, int m, Point p0, double a, double step_length);
#endif
