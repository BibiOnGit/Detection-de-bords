#include "drawtree.h"

void drawTree(const LsTree &tree) {
    LsTreeIterator itTree(LsTreeIterator::Pre,&tree.shapes[0]);
    LsTreeIterator endTree =itTree.end(LsTreeIterator::Pre,&tree.shapes[0]);
    noRefreshBegin();
    for(;itTree!=endTree;++itTree){
        LsShape* currentShape = *itTree;
        std::vector<LsPoint>::iterator itShape, endShape=currentShape->contour.end();
        for(itShape=currentShape->contour.begin(); itShape!=endShape; ++itShape)
            drawPoint(itShape->x, itShape->y, BLACK);
    }
    noRefreshEnd();
}
