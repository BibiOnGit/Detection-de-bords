#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <iostream>
#include <cmath>
#include <cassert>

const int MAXIT = 100;
const double EPS = 3.0e-7;
const double FPMIN = 1.0e-30;

void pascalTriangle(std::vector<int> &t,const int n); ///Build pascal triangle in t until the n-th line

double binomiale(double k, double n, const double p, const std::vector<int> &t); ///Compute the sum of the tail of the binomial from the index k

double betai(double a, double b, double x); ///Returns the incomplete beta function I(x;a, b).

double betacf(double a, double b, double x); ///Evaluates continued fraction for incomplete beta function by modified Lentz’s method
#endif // UTILS_H
