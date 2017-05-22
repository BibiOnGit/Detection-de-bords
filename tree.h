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
    ///
    void drawTree();

private:
    enum Monotony{INCREASING, DECREASING, UNDETERMINED, NOT_MONOTONE};

    void maxMeaningfulBoundaries_rec(LsShape *shape, Monotony monotony, double minNFA,
                                     short int previousGrey);
    ///Recursive function which browses the tree of meaningful shape and remove the non maximal

    void monotoneSectionManager(LsShape *shape,Monotony &monotony,short int previousGrey,double minNFA,
                                double currentNFA, int childNumber);
    ///Manage the tricky case of a shape situated in a monotone section

    void minNfaShapeKeeper(LsShape *shape, double currentNFA, double &minNFA, bool shapeIsPivot);
    ///Remove the shape(if it isn't a pivot) which has the bigger NFA between the current shape and his parent

    void flst_td(const unsigned char* gray); ///< Top-down algo

    void setRemovable();


};




#endif
