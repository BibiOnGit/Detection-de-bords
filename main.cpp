
#include <Imagine/Images.h>
#include "tree.h"
#include "utils.h"

#include "histogramme.h"
#include <iostream>
using namespace Imagine;

int main(int argc, char* argv[]) {
    if(argc!=2) {
        std::cerr << "Usage: " << argv[0] << " imageFile" << std::endl;
        return 1;
    }
    Image<byte> im;
    if(! load(im, argv[1])) {
        std::cerr << "Error loading image " << argv[1] << std::endl;
        return 1;
    }
    openWindow(im.width(), im.height());
    display(im);

    click();

    LsTree tree(im.data(), im.width(), im.height());

    std::cout << tree.shapes[0].gray << std::endl;

    endGraphics();
    return 0;
}
