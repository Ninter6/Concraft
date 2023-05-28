//
//  StaticRandom.hpp
//  Concraft
//
//  Created by Ninter6 on 2023/5/27.
//

#pragma once

#include <random>

static std::default_random_engine _E(static_cast<uint32_t>(time(nullptr)));

/**
 * 生成 [0, n] 范围的随机整数
 * @param n 最大值(不包括)
 * @return 结果
 */
inline int randint(int n) {
    return int(_E() % 2147483647) % n;
}

/**
 * 百分之多少的事情发生一件事
 * @param n 百分之(这个数)
 * @return 真假
 */
inline bool percentage(int n) {
    int r = int(_E() % 2147483647) % 100;
    if (r < n)
        return true;
    else
        return false;
}

/**
 * 返回0~1之间的随机小数
 * @return 结果
 */
inline double uniform01() {
    return uniform_real_distribution<double>{0, 1}(_E);
}
