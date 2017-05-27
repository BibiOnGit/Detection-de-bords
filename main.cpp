#include <Imagine/Images.h>
#include "tree.h"
#include "utils.h"
#include "drawtree.h"
#include "histogramme.h"
#include <iostream>

using namespace Imagine;

int main(int argc, char* argv[]){
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
    double Kpurcent = 0.1;
    int epsilon = 1000;

    Window w1 = openWindow(w,h);
    setActiveWindow(w1);
    display(im);
    LsTree tree(im.data(), w, h);
    click();
    std::cout<<"En cours"<<std::endl;


    int * grad;
    gradient(im.data(),grad,h,w);
    int* hist = new int[HIST_SIZE];
    histo(hist,grad,h,w);


    Window w2 = openWindow(w,h);
    setActiveWindow(w2);
    tree.MeanB(tree.iNbShapes,epsilon,Kpurcent,grad,w,hist);
    tree.maxMeaningfulBoundaries();
    drawTree(tree);

    delete [] grad;
    endGraphics();
    return 0;
}
