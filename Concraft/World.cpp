//
//  World.cpp
//  Concraft
//
//  Created by Ninter6 on 2023/5/20.
//

#include "mathpls.h"

inline bool operator<(mathpls::vec2 v1, mathpls::vec2 v2){
    if (v1.x < v2.x)return true;
    if (v1.x > v2.x)return false;
    if (v1.y < v2.y)return true;
    return false;
}

#include "World.hpp"
#include "Contorller.hpp"

#include <queue>
#include <set>
#include <iomanip>
#include <fstream>

World::World(int width, int height, string seed) : seed(seed){
    CreateWorld(width, height, seed);
}

World::World(string path){
    ifstream ifs(path);
    int width, height;
    ifs >> seed >> width >> height;
    
    map.resize(height);
    for(auto& i : map){
        i.resize(width);
        for(auto& block : i){
            string ID;
            ifs >> ID;
            block = Blocks[ID];
        }
    }
    
    CreateEntities(static_cast<int>(hash<string>{}(seed)));
    
    Player* player = reinterpret_cast<Player*>(entities[0].get());
    ifs >> player->health >> player->satiety >> player->pos.x >> player->pos.y >> player->forward.x >> player->forward.y;
    int bagSize;
    ifs >> bagSize;
    player->bag.resize(bagSize);
    for(auto& i : player->bag){
        string ID;
        int durability, num;
        ifs >> ID >> durability >> num;
        i.first = Items[ID];
        i.first.durability = durability;
        i.second = num;
    }
}

World::~World(){
    
}

void World::CreateWorld(int width, int height, string seed){
    cout<<"正在生成世界..."<<endl;
    map.resize(height);
    for(auto& i : map){
        i.resize(width);
        for(auto& obj : i){
            obj = Blocks["air"];
        }
    }
    uint32_t iseed = static_cast<uint32_t>(hash<string>{}(seed));
    
    cout<<"正在生成山脉..."<<endl;
    CreateMountains(iseed);
    cout<<"正在生成矿石..."<<endl;
    CreateOres(iseed);
    cout<<"正在生成树木..."<<endl;
    CreateTrees(iseed);
    cout<<"正在生成实体..."<<endl;
    CreateEntities(iseed);
}

void World::CreateMountains(int seed){
    NoiseSet2D m(seed);  // hash code from string todo
    m.addNoise(50, 20);
    m.addNoise(5, 25);
    m.addNoise(5, 5);
    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            if (m.getHeight(x, y) > 10) {
                map[y][x] = Blocks["stone"];
            }
        }
    }
    return;
}

void World::CreateOres(int seed){
    NoiseSet2D ironNoise(seed);
    ironNoise.addNoise(4, 20);
    ironNoise.addNoise(5, 25);
    ironNoise.addNoise(4, 5);
    NoiseSet2D coalNoise(seed);
    coalNoise.addNoise(5, 20);
    coalNoise.addNoise(5, 25);
    coalNoise.addNoise(5, 5);
    NoiseSet2D goldNoise(seed);
    goldNoise.addNoise(5, 10);
    goldNoise.addNoise(2, 10);
    goldNoise.addNoise(3, 10);
    NoiseSet2D diamondsNoise(seed);
    diamondsNoise.addNoise(5, 10);
    diamondsNoise.addNoise(2, 10);
    diamondsNoise.addNoise(3, 10);
    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            if (map[y][x] == Blocks["stone"] && diamondsNoise.getHeight(x, y) > 10) {
                map[y][x] = Blocks["diamond_ore"];
            }
            if (map[y][x] == Blocks["stone"] && goldNoise.getHeight(x, y) > 10) {
                map[y][x] = Blocks["gold_ore"];
            }
            if (map[y][x] == Blocks["stone"] && coalNoise.getHeight(x, y) > 10) {
                map[y][x] = Blocks["coal_ore"];
            }
            if (map[y][x] == Blocks["stone"] && ironNoise.getHeight(x, y) > 8) {
                map[y][x] = Blocks["iron_ore"];
            }
        }
    }
}

void World::CreateTrees(int seed){
    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            if (map[y][x] == Blocks["air"] && percentage(5)) {
                PlantOneTree(x, y);
            }
        }
    }
}

void World::CreateEntities(int seed){
    entities.push_back(make_unique<Player>(mathpls::vec2{static_cast<float>(map[0].size()/2), static_cast<float>(map.size()/2)}));
    entities[0]->controller = new PlayerController(this);
}

void World::PlantOneTree(int x, int y){
    map[y][x] = Blocks["wood"];
    // 同时生长树叶
    int r = randint(3) + 1;  // 123
    for (int leaveY = max(0, y - r); leaveY <= min((int) map.size() - 1, y + r); leaveY++) {
        for (int leaveX = max(0, x - r); leaveX <= min((int) map[y].size() - 1, x + r); leaveX++) {
            if (map[leaveY][leaveX].ID == "air") {
                map[leaveY][leaveX] = Blocks["leave"];
            }
        }
    }
}

bool World::isLeaveLossRoot(mathpls::vec2 location) {
    // bfs周围5距离内是否有树木或叶子，如果没有则死亡
    queue<pair<mathpls::vec2, int>> q;
    set<mathpls::vec2> vecSet{location};
    q.push({location, 0});
    while (!q.empty()) {
        auto loc = q.front().first;
        auto r = q.front().second;
        q.pop();
        Block& o = map[loc.y][loc.x];
        if (r > 3)
            continue;
        if (o != Blocks["wood"] && o != Blocks["leave"])
            continue;
        if (o == Blocks["wood"])
            return false;
        else {
            for (int i = -1; i<2; i++) {
                for (int j = -1; j<2; j++){
                    if(abs(i+j) != 1) continue;
                    mathpls::vec2 roundLoc = loc + mathpls::vec2(i, j);
                    if (vecSet.count(roundLoc) != 0 || roundLoc.x < 0 || roundLoc.x >= map[0].size() || roundLoc.y < 0 || roundLoc.y >= map.size())
                        continue;
                    q.push(make_pair(roundLoc, r + 1));
                    vecSet.insert(roundLoc);
                }
            }
        }
    }
    return true;
}

void World::SummonOneEntity(mathpls::vec2 pos, string name){
    if (name.find("item_")) {
        // todo
    } else {
        entities.push_back(make_unique<Item>(Items[name.substr(5, name.length())]));
        entities.back()->controller = new ItemController(this);
        entities.back()->pos = pos;
    }
}

void World::Play(){
    while (entities[0]->health > 0) {
        clear();
        mathpls::ivec2 o{mathpls::mid<int>(0, entities[0]->pos.x - 7, (int)map[0].size()-15), mathpls::mid<int>(0, entities[0]->pos.y - 7, (int)map.size()-15)};
        for(int y = o.y; y < o.y + 15; y++){
            for(int x = o.x; x < o.x + 15; x++){
                // 销毁悬浮树叶
                if(map[y][x].ID == "leave" && isLeaveLossRoot(mathpls::vec2(x, y))){
                    // 有20%的几率销毁
                    if (percentage(20)) {
                        map[y][x] = Blocks["air"];
                        // 销毁之后 可能产生树苗、果子
                        if (percentage(10)) {
                            entities.push_back(make_unique<Item>(Items["apple"]));
                            entities.back()->controller = new ItemController(this);
                            entities.back()->pos = mathpls::vec2(x, y);
                        } else if (percentage(20)) {
                            entities.push_back(make_unique<Item>(Items["sapling"]));
                            entities.back()->controller = new ItemController(this);
                            entities.back()->pos = mathpls::vec2(x, y);
                        }
                    }
                }
                // 树苗生长
                if(map[y][x].ID == "sapling" && percentage(1)){
                    int an = 0;
                    for(int i = -1; i < 2; i++)
                        for(int j = -1; j < 2; j++)
                            if(map[y+i][x+j].ID == "air") an++;
                    if(an > 4) PlantOneTree(x, y);
                }
                
                bool drawBlock = true;
                for(auto& i : entities)
                    if (i->pos == mathpls::vec2(x, y)) {
                        i->out();
                        drawBlock = false;
                        break;
                    }
                if(drawBlock)
                    map[y][x].out();
            }
            endl(cout);
        }
        
        cout<<"=============================="<<endl;
        cout << "血: ";
        for(int i=0;i<entities[0]->health;i++) cout << "Δ";
        endl(cout);
        cout << "饱：";
        for(int i=0;i<reinterpret_cast<Player*>(entities[0].get())->satiety;i++) cout << "●";
        const auto& bag = reinterpret_cast<Player*>(entities[0].get())->bag;
        cout << endl << "包: 空手 ";
        for(int i = 0; i < mathpls::min<uint64_t>(bag.size(), 8);i++){
            cout<<setw(2 + (int)(bag[i].first.name.size()>4 ? 2 : bag[i].first.name.size()))<<setfill(' ')<<bag[i].first.name<<" ";
        }
        cout<<endl<<"    ";
        for(int i = 0; i <= mathpls::min<uint64_t>(bag.size(), 8); i++){
            if(i && bag[i-1].second>1){
                cout<<setw(2)<<setfill('0')<<mathpls::min(bag[i-1].second, 99);
            } else {
                cout<<"  ";
            }
            if(i == reinterpret_cast<Player*>(entities[0].get())->hold){
                cout<<" ↑ ";
            } else {
                cout<<"   ";
            }
        }
        
        // ⚠️这个循环必须用这种形式, 直接用迭代器会出问题⚠️
        for(int i = 0; i < entities.size(); i++)
            if(entities[i]->controller != nullptr)
                entities[i]->controller->move(entities[i].get());
        
        while(!kill.empty()){
            entities.erase(entities.begin() + kill.top());
            kill.pop();
        }
        
//        cout<<endl<<entities[0]->pos.x<<" "<<entities[0]->pos.y;
    }
    clear();
    cout<<"你失败了..."<<endl<<"游戏结束"<<endl<<endl;
}

void World::saveToLocal(){
    ofstream ofs(seed + ".cnmp");
    if(!ofs.is_open())
        throw runtime_error("failed to open files");
    ofs << seed << " " << map[0].size() << " " << map.size() << endl;
    for(auto& i : map)
        for(auto& block : i)
            ofs << block.ID << endl;
    
    Player* player = reinterpret_cast<Player*>(entities[0].get());
    ofs << player->health << " " << player->satiety << endl << player->pos.x << " " << player->pos.y << " " << player->forward.x << " " << player->forward.y << endl;
    ofs << player->bag.size() << endl;
    for(auto& i : player->bag){
        ofs << i.first.ID << " " << i.first.durability << " "  << i.second << endl;
    }
}
