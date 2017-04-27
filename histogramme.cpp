#include "histogramme.h"
#include <cmath>


void gradient (unsigned char * u, float * grad, int h, int w){
    for (int i=0; i<h; i++){
        for (int j=0; j<w; j++){
            if(i==0 || j==0){
                grad[i*w + j] = 0;
            } else{
                short int a = u[i*w + j] + u[i*w + j - 1] - u[i*w + j - w] - u[i*w + j - w - 1];
                short int b = u[i*w + j] + u[i*w + j - w] - u[i*w + j - w -1] - u[i*w + j - 1];
                grad[i*w + j] = std::sqrt(a*a + b*b);
            }
        }
    }
}

float histo (unsigned char * u, int h, int w, float mu){
    float res;
    float* grad = new float[w*h];
    gradient (u, grad, h, w);
    int sup_seuil = 0;
    int total = 0;
    for (int i=0; i<h; i++){
        for (int j=0; j<w; j++){
            if (grad[i*w + j] > mu){
                sup_seuil ++;
            }
            if (grad[i*w + j] > 0){
                total ++;
            }
        }
    }
    res = sup_seuil/total;
    return res;
}
