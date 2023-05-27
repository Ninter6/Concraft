//
//  GameObject.cpp
//  Concraft
//
//  Created by Ninter6 on 2023/5/20.
//

#include "GameObject.hpp"

GameObject::GameObject(mathpls::vec3 fc, mathpls::vec3 bc, std::string zi, std::string ID) : frontColor(fc), backColor(bc), zi(zi), ID(ID) {}

Block::Block(mathpls::vec3 fc, mathpls::vec3 bc, std::string zi, int hd, int db, int imx, int imn, std::string itemName, bool blocked, std::string ID) :
GameObject(fc, bc, zi, ID), hardness(hd), durability(db), item_max(imx), item_min(imn), itemName(itemName), blocked(blocked)
{}

Entity::Entity(mathpls::vec3 fc, mathpls::vec3 bc, std::string zi, int health, int damage, std::string ID) :
GameObject(fc, bc, zi, ID), health(health), damage(damage) {}

Entity::Entity(mathpls::vec3 fc, mathpls::vec3 bc, std::string zi, mathpls::vec2 pos, int health, int damage, std::string ID) :
GameObject(fc, bc, zi, ID), pos(pos), health(health), damage(damage) {}

Entity::~Entity(){
    
}

Item::Item(mathpls::vec3 fc, mathpls::vec3 bc, std::string zi, int damage, int hardness, int durability, std::string name, std::string ID) :
Entity(fc, bc, zi, -1, damage, ID), hardness(hardness), durability(durability), name(name) {}

Player::Player(mathpls::vec2 pos) : Entity({5, 1, 1}, {0, 0, 0}, "AA", pos, 10, 1) {}
