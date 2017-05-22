#ifndef TREE_H
#define TREE_H

#include "shape.h"
#include "histogramme.h"


/// Tree of shapes.
struct LsTree {
    LsTree(LsShape* shapes);
    LsTree(const unsigned char* gray, int w, int h);
    ~LsTree();

    unsigned char* build_image() const;
    LsShape* smallest_shape(int x, int y);

    int ncol, nrow; ///< Dimensions of image
    LsShape* shapes; ///< The array of shapes
    int iNbShapes; ///< The number of shapes

    /// For each pixel, the smallest shape containing it
    LsShape** smallestShape;


    void MeanB(int Nll, double epsilon, float Kpercent, unsigned char * grad, int w, int hist[]);

    void maxMeaningfulBoundaries();
    ///Keep only the maximal meaningful boundaries among the meaningful boundaries
    ///by browsing the tree of meaningful boundaries and removing the non maximal

private:
    enum Monotony{INCREASING, DECREASING, UNDETERMINED, NOT_MONOTONE};
    ///Different cases of monotony in a tree:
    ///INCREASING = increasing level of grey
    ///DECREASING = decreasing level of grey
    ///UNDETERMINED = we don't know if the level of grey is increasing or decreasing at the beginning of a monotone section
    ///NOT_MONOTONE = we aren't in a monotone section

    void monotoneSectionManager(LsShape *shape,Monotony &monotony,short int previousGrey,double minNFA,
                                double currentNFA, int childNumber);
    ///Manage the tricky case of a shape situated in a monotone section

    void minNfaShapeKeeper(LsShape *shape, double currentNFA, double &minNFA, bool shapeIsPivot);
    ///Remove the shape(if it isn't a pivot) which has the bigger NFA between the current shape and his parent

    void setRemovable();
    ///Set the attribute "removable" to true of every shape of the tree

    void flst_td(const unsigned char* gray); ///< Top-down algo


};




#endif
