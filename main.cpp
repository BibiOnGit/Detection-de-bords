#include <Imagine/Images.h>
#include "tree.h"
#include "utils.h"
#include "drawtree.h"
#include "histogramme.h"
#include <iostream>
#include <limits>

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
    double Kpercent = 0.4;
    int epsilon = 100000;

    Window w1 = openWindow(w,h);
    setActiveWindow(w1);
    display(im);
    LsTree tree(im.data(), w, h);
    click();
    std::cout<<"En cours"<<std::endl;

    std::vector<int> pascTri;
    pascTri.push_back(1);
    pascalTriangle(pascTri,30);
    int * grad;
    gradient(im.data(),grad,h,w);
    int* hist = new int[HIST_SIZE];
    histo(hist,grad,h,w);

    Window w2 = openWindow(2*w,2*h);
    setActiveWindow(w2);
    tree.MeanB(tree.iNbShapes,epsilon,Kpercent,grad,w,h,hist,pascTri);

    tree.maxMeaningfulBoundaries();

    drawTree(tree);//for a window that is Twice biger

    std::cout << "Termine" <<std::endl;
    noRefreshEnd();
    delete [] hist;
    delete [] grad;
    endGraphics();
    return 0;
}
