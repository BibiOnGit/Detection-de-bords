#include "drawtree.h"

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
