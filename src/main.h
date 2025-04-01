#ifndef MAIN_H
#define MAIN_H
#include "simulate.h"

void perform_csv_output(Environment env, Sphere sphere_solid, Sphere sphere_hollow);
void setup_simulation();
void update_simulation(double dt);
void draw_sphere(const Sphere *sphere, double r, double g, double b);
void render_scene();

#endif // MAIN_H
