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
    ios::sync_with_stdio(false);
    cout<<"请选择..."<<endl<<"1.新建世界"<<endl<<"2.加载存档"<<endl;
    int opt;
    cin >> opt;
    switch (opt) {
        case 1:{
            cout<<"请输入种子: ";
            string seed;
            cin>>seed;
            cout<<"请输入世界大小(100~500):"<<endl;
            int x, y;
            cout<<"长: ";
            cin>>x;
            cout<<"宽: ";
            cin>>y;
            x = mathpls::mid(100, x, 500);
            y = mathpls::mid(100, y, 500);
            World world(x, y, seed);
            world.Play();
            break;
        }
            
        case 2:{
            cout<<"请输入存档路径: ";
            string path;
            cin>>path;
            World world(path);
            world.Play();
            break;
        }
            
        default:
            cout<<"你在干什么啊?"<<endl;
            break;
    }
//    for(int r = 0; r < 6; r++)
//        for(int g = 0; g < 6; g++)
//            for(int b = 0; b < 6; b++){
//                std::stringstream formatter;
//                formatter << r << g << b << ": " << "\033[38;5;"<<16+36*r+6*g+b<<"m" << "Hello world!" << "\033[0m" << endl;
//                std::cout << formatter.str();
//            }
//    for(int i=0;i<256;i++){
//        std::stringstream formatter;
//        formatter << i << ": " << "\033[38;5;"<<i<<"m" << "Hello world!" << "\033[0m" << endl;
//        std::cout << formatter.str();
//    }
    return 0;
}
