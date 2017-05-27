#include "drawtree.h"

void drawTree(const LsTree &tree) {
    for(int i=0; i<tree.iNbShapes;i++){
        LsShape currentShape = tree.shapes[i];
        if(!currentShape.bIgnore){
            noRefreshBegin();
            std::vector<LsPoint>::iterator itShape, endShape=currentShape.contour.end();
            for(itShape=currentShape.contour.begin(); itShape!=endShape; ++itShape)
                drawPoint(itShape->x, itShape->y, BLACK);
            noRefreshEnd();
        }
    }
//    LsTreeIterator itTree(LsTreeIterator::Pre,&tree.shapes[0]);
//    LsTreeIterator endTree =itTree.end(LsTreeIterator::Pre,&tree.shapes[0]);
//    for(;itTree!=endTree;++itTree){
//        LsShape* currentShape = *itTree;
//        noRefreshBegin();
//        std::vector<LsPoint>::iterator itShape, endShape=currentShape->contour.end();
//        for(itShape=currentShape->contour.begin(); itShape!=endShape; ++itShape)
//            drawPoint(itShape->x, itShape->y, BLACK);
//        noRefreshEnd();
//    }
}
