//
//  Contorller.hpp
//  Concraft
//
//  Created by Ninter6 on 2023/5/21.
//

#pragma once

#include <vector>
#include "World.hpp"

class PlayerController : public Controller {
public:
    PlayerController(World* world);
    
    void move(GameObject* object);
    
private:
    World* world;
    
    /**
     * Unix无缓冲输入
     * @return 输入字符
     */
    char scanKeyboard();
};

class ItemController : public Controller {
public:
    ItemController(World* world);
    
    void move(GameObject* object);
    
private:
    World* world;
};
