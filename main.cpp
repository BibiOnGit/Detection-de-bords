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
    double Kpercent = 0.4;
    double epsilon = pow(10,-8);

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

    Window w2 = openWindow(w,h);
    setActiveWindow(w2);
    tree.MeanB(tree.iNbShapes,epsilon,Kpercent,grad,w,h,hist,pascTri);
    tree.maxMeaningfulBoundaries();
    drawTree(tree);

    Window w3 = openWindow(2*w,2*h);
    setActiveWindow(w3);
    drawZoomTree(tree);
    std::cout << "Termine" <<std::endl;
    noRefreshEnd();
    delete [] hist;
    delete [] grad;
    endGraphics();
    return 0;
}
