//
//  World.hpp
//  Concraft
//
//  Created by Ninter6 on 2023/5/20.
//

#pragma once

#include <iostream>
#include <vector>
#include <queue>

#include "GameObject.hpp"
#include "Noise2D.hpp"
#include "StaticRandom.hpp"

using namespace std;

inline void clear(){
    system("clear");
}

class World {
public:
    World(int width, int height, string seed);
    World(string path);
    ~World();
    
    vector<vector<Block>> map;
    vector<unique_ptr<Entity>> entities;
    priority_queue<uint32_t> kill;
    
    string seed;
    
    void Play();
    
    unsigned short time = 0;
    
    /**
     * 召唤一个已经预设的实体(或物品)
     * @param pos 位置
     * @param name 实体名称(⚠️物品要在开头加"item_"⚠️)
     */
    void SummonOneEntity(mathpls::vec2 pos, string name);
    
    void saveToLocal();
    
private:
    void CreateWorld(int width, int height, string seed);
    void CreateMountains(int seed);
    void CreateOres(int seed);
    void CreateTrees(int seed);
    void CreateEntities(int seed);
    
    void PlantOneTree(int x, int y);
    
    bool isLeaveLossRoot(mathpls::vec2 location);
};
