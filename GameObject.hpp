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
    GameObject(mathpls::vec3 fc, mathpls::vec3 bc, std::string zi, std::string ID = {});
    mathpls::vec3 frontColor, backColor; // 前景色(即字体颜色), 背景色, 全部使用256色(一共256种颜色)
    std::string zi; // 显示在终端上的字
    std::string ID; // built-in-ID
    inline void out() const {
        int fc = 16 + 36*frontColor.r + 6*frontColor.g + frontColor.b;
        int bc = 16 + 36*backColor.r + 6*backColor.g + backColor.b;
        std::ostringstream formatter;
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
    Entity(mathpls::vec3 fc, mathpls::vec3 bc, std::string zi, int health, int damage, std::string ID);
    Entity(mathpls::vec3 fc, mathpls::vec3 bc, std::string zi, mathpls::vec2 pos, int health, int damage, std::string ID = {});
    ~Entity();
    
    mathpls::vec2 pos; // 位置
    int health, damage;
    
    Controller* controller = nullptr;
};

struct ItemAttribute {
    bool placeable;
    std::string placeBlockName;
    bool edible;
    int satietyAdd;
};

class Item : public Entity {
public:
    Item() = default;
    Item(mathpls::vec3 fc, mathpls::vec3 bc, std::string zi, int damage, int hardness, int durability, std::string name, std::string ID = {});
    int hardness, durability; // 硬度和耐久
    std::string name;
};

class Block : public GameObject {
public:
    Block() = default;
    Block(mathpls::vec3 fc, mathpls::vec3 bc, std::string zi, int hd, int db, int imx, int imn, std::string itemName, bool blocked, std::string ID = {});
    int hardness, durability; // 硬度和挖掘需要的时间
    bool blocked;
    int item_min, item_max;
    std::string itemName; // 掉落物
};

class Player : public Entity {
public:
    Player(mathpls::vec2 pos);
    int satiety = 10; // 饱食度
    mathpls::vec2 forward{0, -1};
    std::vector<std::pair<Item, int>> bag;
    short hold = 0;
};

static std::unordered_map<std::string, Item> Items{
    {"stone", {{0, 0, 232}, {0, 0, 0}, "石", 1, 1, -1, "圆石", "stone"}},
    {"diamond", {{0, 4, 5}, {0, 0, 0}, "钻", 1, 1, -1, "钻石", "diamond"}},
    {"gold_ingot", {{5, 4, 0}, {0, 0, 0}, "金", 1, 1, -1, "金锭", "gold_ingot"}},
    {"coal", {{0, 0, 230}, {0, 0, 0}, "煤", 1, 1, -1, "煤", "coal"}},
    {"iron_ingot", {{5, 5, 5}, {0, 0, 0}, "铁", 1, 1, -1, "铁锭", "iron_ingot"}},
    {"leave", {{1, 5, 1}, {0, 0, 0}, "叶", 1, 1, -1, "树叶", "leave"}},
    {"wood", {{5, 2, 0}, {0, 0, 0}, "木", 1, 1, -1, "木头", "wood"}},
    {"apple", {{5, 0, 0}, {0, 0, 0}, "果", 1, 1, -1, "苹果", "apple"}},
    {"sapling", {{2, 5, 2}, {0, 0, 0}, "苗", 1, 1, -1, "树苗", "sapling"}},
    {"plank", {{3, 1, 0}, {5, 2, 0}, "板", 1, 1, -1, "木板", "plank"}},
    {"stick", {{5, 2, 0}, {0, 0, 0}, "棍", 1, 1, -1, "木棍", "stick"}},
    {"wooden_pickaxe", {{5, 2, 0}, {0, 0, 0}, "镐", 3, 2, 60, "木镐", "wooden_pickaxe"}},
    {"stone_pickaxe", {{0, 0, 232}, {0, 0, 0}, "镐", 4, 3, 132, "石镐", "stone_pickaxe"}},
    {"iron_pickaxe", {{0, 0, 232}, {0, 0, 0}, "镐", 5, 4, 251, "铁镐", "iron_pickaxe"}},
    {"diamond_pickaxe", {{0, 4, 5}, {0, 0, 0}, "镐", 6, 5, 1562, "钻镐", "diamond_pickaxe"}},
};

static std::unordered_map<std::string, Block> Blocks{
    {"air", {{0, 0, 0}, {0, 0, 0}, "  ", 0, 0, 0, 0, {}, 0, "air"}},
    {"stone", {{0, 0, 0}, {0, 0, 232}, "  ", 2, 3, 1, 1, "stone", 1, "stone"}},
    {"diamond_ore", {{0, 4, 5}, {0, 0, 232}, "钻", 4, 5, 1, 1, "diamond", 1, "diamond_ore"}},
    {"gold_ore", {{5, 4, 0}, {0, 0, 232}, "金", 3, 3, 1, 1, "gold_ingot", 1, "gold_ore"}},
    {"coal_ore", {{0, 0, 0}, {0, 0, 232}, "煤", 2, 3, 1, 3, "coal", 1, "coal_ore"}},
    {"iron_ore", {{4, 3.2, 2}, {0, 0, 232}, "铁", 3, 3, 1, 1, "iron_ingot", 1, "iron_ore"}},
    {"leave", {{1, 5, 1}, {0, 0, 0}, "叶", 0, 1, 0, 0, "leave", 0, "leave"}},
    {"wood", {{5, 3, 0}, {3, 1, 0}, "木", 1, 3, 1, 5, "wood", 1, "wood"}},
    {"sapling", {{2, 5, 2}, {5, 2, 0}, "苗", 1, 1, 1, 1, "sapling", 0, "sapling"}},
    {"plank", {{5, 2, 0}, {3, 1, 0}, "板", 1, 3, 1, 1, "plank", 1, "plank"}},
};

static std::unordered_map<std::string, ItemAttribute> ItemAttributes{
    {"stone", {true, "stone", false}},
    {"diamond", {false, {}, false}},
    {"gold", {false, {}, false}},
    {"coal", {false, {}, false}},
    {"iron_ingot", {false, {}, false}},
    {"leave", {true, "leave", false}},
    {"wood", {true, "wood", false}},
    {"apple", {false, {}, true, 2}},
    {"sapling", {true, "sapling", false}},
    {"plank", {true, "plank", false}},
    {"stick", {false, {}, false}},
    {"wooden_pickaxe", {false, {}, false}},
    {"stone_pickaxe", {false, {}, false}},
    {"iron_pickaxe", {false, {}, false}},
    {"diamond_pickaxe", {false, {}, false}},
};

struct CraftList {
    std::string product;
    int num;
    std::vector<std::pair<std::string, int>> materials;
};

static std::vector<CraftList> CraftLists{
    {"plank", 4, {{"wood", 1}}},
    {"stick", 4, {{"plank", 2}}},
    {"wooden_pickaxe", 1, {{"plank", 3}, {"stick", 2}}},
    {"stone_pickaxe", 1, {{"stone", 3}, {"stick", 2}}},
    {"iron_pickaxe", 1, {{"iron_ingot", 3}, {"stick", 2}}},
    {"diamond_pickaxe", 1, {{"diamond", 3}, {"stick", 2}}},
};
