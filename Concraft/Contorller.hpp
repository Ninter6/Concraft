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
    
    /**
     * 使用物品
     * @param player 玩家指针
     */
    void useItems(Player* player);
    
    bool keepDigging = false;
    int diggingTime = 0;
    /**
     * 破坏方块
     * @param player 玩家指针
     */
    void destroyBlock(Player* player);
    
    void bagMenu(Player* player);
    vector<CraftList> searchCraftLists(const vector<std::pair<Item, int>>& bag);
};

class ItemController : public Controller {
public:
    ItemController(World* world);
    
    void move(GameObject* object);
    
private:
    World* world;
};
