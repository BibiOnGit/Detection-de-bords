#ifndef HISTOGRAMME_H
#define HISTOGRAMME_H



void gradient (unsigned char * u, unsigned char * grad, int h, int w);


void histo (int hist[], unsigned char * grad, int h, int w);// le 130051 provient du nombre total de valeurs de gradients

double Hc(int mu, int hist[]);

#endif // HISTOGRAMME_H
