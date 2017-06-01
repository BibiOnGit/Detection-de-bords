#include <Imagine/Images.h>
#include "tree.h"
#include "utils.h"
#include "drawtree.h"
#include "histogramme.h"
#include <iostream>

using namespace Imagine;


void TransformationAffine(Image<byte>& im){
    float facteur=0.5;
    for (int i =0; i<im.width();i++){
        for (int j=0;j<im.height(); j++){
            float Nouvbyte=im(i,j)*facteur;

            im(i,j)=byte(Nouvbyte);
        }
    }
}





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

    double epsilon = pow(10,-30);

    Window w1 = openWindow(w,h);
    setActiveWindow(w1);

    TransformationAffine(im);

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

//    int x, y;
//    while(getMouse(x,y)==1) {
//        LsShape* s = tree.smallest_shape(x,y);
//        std::vector<LsPoint>::iterator it, end=s->contour.end();
//        std::cout << s->NFA << std::endl;
//        noRefreshBegin();
//        std::vector<int> gradshape;
//        for(it=s->contour.begin(); it!=end; ++it){
//            drawPoint(it->x, it->y, RED);
//            gradshape.push_back(grad[it->x+w*it->y]);
//        }
//        std::sort(gradshape.begin(),gradshape.end());
//        std::cout << gradshape[Kpercent*s->contour.size()-1] << std::endl;
//        int K = Kpercent*s->contour.size();
//        double l2n = s->length()/(2*s->contour.size());
//        if(K==0)
//            K=1;
//        double hc = Hc(gradshape[K-1],hist);
//        std::cout << hc << std::endl;
//        std::cout << Hc(4810,hist)<<std::endl;
//        double min = binomiale((K-1) * l2n,s->contour.size() * l2n,hc,pascTri );
//        std::cout << min << std::endl;
//        std::cout <<s->contour.size() << std::endl;
//        noRefreshEnd();
//    }


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
