//
//  Contorller.cpp
//  Concraft
//
//  Created by Ninter6 on 2023/5/21.
//

#include "Contorller.hpp"

#include <termios.h>
#include <cstdio>
#include <iomanip>

PlayerController::PlayerController(World* world) : world(world) {}

void PlayerController::move(GameObject *object){
    Player* player = reinterpret_cast<Player*>(object);
    
    if(keepDigging)
        keepDigging = false;
    else
        diggingTime = 0;
    
    for(bool flag = true; flag;){
        flag = false;
        char key = scanKeyboard();
        switch (key) {
            case 'i':
                key = 65;
                break;
            case 'k':
                key = 66;
                break;
            case 'l':
                key = 67;
                break;
            case 'j':
                key = 68;
                break;
        }
        switch (key) {
            // 上
            case 65:
                if(player->forward == mathpls::vec2{0, -1}){
                    if(player->pos.y - 1 >= 0 && !world->map[player->pos.y-1][player->pos.x].blocked)
                        player->pos += {0, -1};
                } else {
                    player->forward = {0, -1};
                    player->zi = "AA";
                }
                break;
            // 下
            case 66:
                if(player->forward == mathpls::vec2{0, 1}){
                    if(player->pos.y+1 < world->map.size() && !world->map[player->pos.y+1][player->pos.x].blocked)
                        player->pos += {0, 1};
                } else {
                    player->forward = {0, 1};
                    player->zi = "VV";
                }
                break;
            // 右
            case 67:
                if(player->forward == mathpls::vec2{1, 0}){
                    if(player->pos.x + 1 < world->map[0].size() && !world->map[player->pos.y][player->pos.x+1].blocked)
                        player->pos += {1, 0};
                } else {
                    player->forward = {1, 0};
                    player->zi = ">>";
                }
                break;
            // 左
            case 68:
                if(player->forward == mathpls::vec2{-1, 0}){
                    if(player->pos.x - 1 >= 0 && !world->map[player->pos.y][player->pos.x-1].blocked)
                        player->pos += {-1, 0};
                } else {
                    player->forward = {-1, 0};
                    player->zi = "<<";
                }
                break;
            case '0':
                player->hold=0;
                break;
            case '1':
                if(player->bag.size() >= 1)
                    player->hold=1;
                break;
            case '2':
                if(player->bag.size() >= 2)
                    player->hold=2;
                break;
            case '3':
                if(player->bag.size() >= 3)
                    player->hold=3;
                break;
            case '4':
                if(player->bag.size() >= 4)
                    player->hold=4;
                break;
            case '5':
                if(player->bag.size() >= 5)
                    player->hold=5;
                break;
            case '6':
                if(player->bag.size() >= 6)
                    player->hold=6;
                break;
            case '7':
                if(player->bag.size() >= 7)
                    player->hold=7;
                break;
            case '8':
                if(player->bag.size() >= 8)
                    player->hold=8;
                break;
                
            case 'z':
                destroyBlock(player);
                break;
            case 'x':
                useItems(player);
                break;
            case 'c':
                if(player->bag.size() > 0)
                    bagMenu(player);
                break;
                
            case 't':
                world->saveToLocal();
                break;
                
            default:
                flag = true;
                break;
        }
    }
    
    if(percentage(2)){
        if(player->satiety>0)
            player->satiety--;
        else
            player->health--;
    }
}

char PlayerController::scanKeyboard(){
    termios new_settings;
    termios stored_settings;
    tcgetattr(0,&stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_cc[VTIME] = 0;
    tcgetattr(0,&stored_settings);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0,TCSANOW,&new_settings);

    char in = getchar();

    tcsetattr(0,TCSANOW,&stored_settings);
    return in;
}

void PlayerController::destroyBlock(Player* player){
    if(player->hold > player->bag.size())
        return;
    
    auto toward = player->pos + player->forward;
    if(toward.x < 0 || toward.x >= world->map[0].size() || toward.y < 0 || toward.y >= world->map.size()) return;
    const auto& block = world->map[toward.y][toward.x];
    if(player->hold){
        auto& item = player->bag[player->hold - 1];
        // 三个值只要有一个是0就挖不掉
        // 挖掘时长 = 方块坚硬度 / 工具坚硬度 * 方块耐久
        if(round((float)block.hardness/item.first.hardness*block.durability)==++diggingTime){
            if(block.hardness <= item.first.hardness){
                int itemNum = block.item_min + uniform01()*(block.item_max-block.item_min) + .5;
                for(int i = 0; i < itemNum; i++)
                    world->SummonOneEntity(toward, "item_" + block.itemName);
                if(--item.first.durability == 0){
                    if(--item.second == 0)
                        player->bag.erase(player->bag.begin() + (player->hold - 1));
                    else
                        item.first.durability = Items[item.first.ID].durability;
                }
            }
            world->map[toward.y][toward.x] = Blocks["air"];
        }
    } else if(block.hardness * block.durability == ++diggingTime){ // 空手坚硬度为1
        if(block.hardness == 1){
            int itemNum = block.item_min + uniform01()*(block.item_max-block.item_min) + .5;
            for(int i = 0; i < itemNum; i++)
                world->SummonOneEntity(toward, "item_" + block.itemName);
        }
        world->map[toward.y][toward.x] = Blocks["air"];
    }
    
    keepDigging = true;
}

void PlayerController::useItems(Player* player){
    if(!player->hold || player->hold > player->bag.size())
        return;
    auto& item = player->bag[player->hold - 1];
    const auto& attribute = ItemAttributes[item.first.ID];
    
    if (attribute.placeable) {
        mathpls::vec2 toward = player->pos + player->forward;
        if (world->map[toward.y][toward.x] == Blocks["air"]) {
            world->map[toward.y][toward.x] = Blocks[attribute.placeBlockName];
            if (--item.second == 0)
                player->bag.erase(player->bag.begin() + (player->hold - 1));
        }
    } else if (attribute.edible && player->satiety != 10) {
        player->satiety += attribute.satietyAdd;
        player->satiety = mathpls::min(player->satiety, 10);
        if (--item.second == 0)
            player->bag.erase(player->bag.begin() + (player->hold - 1));
    }
}

void PlayerController::bagMenu(Player *player){
    auto& bag = player->bag;
    char key{};
    ostringstream os;
    int opt = mathpls::mid(0, player->hold-1, 7), mode = 0, swapBuf = 0;
    
    do{
        clear();
        auto craftList = searchCraftLists(bag);
        switch (mode) {
            case 0:
                opt = mathpls::mid(mathpls::min(8, (int)bag.size() - 1), opt, 0);
                break;
            case 1:
                opt = mathpls::mid((int)bag.size() - 1, opt, 0);
                break;
            case 2:
                if(craftList.size() > 0)
                    opt = mathpls::mid((int)craftList.size() - 1, opt, 0);
                else
                    mode = 0;
                break;
        }
        
        for(int i = 1; i<9; i++)
            cout<<i<<".   ";
        endl(cout);
        for(int i = 0; i < mathpls::min<uint64_t>(bag.size(), 8);i++){
            if(mode == 0 && opt == i)
                os<<"\033[48;5;247m"<<setw(2 + (int)(bag[i].first.name.size()>4 ? 2 : bag[i].first.name.size()))<<setfill(' ')<<bag[i].first.name<<"\033[0m"<<" ";
            else
                os<<setw(2 + (int)(bag[i].first.name.size()>4 ? 2 : bag[i].first.name.size()))<<setfill(' ')<<bag[i].first.name<<" ";
        }
        cout<<os.str()<<endl<<endl;
        os.str("");os.clear();
        for(int i=0;i<bag.size();i++){
            if(mode == 1 && opt == i)
                os<<"\033[48;5;247m"<<'['<<bag[i].first.name<<", "<<bag[i].second<<']'<<"\033[0m"<<" ";
            else
                os<<'['<<bag[i].first.name<<", "<<bag[i].second<<']'<<" ";
        }
        cout<<os.str()<<endl;
        os.str("");os.clear();
        cout<<endl<<"可合成: ";
        for(int i = 0; i < craftList.size(); i++){
            if(mode == 2 && opt == i)
                os << "\033[48;5;247m" <<Items[craftList[i].product].name<< "\033[0m" << " ";
            else
                os << Items[craftList[i].product].name << " ";
        }
        cout<<os.str()<<endl;
        os.str("");os.clear();
        
        switch (key = scanKeyboard()) {
            case 'i':
                key = 65;
                break;
            case 'k':
                key = 66;
                break;
            case 'l':
                key = 67;
                break;
            case 'j':
                key = 68;
                break;
        }
        switch (key) {
            case 65:
                if(mode == 2){
                    opt = swapBuf;
                    mode = 0;
                }
                break;
            case 66:
                if(mode == 0){
                    swapBuf = opt;
                    mode = 2;
                }
                break;
            case 67:
                ++opt;
                break;
            case 68:
                --opt;
                break;
                
            case 'z':
                switch (mode) {
                    case 0:
                        mode = 1;
                        swapBuf = opt;
                        break;
                    case 1:
                        swap(bag[swapBuf], bag[opt]);
                        opt = swapBuf;
                        mode = 0;
                        break;
                    case 2:
                        for(const auto& mat : craftList[opt].materials)
                            for(int i = 0; i < bag.size(); i++)
                                if(bag[i].first.ID == mat.first && (bag[i].second -= mat.second) == 0){
                                    bag.erase(bag.begin() + i);
                                    break;
                                }
                        bool found = false;
                        for(auto& i : player->bag)
                            if (i.first.ID == craftList[opt].product){
                                i.second += craftList[opt].num;
                                found = true;
                                break;
                            }
                        if(!found) bag.emplace_back(Items[craftList[opt].product], craftList[opt].num);
                        break;
                }
                break;
                
            default:
                break;
        }
    }while(key != 'c');
}

vector<CraftList> PlayerController::searchCraftLists(const vector<std::pair<Item, int>>& bag)
{
    vector<CraftList> r;
    for(const auto& list : CraftLists){
        bool ok = true;
        for(const auto& mat : list.materials){
            bool found = false;
            for(const auto& i : bag)
                if (i.first.ID == mat.first) {
                    if(i.second < mat.second)
                        ok = false;
                    found = true;
                    break;
                }
            if (!found) {
                ok = false;
            }
        }
        if (ok) r.push_back(list);
    }
    return r;
}

ItemController::ItemController(World* world) : world(world) {}

void ItemController::move(GameObject *object){
    Item* item = reinterpret_cast<Item*>(object);
    Player* player = reinterpret_cast<Player*>(world->entities[0].get());
    if (player->pos == item->pos) {
        bool found = false;
        for(auto& i : player->bag)
            if (i.first == *item){
                i.second++;
                found = true;
                break;
            }
        if(!found) player->bag.emplace_back(*item, 1);
        
        for(uint32_t i = 0; i < world->entities.size(); i++)
            if(world->entities[i].get() == item){
                world->kill.push(i);
                break;
            }
    }
}
