#include "csv_output.h"
#include <stdlib.h>

void init_csv_output(FILE **csv_file, const char *filename) {
    *csv_file = fopen(filename, "w");

    if (*csv_file == NULL) {
        perror("Error opening file for writing");
        exit(1);
    }

    fprintf(*csv_file, "time,"
                       "solid_pos,solid_vel,solid_theta,solid_omega,solid_E_potential,solid_E_kinetic,solid_E_total,"
                       "solid_pos,solid_velo,solid_theta,solid_omega,solid_E_potential,solid_E_kinetic,solid_E_total\n");
    }

void write_csv_output(FILE *csv_file, const double time,
            const double position1, const double velocity1, const double theta1, const double omega1, const double E_potential1, const double E_kinetic1, const double E_total1,
            const double position2, const double velocity2, const double theta2, const double omega2, const double E_potential2, const double E_kinetic2, const double E_total2) {

    fprintf(csv_file, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
                time,
                position1, velocity1, theta1, omega1, E_potential1, E_kinetic1, E_total1,
                position2, velocity2, theta2, omega2, E_potential2, E_kinetic2, E_total2);

}


void close_csv_output(FILE *csv_file) {
    fclose(csv_file);
}

