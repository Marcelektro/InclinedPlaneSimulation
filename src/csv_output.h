#ifndef CSV_OUTPUT_H
#define CSV_OUTPUT_H

#include <stdio.h>

void init_csv_output(FILE **csv_file, const char *filename);
void write_csv_output(FILE *csv_file, double time,
            double position1, double velocity1, double theta1, double omega1, double E_potential1, double E_kinetic1, double E_total1,
            double position2, double velocity2, double theta2, double omega2, double E_potential2, double E_kinetic2, double E_total2);
void close_csv_output(FILE *csv_file);

#endif //CSV_OUTPUT_H
