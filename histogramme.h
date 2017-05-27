#ifndef HISTOGRAMME_H
#define HISTOGRAMME_H

#include <iostream>
const int HIST_SIZE = 2*510*510+1; //Number of possible gradient values

void gradient (const unsigned char *u, int *&grad, const int h, const int w);

void histo (int *hist,const int * grad,const int h,const int w);


double Hc(const int mu,const int *hist);

#endif // HISTOGRAMME_H
