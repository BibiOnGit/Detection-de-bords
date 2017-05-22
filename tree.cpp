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

void LsTree::maxMeaningfulBoundaries(){
    setRemovable();
    Monotony monotony = NOT_MONOTONE;
    LsTreeIterator itTree(LsTreeIterator::Pre,&shapes[0]);
    short int previousGrey = (*itTree)->gray;
    double minNFA = (*itTree)->NFA;
    ++itTree;//We begin by the shape just after the root
    LsTreeIterator endTree =itTree.end(LsTreeIterator::Pre,&shapes[0]);

    for(;itTree!=endTree;++itTree){
        LsShape* currentShape = *itTree;
        int childNumber = currentShape->childNumber();
        double currentNFA = currentShape->NFA;
        if(childNumber == 0){//No child
            if((monotony == INCREASING && previousGrey <=currentShape->gray)
                    ||(monotony == DECREASING && previousGrey >=currentShape->gray)
                    || (monotony == UNDETERMINED)){//In a monotone section
                minNfaShapeKeeper(currentShape,currentNFA,minNFA,false);
            }
         }
        else{//One or several children
            if(monotony == NOT_MONOTONE && childNumber == 1){//Beginning of a new monotone section
                monotony = UNDETERMINED;
                minNFA = currentNFA;
            }
            else if(monotony == UNDETERMINED){//Second shape of a monotone section
                minNfaShapeKeeper(currentShape,currentNFA,minNFA,false);
                if(childNumber == 1)
                    monotony = (previousGrey > currentShape->gray)?DECREASING:INCREASING;
                else
                    monotony = NOT_MONOTONE;
            }
            else if(monotony == DECREASING || monotony == INCREASING){//Shape in a monotone section
                monotoneSectionManager(currentShape,monotony,previousGrey,minNFA,currentNFA,childNumber);
            }
            previousGrey = currentShape->gray;
        }
    }
}

void LsTree::monotoneSectionManager(LsShape *shape,Monotony &monotony,short int previousGrey,double minNFA, double currentNFA, int childNumber){
    char sign = (monotony == INCREASING)?1:-1;/*INCREASING and DECREASING case are managed in the same way
                                                with the difference that the inegalities change*/
    if(childNumber >1){
        monotony = NOT_MONOTONE;
    }
    else{//One child
        if(sign*shape->gray >= sign*previousGrey){ //The monotony doesn't change
            if(sign*shape->find_child()->gray < sign*shape->gray)//But the monotony changes just after
                minNfaShapeKeeper(shape,currentNFA,minNFA,true); //Current shape is a pivot
            else
                minNfaShapeKeeper(shape,currentNFA,minNFA,false);
        }
        else{//The monotony changes
            monotony = (monotony==INCREASING)?DECREASING:INCREASING;
            if(sign*shape->find_child()->gray > sign*shape->gray)//Monotony changes right after
                minNfaShapeKeeper(shape,currentNFA,minNFA,true);//Current shape is a pivot
            else
                minNfaShapeKeeper(shape,currentNFA,minNFA,false);
        }
    }
}
void LsTree::minNfaShapeKeeper(LsShape *shape,double currentNFA,double &minNFA, bool shapeIsPivot){
    LsShape* parent = shape->find_parent();
    if(currentNFA < minNFA && !(parent->pivotShape)){/*NFA of current shape is smaller than the NFA
                                                  of the parent so we remove the parent if it's not a pivot*/
        parent->remove();
        minNFA = currentNFA;
        if(shapeIsPivot){
            shape->pivotShape = true;
        }
    }
    else if(!shapeIsPivot)/*NFA of current shape is bigger than the NFA
                            of the parent so we remove the currentShape if it's not a pivot*/
        shape->remove();
}
void LsTree::setRemovable(){
    for(int i=0;i<iNbShapes;i++){
        shapes[i].pivotShape = false;
    }
}

