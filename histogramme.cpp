#include "histogramme.h"
#include <cmath>


void gradient (const unsigned char * u, int *& grad,const int h,const int w){
    grad = new int[h*w];
    for (int i=0; i<h; i++){
        for (int j=0; j<w; j++){
            if(i==0 || j==0){
                grad[i*w + j] = 0;
            } else{
                short int a = u[i*w + j] + u[i*w + j - 1] - u[i*w + j - w] - u[i*w + j - w - 1];
                short int b = u[i*w + j] + u[i*w + j - w] - u[i*w + j - w -1] - u[i*w + j - 1];
                grad[i*w + j] = (a*a + b*b);
            }
        }
    }
}
void histo (int* hist, const int *grad, const int h, const int w){
    for(int i=0;i<HIST_SIZE;i++)//initialization
        hist[i]=0;
    for( int i=0;i<h;i++){ //fill
        for(int j=0;j<w;j++){
            hist[grad[i*w+j]]++;
        }
    }
    for(int i=HIST_SIZE -1;i>=0;i--)//cumulative histo
        hist[i]+=hist[i+1];
}

double Hc(const int mu, const int *hist){
    if (mu==HIST_SIZE)
        return 0;
    return double(hist[mu+1])/double(hist[1]);
}












