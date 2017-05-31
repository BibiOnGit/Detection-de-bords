#include "drawtree.h"

void drawTree(const LsTree &tree) {
    LsTreeIterator itTree(LsTreeIterator::Pre,&tree.shapes[0]);
    LsTreeIterator endTree =itTree.end(LsTreeIterator::Pre,&tree.shapes[0]);
    noRefreshBegin();
    for(;itTree!=endTree;++itTree){
        LsShape* currentShape = *itTree;
        std::vector<LsPoint>::iterator itShape, endShape=currentShape->contour.end();
        for(itShape=currentShape->contour.begin(); itShape!=endShape-1; ++itShape){
            std::vector<LsPoint>::iterator nextit=itShape+1;
            drawLine(2*itShape->x,2*itShape->y,2*nextit->x,2*nextit->y,BLACK);
        }
        drawLine(2*itShape->x,2*itShape->y,2*currentShape->contour.begin()->x,2*currentShape->contour.begin()->y,BLACK);
    }

    noRefreshEnd();
}
