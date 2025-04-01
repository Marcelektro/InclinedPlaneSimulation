#include "gl_trail.h"

#include <stdlib.h>
#include <string.h>


Trail create_trail(const Color color, const int max_points) {
    return (Trail) {
        .primary_color = color,
        .max_points = max_points,
        .trail = malloc(max_points * 2 * sizeof(GLdouble)),
        .size = 0,
        .last_x = 0,
        .last_y = 0
    };
}

void trail_update(Trail *trail, const double x, const double y) {

    const double dx = x - trail->last_x;
    const double dy = y - trail->last_y;
    const double diff = dx*dx + dy*dy;

    if (diff < 0.01) {
        return; // ignore tiny movements
    }


    // evicting list mechanism
    if (trail->size < trail->max_points) {
        trail->size++;
    } else {
        // remove the first point by moving the rest
        memmove(trail->trail, trail->trail + 2, (trail->max_points - 1) * 2 * sizeof(GLdouble));
    }

    // update the last point
    trail->trail[(trail->size - 1) * 2 + 0] = x;
    trail->trail[(trail->size - 1) * 2 + 1] = y;

    trail->last_x = x;
    trail->last_y = y;
}

void trail_render(const Trail *trail) {
    if (trail->size < 2)
        return; // need at least two points to draw a line

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(2.0f);

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < trail->size; i++) {

        // calculate multiplier for darkening the color
        const double multiplier = (double)i / (double)trail->size;

        glColor3d(trail->primary_color.r / 255.0 * multiplier,
                    trail->primary_color.g / 255.0 * multiplier,
                    trail->primary_color.b / 255.0 * multiplier);

        glVertex2d(trail->trail[i * 2], trail->trail[i * 2 + 1]);
    }
    glEnd();
}

