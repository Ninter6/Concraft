//
//  Contorller.cpp
//  Concraft
//
//  Created by Ninter6 on 2023/5/21.
//

#include "Contorller.hpp"

#include <termios.h>
#include <stdio.h>

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
            case 'w':
                key = 65;
                break;
            case 's':
                key = 66;
                break;
            case 'd':
                key = 67;
                break;
            case 'a':
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
        if(block.hardness / item.first.hardness * block.durability == ++diggingTime){
            int itemNum = block.item_min + uniform01()*(block.item_max-block.item_min) + .5;
            for(int i = 0; i < itemNum; i++)
                world->SummonOneEntity(toward, "item_" + block.itemName);
            if(--item.first.durability == 0 && --item.second == 0){
                player->bag.erase(player->bag.begin() + (player->hold - 1));
                item.first.durability = Items[item.first.ID].durability;
            }
            world->map[toward.y][toward.x] = Blocks["air"];
        }
    } else if(block.hardness * block.durability == ++diggingTime){ // 空手坚硬度为1
        int itemNum = block.item_min + uniform01()*(block.item_max-block.item_min) + .5;
        for(int i = 0; i < itemNum; i++)
            world->SummonOneEntity(toward, "item_" + block.itemName);
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
        if(!found) player->bag.push_back({*item, 1});
        
        for(uint32_t i = 0; i < world->entities.size(); i++)
            if(world->entities[i].get() == item){
                world->kill.push(i);
                break;
            }
    }
}
