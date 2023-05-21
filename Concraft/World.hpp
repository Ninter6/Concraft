//
//  World.hpp
//  Concraft
//
//  Created by Ninter6 on 2023/5/20.
//

#pragma once

#include <iostream>
#include <vector>

#include "GameObject.hpp"
#include "Noise2D.hpp"

using namespace std;

class World {
public:
    World(int width, int height, string seed);
    ~World();
    
    vector<vector<Block>> map;
    vector<unique_ptr<Entity>> entities;
    vector<vector<unique_ptr<Entity>>::iterator> kill;
    
    string seed;
    
    void Play();
    
    unsigned short time = 0;
    
    default_random_engine _E;
    
private:
    void CreateWorld(int width, int height, string seed);
    void CreateMountains(int seed);
    void CreateOres(int seed);
    void CreateTrees(int seed);
    void CreateEntities(int seed);
    
    void PlantOneTree(int x, int y);
    
    bool isLeaveLossRoot(mathpls::vec2 location);
};
