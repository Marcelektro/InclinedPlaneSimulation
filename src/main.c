#include "main.h"
#include <stdio.h>
#include <math.h>

#include "simulate.h"
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include "csv_output.h"
#include "gl_trail.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define MAX_POINTS 500 // max points in trail


Environment env;
Sphere sphere_solid, sphere_hollow;
Trail trail_solid, trail_hollow;

void setup_simulation() {
    // create inclined plane from (0,20) to (40,0) (top left to bottom right)
    const Vec2d plane_a = create_vec2d(0, 20);
    const Vec2d plane_b = create_vec2d(40, 0);

    env = create_environment(9.81, plane_a, plane_b);

    const double slope = (plane_b.y - plane_a.y) / (plane_b.x - plane_a.x);

    const double initial_x = 5; // initial x pos on the plane
    const double initial_y = plane_a.y + slope * (initial_x - plane_a.x); // y pos on the plane is calculated from the slope

    // solid ball
    const double mass_solid = 1.0;
    const double radius_solid = 2.0;
    sphere_solid = create_sphere(mass_solid, radius_solid, (2.0 / 5.0) * mass_solid * radius_solid*radius_solid, create_vec2d(initial_x, initial_y), 0);
    // hollow sphere
    const double mass_sphere = 1.0;
    const double radius_sphere = 2.5;
    sphere_hollow = create_sphere(mass_sphere, radius_sphere, (2.0 / 3.0) * mass_sphere * radius_sphere*radius_sphere, create_vec2d(initial_x, initial_y + 2), 0);

    perform_csv_output(env, sphere_solid, sphere_hollow);

    // create the trails
    trail_solid = create_trail((Color){200, 200, 0}, MAX_POINTS);
    trail_hollow = create_trail((Color){0, 200, 200}, MAX_POINTS);

}



void update_simulation(const double dt) {

    // printf("time in update_simulation: %f\n", dt);

    calculate_frame(&env, &sphere_solid, dt);
    calculate_frame(&env, &sphere_hollow, dt);

    trail_update(&trail_solid, sphere_solid.tracked_point.x, sphere_solid.tracked_point.y);
    trail_update(&trail_hollow, sphere_hollow.tracked_point.x, sphere_hollow.tracked_point.y);

}



void draw_sphere(const Sphere *sphere, const double r, const double g, const double b) {

    glPushMatrix();
    glColor3d(r, g, b);
    glTranslated(sphere->position.x, sphere->position.y, 0);
    glRotated(-sphere->theta*(180.0 / M_PI), 0, 0, 1);
    glutWireSphere(sphere->radius, 10, 10);
    glPopMatrix();


    // tracked point
    glColor3d(0.3, 0.1, 1.0);

    glBegin(GL_LINES);
    glVertex2d(sphere->position.x, sphere->position.y);
    glVertex2d(sphere->tracked_point.x, sphere->tracked_point.y);
    glEnd();

    glPointSize(4);
    glBegin(GL_POINTS);
    glVertex2d(sphere->tracked_point.x, sphere->tracked_point.y);
    glEnd();

}


void render_scene() {
    glClear(GL_COLOR_BUFFER_BIT);

    // inclined plane
    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3d(0.6, 0.6, 0.6);
    glVertex2d(env.plane_a.x, env.plane_a.y-0.2); // compensate for the line width
    glVertex2d(env.plane_b.x, env.plane_b.y-0.2);
    glEnd();

    // spheres
    glLineWidth(1);
    draw_sphere(&sphere_solid, 1.0, 0.2, 0.3);
    draw_sphere(&sphere_hollow, 0.3, 1.0, 0.2);

    // draw the trail
    trail_render(&trail_solid);
    trail_render(&trail_hollow);

    glfwSwapBuffers(glfwGetCurrentContext());
}


void perform_csv_output(const Environment env, Sphere sphere_solid, Sphere sphere_hollow) {

    double dt_sum = 0.0;
    const double dt = 0.05;

    int steps = 150;

    // output csv file
    FILE *csv_out;
    init_csv_output(&csv_out, "/mnt/tmpfs/output2.csv");

    for (int step = 0; step < steps; step++) {

        calculate_frame(&env, &sphere_solid, dt);
        calculate_frame(&env, &sphere_hollow, dt);

        if (sphere_solid.position.y < env.plane_b.y || sphere_hollow.position.y < env.plane_b.y) {
            break;
        }

        double E_potential1, E_kinetic1, E_total1;
        double E_potential2, E_kinetic2, E_total2;
        calculate_energies(&env, &sphere_solid, &E_potential1, &E_kinetic1, &E_total1);
        calculate_energies(&env, &sphere_hollow, &E_potential2, &E_kinetic2, &E_total2);

        write_csv_output(csv_out, dt_sum,
                    sphere_solid.position.x, sphere_solid.velocity, sphere_solid.theta, sphere_solid.omega, E_potential1, E_kinetic1, E_total1,
                    sphere_hollow.position.x, sphere_hollow.velocity, sphere_hollow.theta, sphere_hollow.omega, E_potential2, E_kinetic2, E_total2);

        dt_sum += dt;
    }

    close_csv_output(csv_out);

    printf("Output written to file\n");
}


int main(int argc, char **argv) {
    glutInit(&argc, argv);

    if (!glfwInit())
        return -1;

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "InclinedPlaneSimulation", NULL, NULL);

    if (!window) {
        perror("Error creating window, exiting...");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glOrtho(0, 50, 0, 25, -1, 1); // projection matrix

    setup_simulation();

    double prev_time = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        const double current_time = glfwGetTime();

        const double time_diff = current_time - prev_time;

        update_simulation(time_diff);

        if (sphere_solid.position.y < env.plane_b.y-2 && sphere_hollow.position.y < env.plane_b.y-2) {
            printf("Reached the end of the plane\n");

            // make the ball bounce back
            sphere_solid.velocity = -sphere_solid.velocity;
            sphere_hollow.velocity = -sphere_hollow.velocity;

            sphere_solid.omega = -sphere_solid.omega;
            sphere_hollow.omega = -sphere_hollow.omega;

            continue;
        }

        render_scene();

        glfwPollEvents();

        prev_time = current_time;
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
