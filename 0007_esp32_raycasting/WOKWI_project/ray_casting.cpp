#include "ray_casting.h"
#include <cmath>
#include <vector>

using namespace std;

// TODO: either player_radius must be in main, or step_length be here
#define FIELD_OF_VIEW 60
#define PLAYER_RADIUS 0.1
#define CLOSENESS_MODE 3
// hard to see when too close to the walls, so -3 to the wall height
//#define MAX_HEIGHT 15
#define pi acos(-1.0)

struct Dist {
    double d;
    bool is_ver;
};

static double d2r(double d) {
    // degrees to radians
    return (d / 180.0) * ((double) pi);
}

double sind(double a) {
    return sin(d2r(a));
}

double cosd(double a) {
    return cos(d2r(a));
}

int getWallHeight(double d, double angle, double a_i, int max_h) {
    // D'
    double d_ = d * sind(90-abs(angle-a_i));
    int h = 1 + 2 * std::round(max_h / d_ - CLOSENESS_MODE);
    h = h > max_h ? max_h : h;
    return h;
}

Point step(const vector<bool>& grid, int m, Point p0, double a, double step_length) {
    // recalculate coordinates of a player
    // m - grid width
    // a - angle in degrees, direction or the step (so to step to the left of the initial angle a0, do a0-90)
  
    Point p1;
    p1.x = p0.x + cosd(a) * step_length;
    p1.y = p0.y + sind(a) * step_length;  // minus??
    // maybe keep angles as integers and add Look Up Tables for sin and cos
    // TODO: how to check the speed of calculating cos and sin all the time?

    // check for collisions with walls
    // x
    float check_x = p1.x + (cosd(a) > 0 ? PLAYER_RADIUS : -PLAYER_RADIUS);
    int map_x = (int)check_x;
    int map_y = (int)p0.y;

    if (grid[map_y * m + map_x]) {
        // can't go there, it's a wall
        p1.x = p0.x;  // or maybe move enough to not hit a wall?
    }

    // y
    float check_y = p1.y + (sind(a) > 0 ? PLAYER_RADIUS : -PLAYER_RADIUS);
    map_x = (int)p0.x;
    map_y = (int)check_y;

    if (grid[map_y * m + map_x]) {
        p1.y = p0.y;
    }

    return p1;
}


Dist castRayDDA(const vector<bool>& grid, int m, double a, double x0, double y0) {
    // Return distance to closest wall, and if it's vertical
    int mapX = int(x0);
    int mapY = int(y0);

    // ray direction - cos(a), sin(a)
    double rayDirX = cosd(a);
    double rayDirY = sind(a);

    // length of ray to cross one grid cell
    double deltaDistX = abs(1 / rayDirX);
    double deltaDistY = abs(1 / rayDirY);
    
    // step direction (+1 or -1)
    int stepX, stepY;
    double sideDistX, sideDistY;  // distance, in ray, to closest x boundary, y boundary 
    if (rayDirX < 0) {  // cos(a)<0, a>90, looking to the left
        stepX = -1;
        sideDistX = (x0 - mapX) * deltaDistX;
    } else {  // cos(a)>=0, a<=90, looking to the right
        stepX = 1;
        sideDistX = (mapX + 1 - x0) * deltaDistX;
    }
    //cerr << "stepX=" << stepX << ", sideDistX=" << sideDistX << endl;

    if (rayDirY < 0) {  // sin(a)<0, looking down
        stepY = -1;
        sideDistY = (y0 - mapY) * deltaDistY;
    } else {  // sin(a)>=0, looking up
        stepY = 1;
        sideDistY = (mapY + 1 - y0) * deltaDistY;
    }

    bool hit = false;
    bool is_vertical;
    double distToWall;

    int i = 0;
    while (!hit) {
        // choose closest next grid boundary
        if (sideDistX < sideDistY) {  // do a horizontal step, because next x is closer
            sideDistX += deltaDistX;
            mapX += stepX;
            is_vertical = true;
        } else {  // do a vertical step, because next y is closer
            sideDistY += deltaDistY;
            mapY += stepY;
            is_vertical = false;
        }

        i += 1;
        // check if we hit a wall
        if (grid[mapY * m + mapX]) {
            hit = true;
            if (is_vertical) {
                distToWall = sideDistX - deltaDistX;
            } else {
                distToWall = sideDistY - deltaDistY;
            }
        }
    }

    return {distToWall, is_vertical};
}

void getRows(const vector<bool>& grid, int n, int m, double angle, double x0, double y0, int total_height, int halfRaysN, std::vector<RayResult>& results) {
    // angle - CLOCKWISE
    // 0, 0 - top left, x and y are positive
    // m - grid width
    // results will have (2*halfRaysN+1) RayResult values
    // [{13, true}, {9, false}, ...]

    results.clear();

    int h;
    double a;
    Dist d;
    double angleStep = FIELD_OF_VIEW / (2.0 * halfRaysN);
    for (int ray_i=-halfRaysN; ray_i<halfRaysN+1; ray_i++) {
        a = angle + ray_i * angleStep;
        d = castRayDDA(grid, m, a, x0, y0);
    
        h = getWallHeight(d.d, angle, a, total_height);
        results.push_back({h, d.is_ver, a, d.d});
    }
}
