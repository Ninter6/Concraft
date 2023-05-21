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
                
            default:
                flag = true;
                break;
        }
    }
    if((((int) world->_E() % 2147483647) % 100) < 1){
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
            }
        if(!found) player->bag.push_back({*item, 1});
        
        for(auto i = world->entities.begin(); i!=world->entities.end(); i++)
            if((*i).get() == item){
                world->kill.push_back(i);
                break;
            }
    }
}
