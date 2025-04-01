#ifndef GL_TRAIL_H
#define GL_TRAIL_H

#include <GL/gl.h>


typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Color;

typedef struct {
    Color primary_color;
    int max_points;
    GLdouble *trail;  // store (x, y) for each point
    int size; // number of active points stored
    double last_x, last_y;  // last point in trail
} Trail;


Trail create_trail(Color color, int max_points);
void trail_update(Trail *trail, double x, double y);
void trail_render(const Trail *trail);


#endif //GL_TRAIL_H
