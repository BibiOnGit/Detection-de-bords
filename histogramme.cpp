#include "histogramme.h"
#include <cmath>


void gradient (unsigned char * u, unsigned char * grad, int h, int w){
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

void histo (int hist[], unsigned char * grad, int h, int w){// le 130051 provient du nombre total de valeurs de gradients
    for( int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            hist[grad[i*w+j]]++;
        }
    }
    for(int i=130050;i>=0;i--){
        hist[i]+=hist[i+1];
    }

}

double Hc(int mu, int hist[]){
    if (mu==130051){
        return 0;
    }
    return double(hist[mu+1])/double(hist[1]);
}
