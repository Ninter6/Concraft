//
//  main.cpp
//  Concraft
//
//  Created by Ninter6 on 2023/5/20.
//

#include <iostream>
#define MATHPLS_DIFINITION
#include "mathpls.h"

#include "World.hpp"

int main(int argc, const char * argv[]) {
    World world(30, 30, "114514");
    world.Play();
//    std::ios::sync_with_stdio(false);
//    for(int r = 0; r < 6; r++)
//        for(int g = 0; g < 6; g++)
//            for(int b = 0; b < 6; b++){
//                std::stringstream formatter;
//                formatter << r << g << b << ": " << "\033[38;5;"<<16+36*r+6*g+b<<"m" << "Hello world!" << "\033[0m" << endl;
//                std::cout << formatter.str();
//            }
    return 0;
}
