#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <iostream>
#include <cmath>
#include <cassert>

void pascalTriangle(std::vector<int> &t,const int n); ///Build pascal triangle in t until the n-th line

double binomiale(const int k,const int n,const double p); ///Compute the sum of the tail of the binomial from the index k


#endif // UTILS_H
