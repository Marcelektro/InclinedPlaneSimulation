#ifndef SIMULATE_H
#define SIMULATE_H


typedef struct {
    double x;
    double y;
} Vec2d;

Vec2d create_vec2d(double x, double y);


typedef struct {
    double gravity;

    // two points defining the plane
    Vec2d plane_a; // the point with the highest y value
    Vec2d plane_b; // the point with the lowest y value

    double inclination_angle; // calculated from the two points

} Environment;

Environment create_environment(double gravity, Vec2d plane_a, Vec2d plane_b);


typedef struct {
    // constants
    double mass;
    double radius;
    double moment_of_inertia;

    // properties
    Vec2d position; // we'll only use x, because y will be always the radius (distance from the plane)
    double velocity;
    double acceleration;

    // rotation, angular velocity and acceleration
    double theta;
    double omega;
    double epsilon;
} Sphere;

Sphere create_sphere(double mass, double radius, double moment_of_inertia,
                     Vec2d initial_position, double initial_velocity);


void perform_midpoint_method(double dt, double accel, double *y, double *dy_dt);
double calculate_acceleration(const Sphere *sphere, const Environment *env);
Vec2d calculate_sphere_tracked_point(const Sphere *sphere);
void calculate_frame(const Environment *env, Sphere *sphere, double dt);
void calculate_energies(const Environment *env, const Sphere *sphere,
                        double *E_potential, double *E_kinetic, double *E_total);


#endif //SIMULATE_H
