//
//  Noise2D.cpp
//  Concraft
//
//  Created by Ninter6 on 2023/5/20.
//

#include "Noise2D.hpp"

Noise2D::Noise2D(int diff, double loud, int seed) : diff(diff), loud(loud), seed(seed) {}

double Noise2D::getBuff(int x, int y){
   vector<mathpls::vec2> noises = this->getNoisePosition(x, y);
   mathpls::vec2 p1 = noises[0];
   mathpls::vec2 p2 = noises[1];
   mathpls::vec2 p3 = noises[2];
   mathpls::vec2 p4 = noises[3];
   int xMin, yMin;
   xMin = p1.x;
   yMin = p4.y;
   double qRight = smoothFunc((float) (x - xMin) / (float) this->diff);
   double qLeft = 1 - qRight;
   double qTop = smoothFunc((float) (y - yMin) / (float) this->diff);
   double qDown = 1 - qTop;
   double n1, n2, n3, n4;
   n1 = this->getCoreNoise((int) p1.x, (int) p1.y);
   n2 = this->getCoreNoise((int) p2.x, (int) p2.y);
   n3 = this->getCoreNoise((int) p3.x, (int) p3.y);
   n4 = this->getCoreNoise((int) p4.x, (int) p4.y);
   return n1 * (qLeft * qTop) +
          n2 * (qRight * qTop) +
          n3 * (qLeft * qDown) +
          n4 * (qRight * qDown);
}

vector<mathpls::vec2> Noise2D::getNoisePosition(int x, int y) const {
    int xMin, xMax, yMin, yMax;
    if (x % this->diff == 0) {
        xMin = x;
        xMax = x + this->diff;
    } else {
        xMin = x - x % this->diff;
        xMax = xMin + this->diff;
    }
    if (y % this->diff == 0) {
        yMin = y;
        yMax = y + this->diff;
    } else {
        yMin = y - y % this->diff;
        yMax = yMin + this->diff;
    }
    vector<mathpls::vec2> res{};
    // 左上 右上 左下 右下
    res.emplace_back(xMin, yMax);
    res.emplace_back(xMax, yMax);
    res.emplace_back(xMin, yMin);
    res.emplace_back(xMax, yMin);
    return res;
}

double Noise2D::getCoreNoise(int x, int y) const {
    vector<mathpls::vec2> p4 = this->getNoisePosition(x, y);
    for (auto p: p4) {
        if (p == mathpls::vec2(x, y)) {
            ostringstream os;
            os << x << '-' << y << '-' << this->seed;
            uint32_t se = static_cast<uint32_t>(hash<string>{}(os.str()));
            default_random_engine e(se);
            uniform_real_distribution<double> u(0, 1);
            return (u(e) * 2 - 1) * this->loud;
        }
    }
    return 0;
}

NoiseSet2D::NoiseSet2D(int seed) : seed(seed) {}

void NoiseSet2D::addNoise(int diff, double loud){
    this->noise2DS.emplace_back(diff, loud, seed + this->noise2DS.size() * 50);
}

double NoiseSet2D::getHeight(int x, int y) {
    double res = 0;
    for (Noise2D noise2D: this->noise2DS) {
        res += noise2D.getBuff(x, y);
    }
    return res;
}
