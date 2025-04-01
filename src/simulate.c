#include "simulate.h"
#include <math.h>


void perform_midpoint_method(const double dt, const double accel, double *y, double *dy_dt) {
    const double dy_half = *dy_dt + 0.5 * dt * accel;
    *y += dt * dy_half;
    *dy_dt += dt * accel;
}


void calculate_frame(const Environment *env, Sphere *sphere, const double dt) {
    const double accel = calculate_acceleration(sphere, env); // can be made constant

    // pos & velocity using the Midpoint method
    perform_midpoint_method(dt, accel, &sphere->position.x, &sphere->velocity);

    // update angular acc and motion
    sphere->epsilon = accel / sphere->radius;
    perform_midpoint_method(dt, sphere->epsilon, &sphere->theta, &sphere->omega);

    // recalc y based on the slope equation
    const double slope = (env->plane_b.y - env->plane_a.y) / (env->plane_b.x - env->plane_a.x);
    sphere->position.y = env->plane_a.y + slope * (sphere->position.x - env->plane_a.x) + sphere->radius;
}


double calculate_acceleration(const Sphere *sphere, const Environment *env) {
    const int direction = env->plane_b.y < env->plane_a.y ? -1 : 1;

    return env->gravity * direction *sin(env->inclination_angle) / (1 + sphere->moment_of_inertia / (sphere->mass * sphere->radius*sphere->radius));
}



// calc the point on the perimeter of the sphere, considering the rotation
Vec2d calculate_sphere_tracked_point(const Sphere *sphere) {
    return (Vec2d) {
        .x = sphere->position.x - sphere->radius * sin(sphere->theta),
        .y = sphere->position.y - sphere->radius * cos(sphere->theta)
    };
}


// energies of the sphere at the current state
void calculate_energies(const Environment *env, const Sphere *sphere,
                        double *E_potential, double *E_kinetic, double *E_total) {
    *E_potential = sphere->mass * env->gravity * sphere->position.y;
    *E_kinetic = sphere->mass * sphere->velocity * sphere->velocity * 0.5 + sphere->moment_of_inertia * sphere->omega * sphere->omega * 0.5;
    *E_total = *E_potential + *E_kinetic;
}




/***********\
|* Helpers *|
\***********/


Vec2d create_vec2d(const double x, const double y) {
    return (Vec2d) {x, y};
}

Environment create_environment(const double gravity, const Vec2d plane_a, const Vec2d plane_b) {
    // calculate the angle of inclination of the plane
    const double angle = atan((plane_b.y - plane_a.y) / (plane_b.x - plane_a.x));

    return (Environment) {
        .gravity = gravity,
        .plane_a = plane_a, .plane_b = plane_b,
        .inclination_angle = angle
    };
}

Sphere create_sphere(const double mass, const double radius, const double moment_of_inertia,
                     const Vec2d initial_position, const double initial_velocity) {
    return (Sphere) {
        .mass = mass,
        .radius = radius,
        .moment_of_inertia = moment_of_inertia,
        .position = initial_position,
        .velocity = initial_velocity,
        .acceleration = 0,
        .theta = 0, .omega = 0, .epsilon = 0
    };
}

