#include "shape.h"
#include <cassert>

/// Return in the subtree of root pShape a shape that is not removed
static LsShape* ls_shape_of_subtree(LsShape* pShape) {
    LsShape* pShapeNotRemoved = 0;
    if(pShape == 0 || ! pShape->bIgnore)
        return pShape;
    for(pShape = pShape->child; pShape != 0; pShape = pShape->sibling)
        if((pShapeNotRemoved = ls_shape_of_subtree(pShape)) != 0)
            break;
    return pShapeNotRemoved;
}

/// To find the true parent, that is the greatest non removed ancestor
LsShape* LsShape::find_parent() {
    LsShape* pShape = this;
    do
        pShape = pShape->parent;
    while(pShape && pShape->bIgnore);
    return pShape;
}

/// Find the first child, taking into account that some shapes are removed
LsShape* LsShape::find_child() {
    LsShape* pShapeNotRemoved = 0;
    for(LsShape* pShape = child; pShape; pShape = pShape->sibling)
        if((pShapeNotRemoved = ls_shape_of_subtree(pShape)) != 0)
            break;
    return pShapeNotRemoved;  
}


/// Returns the floor of the euclidien length of the level line
int LsShape::length(){
    float longueur=0;
    for(int i=0;i<contour.size()-1;i++){
        if(contour[i].x==contour[i+1].x ||contour[i].y==contour[i+1].y ){
            longueur=longueur+1;
        }
        else{
            longueur=longueur+std::sqrt(2);
        }

    }
    if(contour[0].x==contour[contour.size()-1].x ||contour[0].y==contour[contour.size()-1].y ){
        longueur=longueur+1;
    }
    else{
        longueur=longueur+std::sqrt(2);
    }
    return int(longueur);

}

/// Find next sibling, taking into account that some shapes are removed
LsShape* LsShape::find_sibling() {
    LsShape *pShape1 = 0, *pShape2 = 0;
    // First look at the siblings in the original tree
    for(pShape1 = sibling; pShape1 != 0; pShape1 = pShape1->sibling)
        if((pShape2 = ls_shape_of_subtree(pShape1)) != 0)
            return pShape2;
    if(parent == 0 || ! parent->bIgnore)
        return 0; // Parent in original tree is also parent in true tree: done
    // Not found: find node in original tree just before the true parent
    LsShape* pShape = this;
    do
        pShape = pShape->parent;
    while(pShape->parent->bIgnore);
    // Look at the siblings of this node
    for(pShape1 = pShape->sibling; pShape1; pShape1 = pShape1->sibling)
        if((pShape2 = ls_shape_of_subtree(pShape1)) != 0)
            return pShape2;
    return 0;
}

LsShape* LsShape::find_prev_sibling() {
    LsShape* pNext = find_parent();
    if(! pNext)
        return 0;
    pNext = pNext->find_child();
    LsShape* s = 0;
    std::cout << "find_prev_sibling" <<std::endl;
    while(pNext != this) {
        s = pNext;
        pNext = s->find_sibling();
    }
    return s;
}


int LsShape::childNumber(){
    if(find_child() ==0){
        return 0;
    }
    else{
        LsShape* currentChild = find_child();
        int counter = 1;
        while(currentChild ->find_sibling() !=0){
            counter++;
            currentChild = currentChild->find_sibling();
        }

    }
}
void LsShape::setShape(LsShape* parent, LsShape* child, LsShape* sibling){
    this->parent =parent;
    this->child = child;
    this->sibling = sibling;
}


LsTreeIterator::LsTreeIterator(Order ord, LsShape* shape, bool /*dummy*/)
: s(shape), o(ord) {}

LsTreeIterator LsTreeIterator::end(Order ord, LsShape* s) {
    LsTreeIterator it(ord, s, true);
    if(s && !s->bIgnore) {
        if(ord == Pre)
            it.s = uncle(s);
        else // (ord == Post)
            ++it;
    }
    return it;
}

LsShape* LsTreeIterator::go_bottom(LsShape* s) {
    for(LsShape* t = s->find_child(); t; t = s->find_child())
        s = t;
    return s;
}

LsShape* LsTreeIterator::uncle(LsShape* s) {
    LsShape* sNew;
    while((sNew = s->find_sibling()) == 0)
        if((s = s->find_parent()) == 0)
            break;
    return sNew;
}

LsTreeIterator& LsTreeIterator::operator++() {
    if(o == Pre) {
        LsShape* sNew = s->find_child();
        s = (sNew == 0)? uncle(s): sNew;
    } else { // (o == Post)
        LsShape* sNew = s->find_sibling();
        s = (sNew == 0)? s->find_parent(): go_bottom(sNew);
    }
    return *this;
}

// Function NFA
void LsShape::NFAk(int Nll, float Kpercent, double Hc){
    int K = Kpercent*contour.size();
    double l2n = double(length())/(2*double(contour.size()));
//    std::cout << "K "<<int((K-1) * l2n)<<" n " << int(contour.size() * l2n) << std::endl;
    double min = binomiale(int((K-1) * l2n),int(contour.size() * l2n), Hc );
    NFA =  Nll * min * K;
}

void LsShape::remove(){
    LsShape* parent = find_parent();
    if(! parent){
        return;
    }
    bIgnore = true;
}

//void LsShape::remove(){
//    LsShape* parent = find_parent();
//    if(! parent){
//        return;
//    }
//        std::cout << "test_remove1" <<std::endl;
//    LsShape* previous_sibling = find_prev_sibling();
//    if (previous_sibling){
//        std::cout << "test_remove2" <<std::endl;
//        if(this->child){
//            previous_sibling->sibling=this->child;
//        }
//        else if(this->sibling){
//            previous_sibling->sibling=this->sibling;
//        }
//        else{
//            previous_sibling->sibling =0;
//        }

//    }
//    else{
//        if (this->child){
//            std::cout << "test_remove3" <<std::endl;
//            this->parent->child=this->child;
//        }
//        else{
//            std::cout << "test_remove4" <<std::endl;
//            this->parent->child=this->sibling;
//        }
//    }

//    LsShape* brother = this->child;
//    LsShape* prev_child = 0;
//    while (brother){
//        brother->parent=this->parent;
//        prev_child=brother;
//        brother=brother->sibling;
//    }
//    if (this->sibling){
//        if (prev_child){
//            prev_child->sibling=this->sibling;
//        }
//    }
//}

unsigned char LsShape::MuK(float Kpercent, unsigned char * grad, int w){
    int K = Kpercent*contour.size();
    std::vector<unsigned char> gradient_shape;
    for (int i=0; i<contour.size(); i++){
        gradient_shape.push_back(grad[contour[i].x+w*contour[i].y]);
    }
    std::sort(gradient_shape.begin(),gradient_shape.end());
    if(K==0){
        return gradient_shape[0];
    }
    return gradient_shape[K-1];
}



void LsShape::MeanB(int Nll, double epsilon, float Kpercent, unsigned char * grad, int w, int hist[]){
    unsigned char Mu = MuK(Kpercent,grad,w);
    NFAk(Nll,Kpercent,Hc(Mu,hist));
    if (NFA>epsilon){
        remove();
    }

    if (this->find_child()){
        LsShape* child = this->find_child();
        child->MeanB(Nll,epsilon,Kpercent,grad,w,hist);
    }
    if (this->find_sibling()){
        LsShape* sibling = this->find_sibling();
        sibling->MeanB(Nll,epsilon,Kpercent,grad,w,hist);
    }
}






