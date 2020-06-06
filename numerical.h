#pragma once

#include "common.h"

typedef double (*numerical_fx)(double x);
typedef double (*numerical_fxy)(double x, double y);

double  numerical_rk_4th_order(numerical_fxy fxy, double x0, double y0,
                               double xn, double h);
double  numerical_simpsons_1_3rd(numerical_fx fx, double x0, double xn,
                                 double h);
double  numerical_trapezoidals(numerical_fx fx, double x0, double xn, double h);
double  numerical_bisection(numerical_fx fx, double a, double b, double epsilon,
                            i64 maxit);
double  numerical_newton_raphson(numerical_fx fx, numerical_fx fdx,
                                 double initial, double epsilon, i64 maxit);
double  numerical_regula_falsi(numerical_fx fx, double a, double b,
                               double epsilon, i64 maxit);
double *numerical_gauss_elimination(double **pivotted_matrix, siz num_coeff);

void test_numericals();
