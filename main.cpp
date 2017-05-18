
#include <Imagine/Images.h>
#include "tree.h"
#include "utils.h"

#include "histogramme.h"
#include <iostream>
using namespace Imagine;

int main(int argc, char* argv[]) {
    if(argc!=2) {
        std::cerr << "Usage: " << argv[0] << " imageFile" << std::endl;
        return 1;
    }
    Image<byte> im;
    if(! load(im, argv[1])) {
        std::cerr << "Error loading image " << argv[1] << std::endl;
        return 1;
    }
    int w = im.width();
    int h = im.height();
    float Kpurcent = 0.1;
    int epsilon = 1000;
    openWindow(w, h);
    display(im);

    click();
    LsTree tree(im.data(), w, h);


    unsigned char * grad;

    gradient(im.data(),grad,h,w);
    int hist[130051];
    histo(hist,grad,h,w);
    tree.shapes[0].MeanB(tree.iNbShapes,epsilon,Kpurcent,grad,w,hist);
    tree.maxMeaningfulBoundaries();
    endGraphics();
    delete [] grad;
    return 0;
}
