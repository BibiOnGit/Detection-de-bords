#include <Imagine/Images.h>
#include "tree.h"
#include "utils.h"
#include "histogramme.h"
#include <iostream>
using namespace Imagine;

void drawTree(LsTree tree) {
    LsTreeIterator itTree(LsTreeIterator::Pre,&tree.shapes[0]);
    LsTreeIterator endTree =itTree.end(LsTreeIterator::Pre,&tree.shapes[0]);
    for(;itTree!=endTree;++itTree){
        LsShape* currentShape = *itTree;
        noRefreshBegin();
        std::vector<LsPoint>::iterator itShape, endShape=currentShape->contour.end();
        for(itShape=currentShape->contour.begin(); itShape!=endShape; ++itShape)
            drawPoint(itShape->x, itShape->y, BLACK);
        noRefreshEnd();
    }
}

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
    float Kpurcent = 0.3;
    int epsilon = 100000000;
    openWindow(w, h);
    display(im);

    click();
    std::cout<<"En cours"<<std::endl;
    LsTree tree(im.data(), w, h);

//    LsShape* shapes = new LsShape[13];
//    shapes[0].setShape(0,&shapes[1],0);
//    shapes[1].setShape(&shapes[0],&shapes[3],&shapes[2]);
//    shapes[2].setShape(&shapes[0],&shapes[4],0);
//    shapes[3].setShape(&shapes[1],&shapes[5],0);
//    shapes[4].setShape(&shapes[2],&shapes[6],0);
//    shapes[5].setShape(&shapes[3],&shapes[7],0);
//    shapes[6].setShape(&shapes[4],&shapes[9],0);
//    shapes[7].setShape(&shapes[5],&shapes[10],&shapes[8]);
//    shapes[8].setShape(&shapes[5],0,0);
//    shapes[9].setShape(&shapes[6],&shapes[11],0);
//    shapes[10].setShape(&shapes[7],0,0);
//    shapes[11].setShape(&shapes[9],0,&shapes[12]);
//    shapes[12].setShape(&shapes[9],0,0);
//    LsTree *tree = new LsTree();
//    tree->shapes = shapes;


//    delete [] shapes;
//    delete [] tree;


    unsigned char * grad;

    gradient(im.data(),grad,h,w);
    int hist[130051];
    histo(hist,grad,h,w);
    tree.MeanB(tree.iNbShapes,epsilon,Kpurcent,grad,w,hist);
//    tree.maxMeaningfulBoundaries();
    Window w1 = openWindow(w,h);
    setActiveWindow(w1);
    drawTree(tree);
    endGraphics();
    delete [] grad;
    return 0;
}
