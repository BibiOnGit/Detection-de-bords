#include "tree.h"
#include <cassert>

LsTree::LsTree(LsShape* shapes){ ///Constructor test
    this->shapes = shapes;
}
/// Constructor.
LsTree::LsTree(const unsigned char* gray, int w, int h) {
    nrow = h; ncol = w;

    // Set the root of the tree. #shapes <= #pixels
    LsShape* pRoot = shapes = new LsShape[nrow*ncol];
    pRoot->type = LsShape::INF; pRoot->gray = 255;
    pRoot->bBoundary = true;
    pRoot->bIgnore = false;
    pRoot->area = nrow*ncol;
    pRoot->parent = pRoot->sibling = pRoot->child = 0;
    pRoot->pixels = 0;
    iNbShapes = 1;

    smallestShape = new LsShape*[ncol*nrow];
    for(int i = ncol*nrow-1; i >= 0; i--)
        smallestShape[i] = pRoot;

    flst_td(gray);
}

/// Destructor.
LsTree::~LsTree() {
    if(shapes && iNbShapes > 0)
        delete [] shapes[0].pixels;
    delete [] shapes;
    delete [] smallestShape;
}

/// Reconstruct an image from the tree
unsigned char* LsTree::build_image() const {
    unsigned char* gray = new unsigned char[nrow*ncol];
    unsigned char* out = gray;
    LsShape** ppShape = smallestShape;
    for(int i = nrow*ncol-1; i >= 0; i--) {
        LsShape* pShape = *ppShape++;
        while(pShape->bIgnore)
            pShape = pShape->parent;
        *out++ = pShape->gray;
    }
    return gray;
}

/// Smallest non-removed shape at pixel (\a x,\a y).
LsShape* LsTree::smallest_shape(int x, int y) {
    LsShape* pShape = smallestShape[y*ncol + x];
    if(pShape->bIgnore)
        pShape = pShape->find_parent();
    return pShape;
}


void LsTree::MeanB(int Nll, double epsilon, float Kpercent, unsigned char * grad, int w, int hist[]){///applied on the tree
    LsTreeIterator itTree(LsTreeIterator::Pre,&this->shapes[0]);
    LsTreeIterator endTree =itTree.end(LsTreeIterator::Pre,&this->shapes[0]);
    unsigned char Mu;
    for(;itTree!=endTree;++itTree){
        LsShape* currentShape = *itTree;
        Mu = currentShape->MuK(Kpercent,grad,w);
        currentShape->NFAk(Nll,Kpercent,Hc(Mu,hist));
        if(currentShape->NFA>epsilon){
            currentShape->remove();
        }
    }
}

void LsTree::maxMeaningfulBoundaries_rec(LsShape* shape, Monotony monotony, double minNFA,
                                         short int previousGrey){
    int childNumber = shape->childNumber();
    double currentNFA = shape->NFA;
    if(childNumber == 0 ){
        if((monotony == INCREASING && previousGrey <=shape->gray)
                ||(monotony == DECREASING && previousGrey >=shape->gray)
                || monotony == UNDETERMINED){
            minNfaShapeKeeper(shape,currentNFA,minNFA,false);
            return;
        }
     }
    else{
        if(monotony == NOT_MONOTONE && childNumber == 1){
            monotony = UNDETERMINED;
            minNFA = currentNFA;
        }
        else if(monotony == UNDETERMINED){
            minNfaShapeKeeper(shape,currentNFA,minNFA,false);
            if(childNumber == 1)
                monotony = (previousGrey > shape->gray)?DECREASING:INCREASING;
            else
                monotony = NOT_MONOTONE;
        }
        else if(monotony == DECREASING || monotony == INCREASING){
            monotoneSectionManager(shape,monotony,previousGrey,minNFA,currentNFA,childNumber);
        }
        previousGrey = shape->gray;
        shape = shape->find_child();
        do{
            maxMeaningfulBoundaries_rec(shape,monotony,minNFA,previousGrey);
            shape = shape->find_sibling();
        }while(shape !=0);
    }
}
void LsTree::maxMeaningfulBoundaries(){
    setRemovable();
    LsShape* root = &shapes[0];
    maxMeaningfulBoundaries_rec(root,NOT_MONOTONE,0,0);
}
void LsTree::monotoneSectionManager(LsShape *shape,Monotony &monotony,short int previousGrey,double minNFA, double currentNFA, int childNumber){
    char sign = (monotony == INCREASING)?1:-1;
    if(childNumber >1){
        monotony = NOT_MONOTONE;
    }
    else{
        if(sign*shape->gray >= sign*previousGrey){
            if(sign*shape->find_child()->gray < sign*shape->gray)
                minNfaShapeKeeper(shape,currentNFA,minNFA,true);
            else
                minNfaShapeKeeper(shape,currentNFA,minNFA,false);
        }
        else{
            monotony = (monotony==INCREASING)?DECREASING:INCREASING;
            if(sign*shape->find_child()->gray > sign*shape->gray)
                minNfaShapeKeeper(shape,currentNFA,minNFA,true);
            else
                minNfaShapeKeeper(shape,currentNFA,minNFA,false);
        }
    }
}
void LsTree::minNfaShapeKeeper(LsShape *shape,double currentNFA,double &minNFA, bool shapeIsPivot){
    LsShape* parent = shape->find_parent();
    if(currentNFA < minNFA && parent->removable){
        parent->remove();
        minNFA = currentNFA;
        if(shapeIsPivot){
            shape->removable = false;
        }
    }
    else if(!shapeIsPivot)
        shape->remove();
}
void LsTree::setRemovable(){
    for(int i=0;i<iNbShapes;i++){
        shapes[i].removable = true;
    }
}

