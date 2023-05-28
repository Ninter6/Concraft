#pragma once
namespace mathpls{
#ifndef MATHPLS_DEFINITION

template<class T>
constexpr T max(T a, T b){
    return a>b ? a:b;
}

template<class T>
constexpr T min(T a, T b){
    return a<b ? a:b;
}

template<class T> // 返回第二大的,你也可以当成,是否在范围内,否则返回最大或最小值
constexpr T mid(T min, T a, T max){
    return (min<(a<max?a:max)?(a<max?a:max):min<(a>max?a:max)?min:(a>max?a:max));
}

template<class T>
constexpr T abs(T a){
    return a > 0 ? a : -a;
}

constexpr static long double PI = 3.14159265358979323846264338327950288;

template<class T = float>
constexpr T radians(long double angle){
    return angle / (long double)180 * PI;
}

constexpr long double sqrt(long double x){
    if (x == 1 || x == 0)
        return x;
    double temp = x / 2;
    while (abs(temp - (temp + x / temp) / 2) > 0.000001)
        temp = (temp + x / temp) / 2;
    return temp;
}

constexpr long double pow(long double ori, long double a){
    if(a < 0) return 1. / pow(ori, -a);
    int ip = a;
    long double fp = a - ip;
    long double r = 1;
    while(ip--) r *= ori;
    constexpr long double c[] = {0.5, 0.25, 0.125, 0.0625, 0.03125, 0.015625, 0.0078125, 0.00390625, 0.001953125, 0.0009765625, 0.00048828125, 0.000244140625, 0.0001220703125, 0.00006103515625, 0.000030517578125, 0.0000152587890625};
    long double t = ori;
    for(int i=0; fp >= c[15]; i++){
        t = sqrt(t);
        if(fp < c[i]) continue;
        fp -= c[i];
        r *= t;
    }
    return r;
}

// 三角函数这里对精度和性能上做了很多取舍,目前基本上已经是最理想的情况了,可以保证小数点后4位没有误差
constexpr long double sin(long double a){
    if(a < 0) return -sin(-a); // sin(-a) = -sin(a)
    
    constexpr int
        angle[] = {23040, 13601, 7187, 3648, 1831, 916, 458, 229, 115, 57, 29, 14, 7, 4, 2, 1};
    
    long long x = 1000000, y = 0; // x的大小会影响精度,不能太大也不能太小,貌似10^6最好
    long long t = 0, r = a/PI*180*512;
    while(r > 184320) r -= 184320;
    
    for(int i=0; i<16; i++){
        long long rx = x, ry = y;
        while(t < r){
            rx = x;
            ry = y;
            x = rx - (ry>>i);
            y = ry + (rx>>i);
            t += angle[i];
        }
        if(t == r){
            return (long double)y / sqrt(x*x + y*y);
        }else{
            t -= angle[i];
            x = rx;
            y = ry;
        }
    }
    return (long double)y / sqrt(x*x + y*y);
}

constexpr long double cos(long double a){
    return sin(PI/2 - a);
}

constexpr long double tan(long double a){
    return sin(a) / cos(a);
}

constexpr long double cot(long double a){
    return cos(a) / sin(a);
}

constexpr long double sec(long double a){
    return 1 / cos(a);
}

constexpr long double csc(long double a){
    return 1 / sin(a);
}

constexpr long double atan2(long double y, long double x)
{
    constexpr int
        angle[] = {11520, 6801, 3593, 1824, 916, 458, 229, 115, 57, 29, 14, 7, 4, 2, 1};
    
    int x_new{}, y_new{};
    int angleSum = 0;
    
    int lx = x * 1000000;
    int ly = y * 1000000;
    
    for(int i = 0; i < 15; i++)
    {
        if(ly > 0)
        {
            x_new = lx + (ly >> i);
            y_new = ly - (lx >> i);
            lx = x_new;
            ly = y_new;
            angleSum += angle[i];
        }
        else
        {
            x_new = lx - (ly >> i);
            y_new = ly + (lx >> i);
            lx = x_new;
            ly = y_new;
            angleSum -= angle[i];
        }
    }
    return radians<long double>((long double)angleSum / (long double)256);
}

constexpr long double atan(long double a){
    return atan2(a, 1);
}

constexpr long double acot2(long double x, long double y){
    return atan2(y, x);
}

constexpr long double acot(long double a){
    return atan2(1, a);
}

constexpr long double asin2(long double y, long double m){
    long double x = sqrt(m*m - y*y);
    return atan2(y, x);
}

constexpr long double asin(long double a){
    return asin2(a, 1);
}

constexpr long double acos2(long double x, long double m){
    long double y = sqrt(m*m - x*x);
    return atan2(y, x);
}

constexpr long double acos(long double a){
    return acos2(a, 1);
}

constexpr long double asec2(long double m, long double x){
    return acos2(x, m);
}

constexpr long double asec(long double a){
    return asec2(a, 1);
}

constexpr long double acsc2(long double m, long double y){
    return asin2(y, m);
}

constexpr long double acsc(long double a){
    return acsc2(a, 1);
}

template<class T, int N>
struct vec;

template<class T>
struct vec<T, 1>{
    vec<T, 1>() : x(T(1)) {}
    vec<T, 1>(T x) : x(x) {}
    union{T x, r;};
    
    vec<T, 1> operator-() const{return vec<T, 1>(T(0)) - *this;}
    vec<T, 1> operator+(T k) const{return vec<T, 1>(x + k);}
    vec<T, 1> operator+=(T k){x += k;return *this;}
    vec<T, 1> operator-(T k) const{return vec<T, 1>(x - k);}
    vec<T, 1> operator-=(T k){x -= k;return *this;}
    vec<T, 1> operator*(T k) const{return vec<T, 1>(x * k);}
    vec<T, 1> operator*=(T k){x *= k;return *this;}
    vec<T, 1> operator/(T k) const{return vec<T, 1>(x / k);}
    vec<T, 1> operator/=(T k){x /= k;return *this;}
    vec<T, 1> operator+(vec<T, 1> k) const{return vec<T, 1>(x+k.x);}
    vec<T, 1> operator+=(vec<T, 1> k){x += k.x;return *this;}
    vec<T, 1> operator-(vec<T, 1> k) const{return vec<T, 1>(x-k.x);}
    vec<T, 1> operator-=(vec<T, 1> k){x -= k.x;return *this;}
    vec<T, 1> operator*(vec<T, 1> k) const{return vec<T, 1>(x*k.x);}
    vec<T, 1> operator*=(vec<T, 1> k){x *= k.x;return *this;}
    vec<T, 1> operator/(vec<T, 1> k) const{return vec<T, 1>(x-k.x);}
    vec<T, 1> operator/=(vec<T, 1> k){x /= k.x;return *this;}
    bool operator==(vec<T, 1> k) const{return x == k.x;}
    bool operator!=(vec<T, 1> k) const{return x != k.x;}
    
    T length() const {return abs(x);}
    vec<T, 1> normalize() const {return *this / length();}
};// 真的有人用vec1吗?
template<class T>
struct vec<T, 2>{
    vec<T, 2>() : x(T(1)), y(0) {}
    vec<T, 2>(T x, T y) : x(x), y(y) {}
    vec<T, 2>(T a) : x(a), y(a) {}
    vec<T, 2>(vec<T, 1> v1, T y = T(1)) : x(v1.x), y(y) {}
    union{T x, r;};
    union{T y, g;};
    
    vec<T, 2> operator-() const{return vec<T, 2>(T(0)) - *this;}
    vec<T, 2> operator+(T k) const{return vec<T, 2>(x + k, y + k);}
    vec<T, 2> operator+=(T k){x += k;y += k;return *this;}
    vec<T, 2> operator-(T k) const{return vec<T, 2>(x - k, y - k);}
    vec<T, 2> operator-=(T k){x -= k;y -= k;return *this;}
    vec<T, 2> operator*(T k) const{return vec<T, 2>(x * k, y * k);}
    vec<T, 2> operator*=(T k){x *= k;y *= k;return *this;}
    vec<T, 2> operator/(T k) const{return vec<T, 2>(x / k, y / k);}
    vec<T, 2> operator/=(T k){x /= k;y /= k;return *this;}
    vec<T, 2> operator+(vec<T, 2> k) const{return vec<T, 2>(x+k.x, y+k.y);}
    vec<T, 2> operator+=(vec<T, 2> k){x += k.x;y += k.y;return *this;}
    vec<T, 2> operator-(vec<T, 2> k) const{return vec<T, 2>(x-k.x, y-k.y);}
    vec<T, 2> operator-=(vec<T, 2> k){x -= k.x;y -= k.y;return *this;}
    vec<T, 2> operator*(vec<T, 2> k) const{return vec<T, 2>(x*k.x, y*k.y);}
    vec<T, 2> operator*=(vec<T, 2> k){x *= k.x;y *= k.y;return *this;}
    vec<T, 2> operator/(vec<T, 2> k) const{return vec<T, 2>(x/k.x, y/k.y);}
    vec<T, 2> operator/=(vec<T, 2> k){x /= k.x;y /= k.y;return *this;}
    bool operator==(vec<T, 2> k) const{return x == k.x && y == k.y;}
    bool operator!=(vec<T, 2> k) const{return x != k.x && y != k.y;}
    
    T length() const {return sqrt(x*x + y*y);}
    vec<T, 2> normalize() const {return *this / length();}
};
template<class T>
struct vec<T, 3>{
    vec<T, 3>() : x(T(1)), y(0), z(0) {}
    vec<T, 3>(T x, T y, T z) : x(x), y(y), z(z) {}
    vec<T, 3>(T a) : x(a), y(a), z(a) {}
    vec<T, 3>(vec<T, 2> v2, T z = T(1)) : x(v2.x), y(v2.y), z(z) {}
    union{T x, r;};
    union{T y, g;};
    union{T z, b;};
    
    vec<T, 3> operator-() const{return vec<T, 3>(T(0)) - *this;}
    vec<T, 3> operator+(T k) const{return vec<T, 3>(x + k, y + k, z + k);}
    vec<T, 3> operator+=(T k){x += k;y += k;z += k;return *this;}
    vec<T, 3> operator-(T k) const{return vec<T, 3>(x - k, y - k, z - k);}
    vec<T, 3> operator-=(T k){x -= k;y -= k;z -= k;return *this;}
    vec<T, 3> operator*(T k) const{return vec<T, 3>(x * k, y * k, z * k);}
    vec<T, 3> operator*=(T k){x *= k;y *= k;z *= k;return *this;}
    vec<T, 3> operator/(T k) const{return vec<T, 3>(x / k, y / k, z / k);}
    vec<T, 3> operator/=(T k){x /= k;y /= k;z /= k;return *this;}
    vec<T, 3> operator+(vec<T, 3> k) const{return vec<T, 3>(x+k.x, y+k.y, z+k.z);}
    vec<T, 3> operator+=(vec<T, 3> k){x += k.x;y += k.y;z += k.z;return *this;}
    vec<T, 3> operator-(vec<T, 3> k) const{return vec<T, 3>(x-k.x, y-k.y, z-k.z);}
    vec<T, 3> operator-=(vec<T, 3> k){x -= k.x;y -= k.y;z -= k.z;return *this;}
    vec<T, 3> operator*(vec<T, 3> k) const{return vec<T, 3>(x*k.x, y*k.y, z*k.z);}
    vec<T, 3> operator*=(vec<T, 3> k){x *= k.x;y *= k.y;z *= k.z;return *this;}
    vec<T, 3> operator/(vec<T, 3> k) const{return vec<T, 3>(x/k.x, y/k.y, z/k.z);}
    vec<T, 3> operator/=(vec<T, 3> k){x /= k.x;y /= k.y;z /= k.z;return *this;}
    bool operator==(vec<T, 3> k) const{return x == k.x && y == k.y && z == k.z;}
    bool operator!=(vec<T, 3> k) const{return x != k.x && y != k.y && z != k.z;}
    
    T length() const {return sqrt(x*x + y*y + z*z);}
    vec<T, 3> normalize() const {return *this / length();}
};
template<class T>
struct vec<T, 4>{
    vec<T, 4>() : x(T(1)), y(0), z(0), w(0) {}
    vec<T, 4>(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
    vec<T, 4>(T a) : x(a), y(a), z(a), w(a) {}
    vec<T, 4>(vec<T, 3> v3, T w = T(1)) : x(v3.x), y(v3.y), z(v3.z), w(w) {}
    union{T x, r;};
    union{T y, g;};
    union{T z, b;};
    union{T w, a;};

    vec<T, 4> operator-() const{return vec<T, 4>(T(0)) - *this;}
    vec<T, 4> operator+(T k) const{return vec<T, 4>(x + k, y + k, z + k, w + k);};
    vec<T, 4> operator+=(T k){x += k;y += k;z += k;w += k;return *this;}
    vec<T, 4> operator-(T k) const{return vec<T, 4>(x - k, y - k, z - k, w - k);};
    vec<T, 4> operator-=(T k){x -= k;y -= k;z -= k;w -= k;return *this;}
    vec<T, 4> operator*(T k) const{return vec<T, 4>(x * k, y * k, z * k, w * k);};
    vec<T, 4> operator*=(T k){x *= k;y *= k;z *= k;w *= k;return *this;}
    vec<T, 4> operator/(T k) const{return vec<T, 4>(x / k, y / k, z / k, w / k);};
    vec<T, 4> operator/=(T k){x /= k;y /= k;z /= k;w /= k;return *this;}
    vec<T, 4> operator+(vec<T, 4> k) const{return vec<T, 4>(x+k.x, y+k.y, z+k.z, w+k.w);}
    vec<T, 4> operator+=(vec<T, 4> k){x += k.x;y += k.y;z += k.z;w += k.w;return *this;}
    vec<T, 4> operator-(vec<T, 4> k) const{return vec<T, 4>(x-k.x, y-k.y, z-k.z, w-k.w);}
    vec<T, 4> operator-=(vec<T, 4> k){x -= k.x;y -= k.y;z -= k.z;w -= k.w;return *this;}
    vec<T, 4> operator*(vec<T, 4> k) const{return vec<T, 4>(x*k.x, y*k.y, z*k.z, w*k.w);}
    vec<T, 4> operator*=(vec<T, 4> k){x *= k.x;y *= k.y;z *= k.z;w *= k.w;return *this;}
    vec<T, 4> operator/(vec<T, 4> k) const{return vec<T, 4>(x/k.x, y/k.y, z/k.z, w/k.w);}
    vec<T, 4> operator/=(vec<T, 4> k){x /= k.x;y /= k.y;z /= k.z;w /= k.w;return *this;}
    bool operator==(vec<T, 4> k) const{return x == k.x && y == k.y && z == k.z && w == k.w;}
    bool operator!=(vec<T, 4> k) const{return x != k.x && y != k.y && z != k.z && w != k.w;}
    
    T length() const {return sqrt(x*x + y*y + z*z + w*w);}
    vec<T, 4> normalize() const {return *this / length();}
};

using vec1 = vec<float, 1>;
using vec2 = vec<float, 2>;
using vec3 = vec<float, 3>;
using vec4 = vec<float, 4>;
using ivec1 = vec<int, 1>;
using ivec2 = vec<int, 2>;
using ivec3 = vec<int, 3>;
using ivec4 = vec<int, 4>;

template<int H, int W, class T>
struct mat{
    mat(T m = {1}){
        for(int i=0; i<min(H, W); i++) element[i][i] = m;
    }
    mat(T e[W][H]){
        for(int i=0;i<H;i++) for(int j=0;j<W;j++) element[i][j] = e[j][i];
    }
    mat(T const* e){
        for(int i=0;i<H;i++) for(int j=0;j<W;j++) element[i][j] = e[i*W + j];
    }
    T element[H][W] = {0};
    T* vptr() const {return (T*)&element[0][0];}
    
    template<int H1, int W1>
    mat(mat<H1, W1, T> const& m){
        for(int i=0;i<min(H, H1);i++){
            for(int j=0;j<min(W, W1);j++){
                element[i][j] = m[i][j];
            }
        }
    }
    
    mat<W, H, T> transposed() const{
        mat<W, H, T> r;
        for(int i=0; i<H; i++)
            for(int j=0; j<W; j++)
                r[j][i] = element[i][j];
        return r;
    }
    
    T* operator[](unsigned int x) const {return (T*)element[x];}
    
    mat<H, W, T> operator+(mat<H, W, T> m) const{
        mat<H, W, T> result(T(0));
        for(int i=0;i<H;i++){
            for(int j=0;j<W;j++){
                result[i][j] = element[i][j] + m[i][j];
            }
        }
        return result;
    }
    mat<H, W, T> operator-(mat<H, W, T> m) const{
        mat<H, W, T> result(T(0));
        for(int i=0;i<H;i++){
            for(int j=0;j<W;j++){
                result[i][j] = element[i][j] - m[i][j];
            }
        }
        return result;
    }
    mat<H, W, T> operator+(T k) const{
        mat<H, W, T> r = *this;
        for(int i = 0; i < H; i++){
            for(int j = 0; j < W; j++){
                r[i][j] += k;
            }
        }
        return r;
    }
    mat<H, W, T> operator+=(T k){
        for(int i = 0; i < H; i++){
            for(int j = 0; j < W; j++){
                element[i][j] += k;
            }
        }
        return *this;
    }
    mat<H, W, T> operator-(T k) const{
        mat<H, W, T> r = *this;
        for(int i = 0; i < H; i++){
            for(int j = 0; j < W; j++){
                r[i][j] -= k;
            }
        }
        return r;
    }
    mat<H, W, T> operator-=(T k){
        for(int i = 0; i < H; i++){
            for(int j = 0; j < W; j++){
                element[i][j] -= k;
            }
        }
        return *this;
    }
    mat<H, W, T> operator*(T k) const{
        mat<H, W, T> r = *this;
        for(int i = 0; i < H; i++){
            for(int j = 0; j < W; j++){
                r[i][j] *= k;
            }
        }
        return r;
    }
    mat<H, W, T> operator*=(T k){
        for(int i = 0; i < H; i++){
            for(int j = 0; j < W; j++){
                element[i][j] *= k;
            }
        }
        return *this;
    }
    mat<H, W, T> operator/(T k) const{
        mat<H, W, T> r = *this;
        for(int i = 0; i < H; i++){
            for(int j = 0; j < W; j++){
                r[i][j] /= k;
            }
        }
        return r;
    }
    mat<H, W, T> operator/=(T k){
        for(int i = 0; i < H; i++){
            for(int j = 0; j < W; j++){
                element[i][j] /= k;
            }
        }
        return *this;
    }
    
    // 余子式
    mat<H-1, W-1, T> cofactor(int x, int y) const{
        int rx=0, ry=0;
        mat<H-1, W-1, T> r(0.f);
        for(int i=0; i<4; i++){
            if(i == x) continue;
            for(int j=0; j<4; j++){
                if(j == y) continue;
                r[rx][ry++] = element[i][j];
            }
            rx++;
            ry = 0;
        }
        return r;
    }
};
using mat2 = mat<2, 2, float>;
using mat3 = mat<3, 3, float>;
using mat4 = mat<4, 4, float>;
template<class T>
vec<T, 2> operator*(mat<2, 2, T> m, vec<T, 2> v){
    return vec<T, 2>(m[0][0]*v.x+m[1][0]*v.y, m[0][1]*v.x+m[1][1]*v.y);
}
template<class T>
vec<T, 3> operator*(mat<3, 3, T> m, vec<T, 3> v){
    return vec<T, 3>(m[0][0]*v.x+m[1][0]*v.y+m[2][0]*v.z,
                m[0][1]*v.x+m[1][1]*v.y+m[2][1]*v.z,
                m[0][2]*v.x+m[1][2]*v.y+m[2][2]*v.z);
}
template<class T>
vec<T, 4> operator*(mat<4, 4, T> m, vec<T, 4> v){
    return vec<T, 4>(m[0][0]*v.x+m[1][0]*v.y+m[2][0]*v.z+m[3][0]*v.w,
                m[0][1]*v.x+m[1][1]*v.y+m[2][1]*v.z+m[3][1]*v.w,
                m[0][2]*v.x+m[1][2]*v.y+m[2][2]*v.z+m[3][2]*v.w,
                m[0][3]*v.x+m[1][3]*v.y+m[2][3]*v.z+m[3][3]*v.w);
}
template<int H, int W, class T>
mat<H, W, T> operator*(T k, mat<H, W, T> m){
    mat<H, W, T> r = m;
    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            r[i][j] *= k;
        }
    }
    return r;
}
template<int H, int W1, int W2, class T>
mat<W1, W2, T> operator*(mat<H, W2, T> m1, mat<W1, H, T> m2){
    mat<W1, W2, T> r(T(0));
    for(int i = 0; i < W1; i++)
        for(int j = 0; j < W2; j++)
            for(int k = 0; k < H; k++)
                r[i][j] += m1[k][j] * m2[i][k];
    return r;
}
float determinant(mat2 m);
float determinant(mat3 m);
float determinant(mat4 m);
mat2 adjugate(mat2 m);
mat3 adjugate(mat3 m);
mat4 adjugate(mat4 m);
mat2 inverse(mat2 m);
mat3 inverse(mat3 m);
mat4 inverse(mat4 m);
float dot(vec2 v1, vec2 v2);
float dot(vec3 v1, vec3 v2);
float dot(vec4 v1, vec4 v2);
float includedAngle(vec2 v1, vec2 v2);
float includedAngle(vec3 v1, vec3 v2);
float includedAngle(vec4 v1, vec4 v2);
vec3 cross(vec3 v1, vec3 v2);
mat3 translate(mat3 ori, vec2 t);
mat3 translate(vec2 t);
mat4 translate(mat4 ori, vec3 t);
mat4 translate(vec3 t);
mat3 rotate(mat3 ori, long double angle);
mat3 rotate(long double angle);
mat4 rotate(mat4 ori, long double angle, vec3 axis);
mat4 rotate(long double angle, vec3 axis);
enum EARS{
    //Tait-Bryan Angle
    xyz, xzy, yxz, yzx, zxy, zyx,
    //Proper Euler Angle
    xyx, yxy, xzx, zxz, yzy, zyz
}; // 欧拉角旋转序列(Euler Angle Rotational Sequence)
mat4 rotate(mat4 ori, long double a1, long double a2, long double a3, EARS sequence);
mat4 rotate(long double a1, long double a2, long double a3, EARS sequence);
mat3 scale(mat3 ori, vec2 s);
mat3 scale(vec2 s);
mat4 scale(mat4 ori, vec3 s);
mat4 scale(vec3 s);
mat4 ortho(long double l, long double b, long double r, long double t);
mat4 ortho(long double l, long double b, long double r, long double t, long double n, long double f);
mat4 perspective(long double fov, long double asp, long double near, long double far);
mat4 lookAt(vec3 eye, vec3 target, vec3 up);
template<class T>
struct qua{
    qua(T a) : w(a), x(a), y(a), z(a) {}
    qua(T w, T x, T y, T z) : w(w), x(x), y(y), z(z) {}
    qua(T s, vec<T, 3> v) : w(s), x(v.x), y(v.y), z(v.z) {}
    qua(vec<T, 3> u, T angle) : qua<T>(cos(angle / T(2)), sin(angle / T(2)) * u) {}
    
    T w, x, y, z;
    float length() const {return sqrt(w*w + x*x + y*y + z*z);}
    qua<T> conjugate() const {return {w, -vec<T, 3>{x, y, z}};}
    qua<T> inverse() const {return conjugate() / (w*w + x*x + y*y + z*z);}
    
    qua<T> operator-() const{return qua<T>(T(0)) - *this;}
    qua<T> operator+(T k) const{return qua<T>(x + k, y + k, z + k, w + k);};
    qua<T> operator+=(T k){x += k;y += k;z += k;w += k;return *this;}
    qua<T> operator-(T k) const{return qua<T>(x - k, y - k, z - k, w - k);};
    qua<T> operator-=(T k){x -= k;y -= k;z -= k;w -= k;return *this;}
    qua<T> operator*(T k) const{return qua<T>(x * k, y * k, z * k, w * k);};
    qua<T> operator*=(T k){x *= k;y *= k;z *= k;w *= k;return *this;}
    qua<T> operator/(T k) const{return qua<T>(x / k, y / k, z / k, w / k);};
    qua<T> operator/=(T k){x /= k;y /= k;z /= k;w /= k;return *this;}
    qua<T> operator+(qua<T> k) const{return qua<T>(x+k.x, y+k.y, z+k.z, w+k.w);}
    qua<T> operator+=(qua<T> k){x += k.x;y += k.y;z += k.z;w += k.w;return *this;}
    qua<T> operator-(qua<T> k) const{return qua<T>(x-k.x, y-k.y, z-k.z, w-k.w);}
    qua<T> operator-=(qua<T> k){x -= k.x;y -= k.y;z -= k.z;w -= k.w;return *this;}
    qua<T> operator/(qua<T> k) const{return qua<T>(x/k.x, y/k.y, z/k.z, w/k.w);}
    qua<T> operator/=(qua<T> k){x /= k.x;y /= k.y;z /= k.z;w /= k.w;return *this;}
    bool operator==(qua<T> k) const{return x == k.x && y == k.y && z == k.z && w == k.w;}
    bool operator!=(qua<T> k) const{return x != k.x && y != k.y && z != k.z && w != k.w;}
    qua<T> operator*(qua<T> k) const{
        T a = k.w, b = k.x, c = k.y, d = k.z;
        return {
            w*a - x*b - y*c - z*d,
            w*b + x*a + y*d - z*c,
            w*c - x*d + y*a + z*b,
            w*d + x*c - y*b + z*a
        };
    }
    qua<T> operator*=(qua<T> k){
        T a = k.w, b = k.x, c = k.y, d = k.z;
        w = w*a - x*b - y*c - z*d;
        x = w*b + x*a + y*d - z*c;
        y = w*c - x*d + y*a + z*b;
        z = w*d + x*c - y*b + z*a;
        return *this;
    }
};
template<class T>
mat4 rotate(qua<T> q){
    const T a = q.w, b = q.x, c = q.y, d = q.z;
    float mv[] = {
        a*a + b*b - c*c - d*d, 2*a*d - 2*b*c, -(2*a*c + 2*b*d), 0,
        -(2*b*c + 2*a*d), a*a - b*b + c*c - d*d, 2*c*d - 2*a*b, 0,
        2*a*c - 2*b*d, 2*a*b + 2*c*d, a*a - b*b - c*c + d*d, 0,
        0, 0, 0, 1
    };
    return mat4(mv);
}

namespace random {
struct RandomSequence{
private:
    unsigned int m_index;
    unsigned int m_intermediateOffset;

    static unsigned int permuteQPR(unsigned int x)
    {
        static const unsigned int prime = 4294967291u;
        if (x >= prime)
            return x;  // The 5 integers out of range are mapped to themselves.
        unsigned int residue = ((unsigned long long) x * x) % prime;
        return (x <= prime / 2) ? residue : prime - residue;
    }

public:
    RandomSequence(unsigned int seedBase, unsigned int seedOffset)
    {
        m_index = permuteQPR(permuteQPR(seedBase) + 0x682f0161);
        m_intermediateOffset = permuteQPR(permuteQPR(seedOffset) + 0x46790905);
    }
    RandomSequence(unsigned int seed) : RandomSequence(seed, seed + 1) {}

    unsigned int next()
    {
        return permuteQPR((permuteQPR(m_index++) + m_intermediateOffset) ^ 0x5bf03635);
    }
    
    unsigned int operator()(){
        return next();
    }
};

struct mt19937{
    mt19937(unsigned int seed){
        mt[0] = seed;
        for(int i=1;i<624;i++)
            mt[i] = static_cast<unsigned int>(1812433253 * (mt[i - 1] ^ mt[i - 1] >> 30) + i);
    }
    
    unsigned int operator()(){
        return extract_number();
    }
    
private:
    unsigned int mt[624];
    unsigned int mti{0};
    
    unsigned int extract_number(){
        if(mti == 0) twist();
        unsigned long long y = mt[mti];
        y = y ^ y >> 11;
        y = y ^ (y << 7 & 0x9D2C5680);
        y = y ^ (y << 15 & 0xEFC60000);
        y = y ^ y >> 18;
        mti = (mti + 1) % 624;
        return static_cast<unsigned int>(y);
    }
    
    void twist(){
        for(int i=0;i<624;i++){
            // 高位和低位级联
            auto y = static_cast<unsigned int>((mt[i] & 0x80000000) + (mt[(i + 1) % 624] & 0x7fffffff));
            mt[i] = (y >> 1) ^ mt[(i + 397) % 624];
            if(y % 2 != 0) mt[i] = mt[i] ^ 0x9908b0df; // 如果最低为不为零
        }
    }
};

template<class T = long double>
struct uniform_real_distribution{
    T a, b;
    template<class RT>
    T operator()(RT& e) const {
        return a + (b - a) / 0xFFFFFFFF * e();
    }
};
}

#else

template<class T>
constexpr T max(T a, T b){
    return a>b ? a:b;
}

template<class T>
constexpr T min(T a, T b){
    return a<b ? a:b;
}

template<class T> // 返回第二大的,你也可以当成,是否在范围内,否则返回最大或最小值
constexpr T mid(T min, T a, T max){
    return (min<(a<max?a:max)?(a<max?a:max):min<(a>max?a:max)?min:(a>max?a:max));
}

template<class T>
constexpr T abs(T a){
    return a > 0 ? a : -a;
}

constexpr static long double PI = 3.14159265358979323846264338327950288;

template<class T = float>
constexpr T radians(long double angle){
    return angle / (long double)180 * PI;
}

constexpr long double sqrt(long double x){
    if (x == 1 || x == 0)
        return x;
    double temp = x / 2;
    while (abs(temp - (temp + x / temp) / 2) > 0.000001)
        temp = (temp + x / temp) / 2;
    return temp;
}

constexpr long double pow(long double ori, long double a){
    if(a < 0) return 1. / pow(ori, -a);
    int ip = a;
    long double fp = a - ip;
    long double r = 1;
    while(ip--) r *= ori;
    constexpr long double c[] = {0.5, 0.25, 0.125, 0.0625, 0.03125, 0.015625, 0.0078125, 0.00390625, 0.001953125, 0.0009765625, 0.00048828125, 0.000244140625, 0.0001220703125, 0.00006103515625, 0.000030517578125, 0.0000152587890625};
    long double t = ori;
    for(int i=0; fp >= c[15]; i++){
        t = sqrt(t);
        if(fp < c[i]) continue;
        fp -= c[i];
        r *= t;
    }
    return r;
}

// 三角函数这里对精度和性能上做了很多取舍,目前基本上已经是最理想的情况了,可以保证小数点后4位没有误差
constexpr long double sin(long double a){
    if(a < 0) return -sin(-a); // sin(-a) = -sin(a)
    
    constexpr int
        angle[] = {23040, 13601, 7187, 3648, 1831, 916, 458, 229, 115, 57, 29, 14, 7, 4, 2, 1};
    
    long long x = 1000000, y = 0; // x的大小会影响精度,不能太大也不能太小,貌似10^6最好
    long long t = 0, r = a/PI*180*512;
    while(r > 184320) r -= 184320;
    
    for(int i=0; i<16; i++){
        long long rx = x, ry = y;
        while(t < r){
            rx = x;
            ry = y;
            x = rx - (ry>>i);
            y = ry + (rx>>i);
            t += angle[i];
        }
        if(t == r){
            return (long double)y / sqrt(x*x + y*y);
        }else{
            t -= angle[i];
            x = rx;
            y = ry;
        }
    }
    return (long double)y / sqrt(x*x + y*y);
}

constexpr long double cos(long double a){
    return sin(PI/2 - a);
}

constexpr long double tan(long double a){
    return sin(a) / cos(a);
}

constexpr long double cot(long double a){
    return cos(a) / sin(a);
}

constexpr long double sec(long double a){
    return 1 / cos(a);
}

constexpr long double csc(long double a){
    return 1 / sin(a);
}

constexpr long double atan2(long double y, long double x)
{
    constexpr int
        angle[] = {11520, 6801, 3593, 1824, 916, 458, 229, 115, 57, 29, 14, 7, 4, 2, 1};
    
    int x_new{}, y_new{};
    int angleSum = 0;
    
    int lx = x * 1000000;
    int ly = y * 1000000;
    
    for(int i = 0; i < 15; i++)
    {
        if(ly > 0)
        {
            x_new = lx + (ly >> i);
            y_new = ly - (lx >> i);
            lx = x_new;
            ly = y_new;
            angleSum += angle[i];
        }
        else
        {
            x_new = lx - (ly >> i);
            y_new = ly + (lx >> i);
            lx = x_new;
            ly = y_new;
            angleSum -= angle[i];
        }
    }
    return radians<long double>((long double)angleSum / (long double)256);
}

constexpr long double atan(long double a){
    return atan2(a, 1);
}

constexpr long double acot2(long double x, long double y){
    return atan2(y, x);
}

constexpr long double acot(long double a){
    return atan2(1, a);
}

constexpr long double asin2(long double y, long double m){
    long double x = sqrt(m*m - y*y);
    return atan2(y, x);
}

constexpr long double asin(long double a){
    return asin2(a, 1);
}

constexpr long double acos2(long double x, long double m){
    long double y = sqrt(m*m - x*x);
    return atan2(y, x);
}

constexpr long double acos(long double a){
    return acos2(a, 1);
}

constexpr long double asec2(long double m, long double x){
    return acos2(x, m);
}

constexpr long double asec(long double a){
    return asec2(a, 1);
}

constexpr long double acsc2(long double m, long double y){
    return asin2(y, m);
}

constexpr long double acsc(long double a){
    return acsc2(a, 1);
}

template<class T, int N>
struct vec;

template<class T>
struct vec<T, 1>{
    vec<T, 1>() : x(T(1)) {}
    vec<T, 1>(T x) : x(x) {}
    union{T x, r;};
    
    vec<T, 1> operator-() const{return vec<T, 1>(T(0)) - *this;}
    vec<T, 1> operator+(T k) const{return vec<T, 1>(x + k);}
    vec<T, 1> operator+=(T k){x += k;return *this;}
    vec<T, 1> operator-(T k) const{return vec<T, 1>(x - k);}
    vec<T, 1> operator-=(T k){x -= k;return *this;}
    vec<T, 1> operator*(T k) const{return vec<T, 1>(x * k);}
    vec<T, 1> operator*=(T k){x *= k;return *this;}
    vec<T, 1> operator/(T k) const{return vec<T, 1>(x / k);}
    vec<T, 1> operator/=(T k){x /= k;return *this;}
    vec<T, 1> operator+(vec<T, 1> k) const{return vec<T, 1>(x+k.x);}
    vec<T, 1> operator+=(vec<T, 1> k){x += k.x;return *this;}
    vec<T, 1> operator-(vec<T, 1> k) const{return vec<T, 1>(x-k.x);}
    vec<T, 1> operator-=(vec<T, 1> k){x -= k.x;return *this;}
    vec<T, 1> operator*(vec<T, 1> k) const{return vec<T, 1>(x*k.x);}
    vec<T, 1> operator*=(vec<T, 1> k){x *= k.x;return *this;}
    vec<T, 1> operator/(vec<T, 1> k) const{return vec<T, 1>(x-k.x);}
    vec<T, 1> operator/=(vec<T, 1> k){x /= k.x;return *this;}
    bool operator==(vec<T, 1> k) const{return x == k.x;}
    bool operator!=(vec<T, 1> k) const{return x != k.x;}
    
    T length() const {return abs(x);}
    vec<T, 1> normalize() const {return *this / length();}
};// 真的有人用vec1吗?
template<class T>
struct vec<T, 2>{
    vec<T, 2>() : x(T(1)), y(0) {}
    vec<T, 2>(T x, T y) : x(x), y(y) {}
    vec<T, 2>(T a) : x(a), y(a) {}
    vec<T, 2>(vec<T, 1> v1, T y = T(1)) : x(v1.x), y(y) {}
    union{T x, r;};
    union{T y, g;};
    
    vec<T, 2> operator-() const{return vec<T, 2>(T(0)) - *this;}
    vec<T, 2> operator+(T k) const{return vec<T, 2>(x + k, y + k);}
    vec<T, 2> operator+=(T k){x += k;y += k;return *this;}
    vec<T, 2> operator-(T k) const{return vec<T, 2>(x - k, y - k);}
    vec<T, 2> operator-=(T k){x -= k;y -= k;return *this;}
    vec<T, 2> operator*(T k) const{return vec<T, 2>(x * k, y * k);}
    vec<T, 2> operator*=(T k){x *= k;y *= k;return *this;}
    vec<T, 2> operator/(T k) const{return vec<T, 2>(x / k, y / k);}
    vec<T, 2> operator/=(T k){x /= k;y /= k;return *this;}
    vec<T, 2> operator+(vec<T, 2> k) const{return vec<T, 2>(x+k.x, y+k.y);}
    vec<T, 2> operator+=(vec<T, 2> k){x += k.x;y += k.y;return *this;}
    vec<T, 2> operator-(vec<T, 2> k) const{return vec<T, 2>(x-k.x, y-k.y);}
    vec<T, 2> operator-=(vec<T, 2> k){x -= k.x;y -= k.y;return *this;}
    vec<T, 2> operator*(vec<T, 2> k) const{return vec<T, 2>(x*k.x, y*k.y);}
    vec<T, 2> operator*=(vec<T, 2> k){x *= k.x;y *= k.y;return *this;}
    vec<T, 2> operator/(vec<T, 2> k) const{return vec<T, 2>(x/k.x, y/k.y);}
    vec<T, 2> operator/=(vec<T, 2> k){x /= k.x;y /= k.y;return *this;}
    bool operator==(vec<T, 2> k) const{return x == k.x && y == k.y;}
    bool operator!=(vec<T, 2> k) const{return x != k.x && y != k.y;}
    
    T length() const {return sqrt(x*x + y*y);}
    vec<T, 2> normalize() const {return *this / length();}
};
template<class T>
struct vec<T, 3>{
    vec<T, 3>() : x(T(1)), y(0), z(0) {}
    vec<T, 3>(T x, T y, T z) : x(x), y(y), z(z) {}
    vec<T, 3>(T a) : x(a), y(a), z(a) {}
    vec<T, 3>(vec<T, 2> v2, T z = T(1)) : x(v2.x), y(v2.y), z(z) {}
    union{T x, r;};
    union{T y, g;};
    union{T z, b;};
    
    vec<T, 3> operator-() const{return vec<T, 3>(T(0)) - *this;}
    vec<T, 3> operator+(T k) const{return vec<T, 3>(x + k, y + k, z + k);}
    vec<T, 3> operator+=(T k){x += k;y += k;z += k;return *this;}
    vec<T, 3> operator-(T k) const{return vec<T, 3>(x - k, y - k, z - k);}
    vec<T, 3> operator-=(T k){x -= k;y -= k;z -= k;return *this;}
    vec<T, 3> operator*(T k) const{return vec<T, 3>(x * k, y * k, z * k);}
    vec<T, 3> operator*=(T k){x *= k;y *= k;z *= k;return *this;}
    vec<T, 3> operator/(T k) const{return vec<T, 3>(x / k, y / k, z / k);}
    vec<T, 3> operator/=(T k){x /= k;y /= k;z /= k;return *this;}
    vec<T, 3> operator+(vec<T, 3> k) const{return vec<T, 3>(x+k.x, y+k.y, z+k.z);}
    vec<T, 3> operator+=(vec<T, 3> k){x += k.x;y += k.y;z += k.z;return *this;}
    vec<T, 3> operator-(vec<T, 3> k) const{return vec<T, 3>(x-k.x, y-k.y, z-k.z);}
    vec<T, 3> operator-=(vec<T, 3> k){x -= k.x;y -= k.y;z -= k.z;return *this;}
    vec<T, 3> operator*(vec<T, 3> k) const{return vec<T, 3>(x*k.x, y*k.y, z*k.z);}
    vec<T, 3> operator*=(vec<T, 3> k){x *= k.x;y *= k.y;z *= k.z;return *this;}
    vec<T, 3> operator/(vec<T, 3> k) const{return vec<T, 3>(x/k.x, y/k.y, z/k.z);}
    vec<T, 3> operator/=(vec<T, 3> k){x /= k.x;y /= k.y;z /= k.z;return *this;}
    bool operator==(vec<T, 3> k) const{return x == k.x && y == k.y && z == k.z;}
    bool operator!=(vec<T, 3> k) const{return x != k.x && y != k.y && z != k.z;}
    
    T length() const {return sqrt(x*x + y*y + z*z);}
    vec<T, 3> normalize() const {return *this / length();}
};
template<class T>
struct vec<T, 4>{
    vec<T, 4>() : x(T(1)), y(0), z(0), w(0) {}
    vec<T, 4>(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
    vec<T, 4>(T a) : x(a), y(a), z(a), w(a) {}
    vec<T, 4>(vec<T, 3> v3, T w = T(1)) : x(v3.x), y(v3.y), z(v3.z), w(w) {}
    union{T x, r;};
    union{T y, g;};
    union{T z, b;};
    union{T w, a;};

    vec<T, 4> operator-() const{return vec<T, 4>(T(0)) - *this;}
    vec<T, 4> operator+(T k) const{return vec<T, 4>(x + k, y + k, z + k, w + k);};
    vec<T, 4> operator+=(T k){x += k;y += k;z += k;w += k;return *this;}
    vec<T, 4> operator-(T k) const{return vec<T, 4>(x - k, y - k, z - k, w - k);};
    vec<T, 4> operator-=(T k){x -= k;y -= k;z -= k;w -= k;return *this;}
    vec<T, 4> operator*(T k) const{return vec<T, 4>(x * k, y * k, z * k, w * k);};
    vec<T, 4> operator*=(T k){x *= k;y *= k;z *= k;w *= k;return *this;}
    vec<T, 4> operator/(T k) const{return vec<T, 4>(x / k, y / k, z / k, w / k);};
    vec<T, 4> operator/=(T k){x /= k;y /= k;z /= k;w /= k;return *this;}
    vec<T, 4> operator+(vec<T, 4> k) const{return vec<T, 4>(x+k.x, y+k.y, z+k.z, w+k.w);}
    vec<T, 4> operator+=(vec<T, 4> k){x += k.x;y += k.y;z += k.z;w += k.w;return *this;}
    vec<T, 4> operator-(vec<T, 4> k) const{return vec<T, 4>(x-k.x, y-k.y, z-k.z, w-k.w);}
    vec<T, 4> operator-=(vec<T, 4> k){x -= k.x;y -= k.y;z -= k.z;w -= k.w;return *this;}
    vec<T, 4> operator*(vec<T, 4> k) const{return vec<T, 4>(x*k.x, y*k.y, z*k.z, w*k.w);}
    vec<T, 4> operator*=(vec<T, 4> k){x *= k.x;y *= k.y;z *= k.z;w *= k.w;return *this;}
    vec<T, 4> operator/(vec<T, 4> k) const{return vec<T, 4>(x/k.x, y/k.y, z/k.z, w/k.w);}
    vec<T, 4> operator/=(vec<T, 4> k){x /= k.x;y /= k.y;z /= k.z;w /= k.w;return *this;}
    bool operator==(vec<T, 4> k) const{return x == k.x && y == k.y && z == k.z && w == k.w;}
    bool operator!=(vec<T, 4> k) const{return x != k.x && y != k.y && z != k.z && w != k.w;}
    
    T length() const {return sqrt(x*x + y*y + z*z + w*w);}
    vec<T, 4> normalize() const {return *this / length();}
};

using vec1 = vec<float, 1>;
using vec2 = vec<float, 2>;
using vec3 = vec<float, 3>;
using vec4 = vec<float, 4>;
using ivec1 = vec<int, 1>;
using ivec2 = vec<int, 2>;
using ivec3 = vec<int, 3>;
using ivec4 = vec<int, 4>;

template<int H, int W, class T>
struct mat{
    mat(T m = {1}){
        for(int i=0; i<min(H, W); i++) element[i][i] = m;
    }
    mat(T e[W][H]){
        for(int i=0;i<H;i++) for(int j=0;j<W;j++) element[i][j] = e[j][i];
    }
    mat(T const* e){
        for(int i=0;i<H;i++) for(int j=0;j<W;j++) element[i][j] = e[i*W + j];
    }
    T element[H][W] = {0};
    T* vptr() const {return (T*)&element[0][0];}
    
    template<int H1, int W1>
    mat(mat<H1, W1, T> const& m){
        for(int i=0;i<min(H, H1);i++){
            for(int j=0;j<min(W, W1);j++){
                element[i][j] = m[i][j];
            }
        }
    }
    
    mat<W, H, T> transposed() const{
        mat<W, H, T> r;
        for(int i=0; i<H; i++)
            for(int j=0; j<W; j++)
                r[j][i] = element[i][j];
        return r;
    }
    
    T* operator[](unsigned int x) const {return (T*)element[x];}
    
    mat<H, W, T> operator+(mat<H, W, T> m) const{
        mat<H, W, T> result(T(0));
        for(int i=0;i<H;i++){
            for(int j=0;j<W;j++){
                result[i][j] = element[i][j] + m[i][j];
            }
        }
        return result;
    }
    mat<H, W, T> operator-(mat<H, W, T> m) const{
        mat<H, W, T> result(T(0));
        for(int i=0;i<H;i++){
            for(int j=0;j<W;j++){
                result[i][j] = element[i][j] - m[i][j];
            }
        }
        return result;
    }
    mat<H, W, T> operator+(T k) const{
        mat<H, W, T> r = *this;
        for(int i = 0; i < H; i++){
            for(int j = 0; j < W; j++){
                r[i][j] += k;
            }
        }
        return r;
    }
    mat<H, W, T> operator+=(T k){
        for(int i = 0; i < H; i++){
            for(int j = 0; j < W; j++){
                element[i][j] += k;
            }
        }
        return *this;
    }
    mat<H, W, T> operator-(T k) const{
        mat<H, W, T> r = *this;
        for(int i = 0; i < H; i++){
            for(int j = 0; j < W; j++){
                r[i][j] -= k;
            }
        }
        return r;
    }
    mat<H, W, T> operator-=(T k){
        for(int i = 0; i < H; i++){
            for(int j = 0; j < W; j++){
                element[i][j] -= k;
            }
        }
        return *this;
    }
    mat<H, W, T> operator*(T k) const{
        mat<H, W, T> r = *this;
        for(int i = 0; i < H; i++){
            for(int j = 0; j < W; j++){
                r[i][j] *= k;
            }
        }
        return r;
    }
    mat<H, W, T> operator*=(T k){
        for(int i = 0; i < H; i++){
            for(int j = 0; j < W; j++){
                element[i][j] *= k;
            }
        }
        return *this;
    }
    mat<H, W, T> operator/(T k) const{
        mat<H, W, T> r = *this;
        for(int i = 0; i < H; i++){
            for(int j = 0; j < W; j++){
                r[i][j] /= k;
            }
        }
        return r;
    }
    mat<H, W, T> operator/=(T k){
        for(int i = 0; i < H; i++){
            for(int j = 0; j < W; j++){
                element[i][j] /= k;
            }
        }
        return *this;
    }
    
    // 余子式
    mat<H-1, W-1, T> cofactor(int x, int y) const{
        int rx=0, ry=0;
        mat<H-1, W-1, T> r(0.f);
        for(int i=0; i<4; i++){
            if(i == x) continue;
            for(int j=0; j<4; j++){
                if(j == y) continue;
                r[rx][ry++] = element[i][j];
            }
            rx++;
            ry = 0;
        }
        return r;
    }
};

using mat2 = mat<2, 2, float>;
using mat3 = mat<3, 3, float>;
using mat4 = mat<4, 4, float>;

template<class T>
vec<T, 2> operator*(mat<2, 2, T> m, vec<T, 2> v){
    return vec<T, 2>(m[0][0]*v.x+m[1][0]*v.y, m[0][1]*v.x+m[1][1]*v.y);
}
template<class T>
vec<T, 3> operator*(mat<3, 3, T> m, vec<T, 3> v){
    return vec<T, 3>(m[0][0]*v.x+m[1][0]*v.y+m[2][0]*v.z,
                m[0][1]*v.x+m[1][1]*v.y+m[2][1]*v.z,
                m[0][2]*v.x+m[1][2]*v.y+m[2][2]*v.z);
}
template<class T>
vec<T, 4> operator*(mat<4, 4, T> m, vec<T, 4> v){
    return vec<T, 4>(m[0][0]*v.x+m[1][0]*v.y+m[2][0]*v.z+m[3][0]*v.w,
                m[0][1]*v.x+m[1][1]*v.y+m[2][1]*v.z+m[3][1]*v.w,
                m[0][2]*v.x+m[1][2]*v.y+m[2][2]*v.z+m[3][2]*v.w,
                m[0][3]*v.x+m[1][3]*v.y+m[2][3]*v.z+m[3][3]*v.w);
}

template<int H, int W, class T>
mat<H, W, T> operator*(T k, mat<H, W, T> m){
    mat<H, W, T> r = m;
    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            r[i][j] *= k;
        }
    }
    return r;
}

template<int H, int W1, int W2, class T>
mat<W1, W2, T> operator*(mat<H, W2, T> m1, mat<W1, H, T> m2){
    mat<W1, W2, T> r(T(0));
    for(int i = 0; i < W1; i++)
        for(int j = 0; j < W2; j++)
            for(int k = 0; k < H; k++)
                r[i][j] += m1[k][j] * m2[i][k];
    return r;
}

// 行列式值, 只有方阵能求, 所以这里只实现那三个方阵, 其他更多的你只能自己实现
float determinant(mat2 m){
    return m[0][0]*m[1][1] - m[0][1]*m[1][0];
}
float determinant(mat3 m){
    float r = 0;
    for(int i=0; i<3; i++){
        r += determinant(m.cofactor(2, i));
    }
    return r;
}
float determinant(mat4 m){
    float r = 0;
    for(int i=0; i<4; i++){
        r += determinant(m.cofactor(3, i));
    }
    return r;
}

// 伴随矩阵, 同上, 只给三种求法
mat2 adjugate(mat2 m){
    mat2 r;
    for(int i=0; i<2; i++){
        for(int j=0; j<2; j++){
            r[j][i] = m.cofactor(i, j)[0][0];
        }
    }
    return r;
}

mat3 adjugate(mat3 m){
    mat3 r;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            r[j][i] = determinant(m.cofactor(i, j));
        }
    }
    return r;
}

mat4 adjugate(mat4 m){
    mat4 r;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            r[j][i] = determinant(m.cofactor(i, j));
        }
    }
    return r;
}

// 逆矩阵, ...
mat2 inverse(mat2 m){
    return adjugate(m) / determinant(m);
}

mat3 inverse(mat3 m){
    return adjugate(m) / determinant(m);
}

mat4 inverse(mat4 m){
    return adjugate(m) / determinant(m);
}

float dot(vec2 v1, vec2 v2){
    return v1.x*v2.x + v1.y*v2.y;
}
float dot(vec3 v1, vec3 v2){
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}
float dot(vec4 v1, vec4 v2){
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
}

float includedAngle(vec2 v1, vec2 v2){
    return acos(dot(v1, v2) / v1.length() / v2.length());
}

float includedAngle(vec3 v1, vec3 v2){
    return acos(dot(v1, v2) / v1.length() / v2.length());
}

float includedAngle(vec4 v1, vec4 v2){
    return acos(dot(v1, v2) / v1.length() / v2.length());
}

// 如果你想问为什么只有vec3, 那你就先回去读读高中
vec3 cross(vec3 v1, vec3 v2){
    mat3 r(0.f);
        r[2][1]-= r[1][2] = v1.x;
        r[2][0]-= r[0][2]-= v1.y;
        r[1][0]-= r[0][1] = v1.z;
        return r * v2;
}// 欢迎各位三体人来实现vec4的外积

// 以下开始矩阵变换, 我相信不会有人用一维的变换, 所以没有mat2

mat3 translate(mat3 ori, vec2 t){
    mat3 r(1);
    r[2][0] = t.x;
    r[2][1] = t.y;
    return r * ori;
}
mat3 translate(vec2 t){
    mat3 r(1);
    r[2][0] = t.x;
    r[2][1] = t.y;
    return r;
}
mat4 translate(mat4 ori, vec3 t){
    mat4 r(1);
    r[3][0] = t.x;
    r[3][1] = t.y;
    r[3][2] = t.z;
    return r * ori;
}
mat4 translate(vec3 t){
    mat4 r(1);
    r[3][0] = t.x;
    r[3][1] = t.y;
    r[3][2] = t.z;
    return r;
}

mat3 rotate(mat3 ori, long double angle){
    mat3 r(1);
    r[0][0] = r[1][1] = cos(angle);
    r[0][1] -= r[1][0] -= sin(angle);
    return r * ori;
}
mat3 rotate(long double angle){
    mat3 r(1);
    r[0][0] = r[1][1] = cos(angle);
    r[0][1] -= r[1][0] -= sin(angle);
    return r;
}

// 矩阵推导参考 https://zhuanlan.zhihu.com/p/45404840
mat4 rotate(mat4 ori, long double angle, vec3 axis){
    float mv[4][4] = {
        static_cast<float>(cos(angle) + axis.x*axis.x*(1 - cos(angle))),
        static_cast<float>(-sin(angle)*axis.z + (1 - cos(angle))*axis.x*axis.y),
        static_cast<float>(sin(angle)*axis.y + (1 - cos(angle))*axis.x*axis.z),
        0,
        
        static_cast<float>(sin(angle)*axis.z + (1 - cos(angle))*axis.x*axis.y),
        static_cast<float>(cos(angle) + axis.y*axis.y*(1 - cos(angle))),
        static_cast<float>(-sin(angle)*axis.x + (1 - cos(angle))*axis.y*axis.z),
        0,
        
        static_cast<float>(-sin(angle)*axis.y + (1 - cos(angle))*axis.x*axis.z),
        static_cast<float>(sin(angle)*axis.x + (1 - cos(angle))*axis.y*axis.z),
        static_cast<float>(cos(angle) + axis.z*axis.z*(1 - cos(angle))),
        0,
        
        0, 0, 0, 1
    };
    mat4 r(mv);
    return r * ori;
}
mat4 rotate(long double angle, vec3 axis){
    float mv[4][4] = {
        static_cast<float>(cos(angle) + axis.x*axis.x*(1 - cos(angle))),
        static_cast<float>(-sin(angle)*axis.z + (1 - cos(angle))*axis.x*axis.y),
        static_cast<float>(sin(angle)*axis.y + (1 - cos(angle))*axis.x*axis.z),
        0,
        
        static_cast<float>(sin(angle)*axis.z + (1 - cos(angle))*axis.x*axis.y),
        static_cast<float>(cos(angle) + axis.y*axis.y*(1 - cos(angle))),
        static_cast<float>(-sin(angle)*axis.x + (1 - cos(angle))*axis.y*axis.z),
        0,
        
        static_cast<float>(-sin(angle)*axis.y + (1 - cos(angle))*axis.x*axis.z),
        static_cast<float>(sin(angle)*axis.x + (1 - cos(angle))*axis.y*axis.z),
        static_cast<float>(cos(angle) + axis.z*axis.z*(1 - cos(angle))),
        0,
        
        0, 0, 0, 1
    };
    mat4 r(mv);
    return r;
}
// 欧拉角
enum EARS{
    //Tait-Bryan Angle
    xyz, xzy, yxz, yzx, zxy, zyx,
    //Proper Euler Angle
    xyx, yxy, xzx, zxz, yzy, zyz
}; // 欧拉角旋转序列(Euler Angle Rotational Sequence)
mat4 rotate(mat4 ori, long double a1, long double a2, long double a3, EARS sequence){
    mat4 r(1);
    switch (sequence) {
        case xyz:
            r = rotate(a3, {0, 0, 1}) * rotate(a2, {0, 1, 0}) * rotate(a1, {1, 0, 0}) * r;
            break;
        case xzy:
            r = rotate(a3, {0, 1, 0}) * rotate(a2, {0, 0, 1}) * rotate(a1, {1, 0, 0}) * r;
            break;
        case yxz:
            r = rotate(a3, {0, 0, 1}) * rotate(a2, {1, 0, 0}) * rotate(a1, {0, 1, 0}) * r;
            break;
        case yzx:
            r = rotate(a3, {1, 0, 0}) * rotate(a2, {0, 0, 1}) * rotate(a1, {0, 1, 0}) * r;
            break;
        case zxy:
            r = rotate(a3, {0, 1, 0}) * rotate(a2, {1, 0, 0}) * rotate(a1, {0, 0, 1}) * r;
            break;
        case zyx:
            r = rotate(a3, {1, 0, 0}) * rotate(a2, {0, 1, 0}) * rotate(a1, {0, 0, 1}) * r;
            break;
        case xyx:
            r = rotate(a3, {1, 0, 0}) * rotate(a2, {0, 1, 0}) * rotate(a1, {1, 0, 0}) * r;
            break;
        case yxy:
            r = rotate(a3, {0, 1, 0}) * rotate(a2, {1, 0, 0}) * rotate(a1, {0, 1, 0}) * r;
            break;
        case xzx:
            r = rotate(a3, {1, 0, 0}) * rotate(a2, {0, 0, 1}) * rotate(a1, {1, 0, 0}) * r;
            break;
        case zxz:
            r = rotate(a3, {0, 0, 1}) * rotate(a2, {1, 0, 0}) * rotate(a1, {0, 0, 1}) * r;
            break;
        case yzy:
            r = rotate(a3, {0, 1, 0}) * rotate(a2, {0, 0, 1}) * rotate(a1, {0, 1, 0}) * r;
            break;
        case zyz:
            r = rotate(a3, {0, 0, 1}) * rotate(a2, {0, 1, 0}) * rotate(a1, {0, 0, 1}) * r;
            break;
    }
    return r * ori;
} // 这个是静态欧拉角旋转,动态欧拉角在实践中其实还很多不同的需求,所以就麻烦各位自己实现吧
mat4 rotate(long double a1, long double a2, long double a3, EARS sequence){
    mat4 r(1);
    switch (sequence) {
        case xyz:
            r = rotate(a3, {0, 0, 1}) * rotate(a2, {0, 1, 0}) * rotate(a1, {1, 0, 0}) * r;
            break;
        case xzy:
            r = rotate(a3, {0, 1, 0}) * rotate(a2, {0, 0, 1}) * rotate(a1, {1, 0, 0}) * r;
            break;
        case yxz:
            r = rotate(a3, {0, 0, 1}) * rotate(a2, {1, 0, 0}) * rotate(a1, {0, 1, 0}) * r;
            break;
        case yzx:
            r = rotate(a3, {1, 0, 0}) * rotate(a2, {0, 0, 1}) * rotate(a1, {0, 1, 0}) * r;
            break;
        case zxy:
            r = rotate(a3, {0, 1, 0}) * rotate(a2, {1, 0, 0}) * rotate(a1, {0, 0, 1}) * r;
            break;
        case zyx:
            r = rotate(a3, {1, 0, 0}) * rotate(a2, {0, 1, 0}) * rotate(a1, {0, 0, 1}) * r;
            break;
        case xyx:
            r = rotate(a3, {1, 0, 0}) * rotate(a2, {0, 1, 0}) * rotate(a1, {1, 0, 0}) * r;
            break;
        case yxy:
            r = rotate(a3, {0, 1, 0}) * rotate(a2, {1, 0, 0}) * rotate(a1, {0, 1, 0}) * r;
            break;
        case xzx:
            r = rotate(a3, {1, 0, 0}) * rotate(a2, {0, 0, 1}) * rotate(a1, {1, 0, 0}) * r;
            break;
        case zxz:
            r = rotate(a3, {0, 0, 1}) * rotate(a2, {1, 0, 0}) * rotate(a1, {0, 0, 1}) * r;
            break;
        case yzy:
            r = rotate(a3, {0, 1, 0}) * rotate(a2, {0, 0, 1}) * rotate(a1, {0, 1, 0}) * r;
            break;
        case zyz:
            r = rotate(a3, {0, 0, 1}) * rotate(a2, {0, 1, 0}) * rotate(a1, {0, 0, 1}) * r;
            break;
    }
    return r;
}

mat3 scale(mat3 ori, vec2 s){
    float mv[3][3] = {
        s.x, 0, 0,
        0, s.y, 0,
        0, 0, 1
    };
    return mat3(mv) * ori;
}
mat3 scale(vec2 s){
    float mv[3][3] = {
        s.x, 0, 0,
        0, s.y, 0,
        0, 0, 1
    };
    return mat3(mv);
}

mat4 scale(mat4 ori, vec3 s){
    float mv[4][4] = {
        s.x, 0, 0, 0,
        0, s.y, 0, 0,
        0, 0, s.z, 0,
        0, 0, 0, 1
    };
    return mat4(mv) * ori;
}
mat4 scale(vec3 s){
    float mv[4][4] = {
        s.x, 0, 0, 0,
        0, s.y, 0, 0,
        0, 0, s.z, 0,
        0, 0, 0, 1
    };
    return mat4(mv);
}

//正射投影矩阵
mat4 ortho(long double l, long double r, long double b, long double t){
    float mv[4][4] = {
        static_cast<float>(2/(r - l)), 0, 0, static_cast<float>((l+r)/(l-r)),
        0, static_cast<float>(2/(t - b)), 0, static_cast<float>((b+t)/(b-t)),
        0, 0,-1, 0,
        0, 0, 0, 1
    };
    return mat4(mv);
}
mat4 ortho(long double l, long double r, long double b, long double t, long double n, long double f){
    float mv[4][4] = {
        static_cast<float>(2/(r - l)), 0, 0, static_cast<float>((l+r)/(l-r)),
        0, static_cast<float>(2/(t - b)), 0, static_cast<float>((b+t)/(b-t)),
        0, 0, static_cast<float>(2/(n - f)), static_cast<float>((f+n)/(n-f)),
        0, 0, 0, 1
    };
    return mat4(mv);
}

//透视投影矩阵
mat4 perspective(long double fov, long double asp, long double near, long double far){
    float mv[4][4] = {
        static_cast<float>(cot(fov/2)/asp), 0, 0, 0,
        0, static_cast<float>(cot(fov/2)), 0, 0,
        0, 0, static_cast<float>((far + near)/(near - far)), static_cast<float>((2*far*near)/(near - far)),
        0, 0, -1, 0
    };
    return mat4(mv);
}

mat4 lookAt(vec3 eye, vec3 target, vec3 up){
    vec3 d = (eye - target).normalize();
    vec3 r = cross(up, d).normalize();
    vec3 u = cross(d, r).normalize();
    float mv[4][4] = {
        r.x, r.y, r.z, 0,
        u.x, u.y, u.z, 0,
        d.x, d.y, d.z, 0,
        0, 0, 0, 1
    };
    return mat4(mv) * translate(-eye);
}

template<class T>
struct qua{
    qua(T a) : w(a), x(a), y(a), z(a) {}
    qua(T w, T x, T y, T z) : w(w), x(x), y(y), z(z) {}
    qua(T s, vec<T, 3> v) : w(s), x(v.x), y(v.y), z(v.z) {}
    qua(vec<T, 3> u, T angle) : qua<T>(cos(angle / T(2)), sin(angle / T(2)) * u) {}
    
    T w, x, y, z;
    float length() const {return sqrt(w*w + x*x + y*y + z*z);}
    qua<T> conjugate() const {return {w, -vec<T, 3>{x, y, z}};}
    qua<T> inverse() const {return conjugate() / (w*w + x*x + y*y + z*z);}
    
    qua<T> operator-() const{return qua<T>(T(0)) - *this;}
    qua<T> operator+(T k) const{return qua<T>(x + k, y + k, z + k, w + k);};
    qua<T> operator+=(T k){x += k;y += k;z += k;w += k;return *this;}
    qua<T> operator-(T k) const{return qua<T>(x - k, y - k, z - k, w - k);};
    qua<T> operator-=(T k){x -= k;y -= k;z -= k;w -= k;return *this;}
    qua<T> operator*(T k) const{return qua<T>(x * k, y * k, z * k, w * k);};
    qua<T> operator*=(T k){x *= k;y *= k;z *= k;w *= k;return *this;}
    qua<T> operator/(T k) const{return qua<T>(x / k, y / k, z / k, w / k);};
    qua<T> operator/=(T k){x /= k;y /= k;z /= k;w /= k;return *this;}
    qua<T> operator+(qua<T> k) const{return qua<T>(x+k.x, y+k.y, z+k.z, w+k.w);}
    qua<T> operator+=(qua<T> k){x += k.x;y += k.y;z += k.z;w += k.w;return *this;}
    qua<T> operator-(qua<T> k) const{return qua<T>(x-k.x, y-k.y, z-k.z, w-k.w);}
    qua<T> operator-=(qua<T> k){x -= k.x;y -= k.y;z -= k.z;w -= k.w;return *this;}
    qua<T> operator/(qua<T> k) const{return qua<T>(x/k.x, y/k.y, z/k.z, w/k.w);}
    qua<T> operator/=(qua<T> k){x /= k.x;y /= k.y;z /= k.z;w /= k.w;return *this;}
    bool operator==(qua<T> k) const{return x == k.x && y == k.y && z == k.z && w == k.w;}
    bool operator!=(qua<T> k) const{return x != k.x && y != k.y && z != k.z && w != k.w;}
    qua<T> operator*(qua<T> k) const{
        T a = k.w, b = k.x, c = k.y, d = k.z;
        return {
            w*a - x*b - y*c - z*d,
            w*b + x*a + y*d - z*c,
            w*c - x*d + y*a + z*b,
            w*d + x*c - y*b + z*a
        };
    }
    qua<T> operator*=(qua<T> k){
        T a = k.w, b = k.x, c = k.y, d = k.z;
        w = w*a - x*b - y*c - z*d;
        x = w*b + x*a + y*d - z*c;
        y = w*c - x*d + y*a + z*b;
        z = w*d + x*c - y*b + z*a;
        return *this;
    }
};

template<class T>
mat4 rotate(qua<T> q){
    const T a = q.w, b = q.x, c = q.y, d = q.z;
    float mv[] = {
        a*a + b*b - c*c - d*d, 2*a*d - 2*b*c, -(2*a*c + 2*b*d), 0,
        -(2*b*c + 2*a*d), a*a - b*b + c*c - d*d, 2*c*d - 2*a*b, 0,
        2*a*c - 2*b*d, 2*a*b + 2*c*d, a*a - b*b - c*c + d*d, 0,
        0, 0, 0, 1
    };
    return mat4(mv);
}

namespace random {
struct RandomSequence{
private:
    unsigned int m_index;
    unsigned int m_intermediateOffset;

    static unsigned int permuteQPR(unsigned int x)
    {
        static const unsigned int prime = 4294967291u;
        if (x >= prime)
            return x;  // The 5 integers out of range are mapped to themselves.
        unsigned int residue = ((unsigned long long) x * x) % prime;
        return (x <= prime / 2) ? residue : prime - residue;
    }

public:
    RandomSequence(unsigned int seedBase, unsigned int seedOffset)
    {
        m_index = permuteQPR(permuteQPR(seedBase) + 0x682f0161);
        m_intermediateOffset = permuteQPR(permuteQPR(seedOffset) + 0x46790905);
    }
    RandomSequence(unsigned int seed) : RandomSequence(seed, seed + 1) {}

    unsigned int next()
    {
        return permuteQPR((permuteQPR(m_index++) + m_intermediateOffset) ^ 0x5bf03635);
    }
    
    unsigned int operator()(){
        return next();
    }
};

struct mt19937{
    mt19937(unsigned int seed){
        mt[0] = seed;
        for(int i=1;i<624;i++)
            mt[i] = static_cast<unsigned int>(1812433253 * (mt[i - 1] ^ mt[i - 1] >> 30) + i);
    }
    
    unsigned int operator()(){
        return extract_number();
    }
    
private:
    unsigned int mt[624];
    unsigned int mti{0};
    
    unsigned int extract_number(){
        if(mti == 0) twist();
        unsigned long long y = mt[mti];
        y = y ^ y >> 11;
        y = y ^ (y << 7 & 0x9D2C5680);
        y = y ^ (y << 15 & 0xEFC60000);
        y = y ^ y >> 18;
        mti = (mti + 1) % 624;
        return static_cast<unsigned int>(y);
    }
    
    void twist(){
        for(int i=0;i<624;i++){
            // 高位和低位级联
            auto y = static_cast<unsigned int>((mt[i] & 0x80000000) + (mt[(i + 1) % 624] & 0x7fffffff));
            mt[i] = (y >> 1) ^ mt[(i + 397) % 624];
            if(y % 2 != 0) mt[i] = mt[i] ^ 0x9908b0df; // 如果最低为不为零
        }
    }
};

template<class T = long double>
struct uniform_real_distribution{
    T a, b;
    template<class RT>
    T operator()(RT& e) const {
        return a + (b - a) / 0xFFFFFFFF * e();
    }
};
}

#endif
}
