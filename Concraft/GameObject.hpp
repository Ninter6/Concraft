//
//  GameObject.hpp
//  Concraft
//
//  Created by Ninter6 on 2023/5/20.
//

#pragma once

#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include "mathpls.h"

class GameObject {
public:
    GameObject() = default;
    GameObject(mathpls::vec3 fc, mathpls::vec3 bc, std::string zi);
    mathpls::vec3 frontColor, backColor; // 前景色(即字体颜色), 背景色, 全部使用256色(一共256种颜色)
    std::string zi; // 显示在终端上的字
    inline void out() const {
        int fc = 16 + 36*frontColor.r + 6*frontColor.g + frontColor.b;
        int bc = 16 + 36*backColor.r + 6*backColor.g + backColor.b;
        std::stringstream formatter;
        formatter << "\033[38;5;"<<fc<<"m" << "\033[48;5;"<<bc<<"m" << zi << "\033[0m";
        std::cout << formatter.str();
    }
    
    bool operator==(GameObject& obj){return (frontColor == obj.frontColor && backColor == obj.backColor && zi == obj.zi);}
    bool operator!=(GameObject& obj){return !(*this == obj);}
};

/**
 * 注意
 * 实体控制器父类
 * 在这里
 */
class Controller {
public:
    virtual void move(GameObject* object) {}
};

class Entity : public GameObject {
public:
    Entity() = default;
    Entity(mathpls::vec3 fc, mathpls::vec3 bc, std::string zi, int health, int damage);
    Entity(mathpls::vec3 fc, mathpls::vec3 bc, std::string zi, mathpls::vec2 pos, int health, int damage);
    ~Entity();
    
    mathpls::vec2 pos; // 位置
    int health, damage;
    
    Controller* controller = nullptr;
};

class Item : public Entity {
public:
    Item() = default;
    Item(mathpls::vec3 fc, mathpls::vec3 bc, std::string zi, int damage, int hardness, int durability, std::string name);
    int hardness, durability; // 硬度和耐久
    std::string name;
};

class Block : public GameObject {
public:
    Block() = default;
    Block(mathpls::vec3 fc, mathpls::vec3 bc, std::string zi, int hd, int db, int imx, int imn, Entity item, bool blocked = true);
    int hardness, durability; // 硬度和挖掘需要的时间
    bool blocked;
    int item_min, item_max;
    Entity item; // 掉落物
};

class Player : public Entity {
public:
    Player(mathpls::vec2 pos);
    int satiety = 10; // 饱食度
    mathpls::vec2 forward{0, -1};
    std::vector<std::pair<Item, int>> bag;
};

static std::unordered_map<std::string, Item> Items{
    {"stone", {{0, 0, 232}, {0, 0, 0}, "石", 1, 0, -1, "圆石"}},
    {"diamond", {{0, 4, 5}, {0, 0, 0}, "钻", 1, 0, -1, "钻石"}},
    {"gold", {{5, 4, 0}, {0, 0, 0}, "金", 1, 0, -1, "金锭"}},
    {"coal", {{0, 0, 0}, {0, 0, 0}, "煤", 1, 0, -1, "煤"}},
    {"iron_ore", {{4, 3.2, 2}, {0, 0, 0}, "铁", 1, 0, -1, "铁矿"}},
    {"iron_ore", {{5, 5, 5}, {0, 0, 0}, "铁", 1, 0, -1, "铁锭"}},
    {"leave", {{1, 5, 1}, {0, 0, 0}, "叶", 1, 0, -1, "树叶"}},
    {"wood", {{5, 2, 0}, {0, 0, 0}, "木", 1, 0, -1, "木头"}},
    {"apple", {{5, 0, 0}, {0, 0, 0}, "果", 1, 0, -1, "苹果"}},
    {"sapling", {{2, 5, 2}, {0, 0, 0}, "苗", 1, 0, -1, "树苗"}},
};

static std::unordered_map<std::string, Block> Blocks{
    {"air", {{0, 0, 0}, {0, 0, 0}, "  ", 0, 0, 0, 0, {}, false}},
    {"stone", {{0, 0, 0}, {0, 0, 232}, "  ", 1, 3, 1, 1, Items["stone"]}},
    {"diamond_ore", {{0, 4, 5}, {0, 0, 232}, "钻", 4, 5, 1, 1, Items["diamond_ore"]}},
    {"gold_ore", {{5, 4, 0}, {0, 0, 232}, "金", 3, 3, 1, 1, Items["gold_ore"]}},
    {"coal_ore", {{0, 0, 0}, {0, 0, 232}, "煤", 1, 3, 1, 3, Items["coal_ore"]}},
    {"iron_ore", {{4, 3.2, 2}, {0, 0, 232}, "铁", 2, 3, 1, 1, Items["iron_ore"]}},
    {"leave", {{1, 5, 1}, {0, 0, 0}, "叶", 0, 1, 0, 0, Items["leave"], false}},
    {"wood", {{5, 2, 0}, {0, 0, 0}, "木", 1, 3, 1, 5, Items["wood"]}},
};
