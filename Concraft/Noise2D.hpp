//
//  Noise2D.hpp
//  Concraft
//
//  Created by Ninter6 on 2023/5/20.
//

#pragma once

#include <vector>
#include <sstream>
#include <random>
#include "mathpls.h"

using namespace std;

/**
 * 二维噪音类
 */
class Noise2D {
public:
    /**
     * 二维噪音类的构造方法
     * @param diff 间距
     * @param loud 响度
     * @param seed 种子
     */
    Noise2D(int diff, double loud, int seed);
    
    int diff;  // 间距
    double loud;  // 响度
    int seed;
    
    /**
     * 获取某个位置上的buff
     * @param x 坐标
     * @param y 坐标
     * @return buff
     */
    double getBuff(int x, int y);
    
private:
    /**
     * 平滑函数, 即 y = 3x^2 - 2x^3
     * @param xi 自变量x
     */
    constexpr double smoothFunc(double xi) {
        return 3 * xi*xi - 2 * xi*xi*xi;
    }
    
    /**
     * 传入一个坐标，返回这个坐标所在区域的四个角落噪音源头位置
     * @param x 坐标
     * @param y 坐标
     * @return 四个源头位置组成的vector
     */
    vector<mathpls::vec2> getNoisePosition(int x, int y) const;
    
    /**
     * 获取一个源头的噪音值
     * @param x 坐标
     * @param y 坐标
     * @return 噪音值
     */
    double getCoreNoise(int x, int y) const;
    
};

/**
 * 二维噪音集类
 */
class NoiseSet2D {
public:
    int seed;
    // 这个二维噪音集的所有单层二维噪音
    vector<Noise2D> noise2DS;

    /**
     * 二维噪音集类的构造方法
     * @param seed 种子
     */
    explicit NoiseSet2D(int seed);

    /**
     * 添加一个噪音
     * @param diff 间距
     * @param loud 响度
     */
    void addNoise(int diff, double loud);

    /**
     * 获取该噪声处某一个位置的高度
     * @param x 坐标
     * @param y 坐标
     * @return 高度
     */
    double getHeight(int x, int y);
};
