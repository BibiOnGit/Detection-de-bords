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

/// Find next sibling, taking into account that some shapes are removed
LsShape* LsShape::find_sibling() {
    // First look at the siblings in the original tree
    for(LsShape *s1=sibling,*s2; s1 != 0; s1 = s1->sibling)
        if((s2 = ls_shape_of_subtree(s1)) != 0)
            return s2;
    if(parent == 0 || ! parent->bIgnore)
        return 0; // Parent in original tree is also parent in true tree: done
    return parent->find_sibling();
}

LsShape* LsShape::find_prev_sibling() {
    LsShape* pNext = find_parent();
    if(! pNext)
        return 0;
    pNext = pNext->find_child();
    LsShape* s = 0;
    while(pNext != this) {
        s = pNext;
        pNext = s->find_sibling();
    }
    return s;
}

/// Indicate if \a s is a descendent of \a parent.
bool descendent(LsShape* parent, LsShape* s) {
    while(s!=parent && s->area < parent->area)
        s = s->parent;
    return (s==parent);
}

/// Return closest common ancestor to shapes \a s and \a t.
LsShape* common_ancestor(LsShape* s, LsShape* t) {
    while(s!=t) {
        while(s->area < t->area)
            s = s->find_parent();
        while(t->area < s->area)
            t = t->find_parent();
    }
    return s;
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
        if((s = s->parent) == 0)
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

/// Returns the euclidien length of the level line
double LsShape::length()const{
    double longueur=0;
    for(int i=0;i<contour.size()-1;i++){
        if(contour[i].x==contour[i+1].x ||contour[i].y==contour[i+1].y ){
            longueur++;
        }
        else{
            longueur+=std::sqrt(2);
        }
    }
    if(contour[0].x==contour[contour.size()-1].x ||contour[0].y==contour[contour.size()-1].y ){
        longueur++;
    }
    else{
        longueur+=std::sqrt(2);
    }
    return longueur;

}

int LsShape::childNumber(){
    LsShape* currentChild = find_child();
    if(!currentChild){
        return 0;
    }
    else{
        int counter = 1;
        while(currentChild ->find_sibling()){
            counter++;
            currentChild = currentChild->find_sibling();
        }
        return counter;

    }
}
// Function NFA
void LsShape::NFAk(const int Nll,const double Kpercent,const int *hist,const std::vector<int> &pascTri, const int*grad,const int w, const int h){
    int K = Kpercent*contour.size();
    double l2n = length()/(2*contour.size());
    if(K==0)
        K=1;
    double hc = Hc(MuK(Kpercent,grad,w,h),hist);
    double min = binomiale((K-1) * l2n,contour.size() * l2n,hc,pascTri );
    NFA = Nll * min * K;
}

void LsShape::remove(){
    LsShape* parent = find_parent();
    if(!parent){
        return;
    }
    bIgnore = true;
}


int LsShape::MuK(const double Kpercent, const int *grad, const int w,const int h)const{
    int K = Kpercent*contour.size();
    std::vector<int> gradient_shape;
    int x,y;
    for (int i=0; i<contour.size(); i++){
           x = (contour[i].x==w)?contour[i].x-1:contour[i].x;
           y = (contour[i].y==h)?contour[i].y-1:contour[i].y;
           gradient_shape.push_back(grad[x+w*y]);
    }
    std::sort(gradient_shape.begin(),gradient_shape.end());

    if(K==0){
        return gradient_shape[0];
    }
    return gradient_shape[K-1];
}




