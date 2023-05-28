# Concraft
## 介绍
一个简单的用c++编写的, 《我的世界》2D终端文字版小游戏(暂只支持Unix, 但很好移植)

## 编译并运行
### Unix
```
git clone https://github.com/Ninter6/Concraft.git
cd Concraft
make run
```

### Windows
你可能要修改以下文件
- World.pp:21         修改用于清屏的指令
- GameObject.hpp:27   修改改变终端颜色的方法
- Controller.cpp:251  修改改变终端颜色的方法

## 玩法
- `上下左右` 或 `IKJL` 键 控制移动
- `Z` 破坏方块
- `X` 放置或使用物品
- `C` 打开背包
- 打开背包后, `K`或`下` 进入合成 `I`或`上` 返回槽位, 物品槽位按 `Z` 可切换背包中的物品, 按 `JL`或`左右` 左右切换
