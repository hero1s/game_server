
#pragma once

#include <vector>
#include <map>
#include <time.h>
#include <random>
#include <functional>
#include "utility/basic_types.h"

using namespace std;

//随机数生成类

class RandGen {
private:
    default_random_engine engine;
public:
    RandGen(const uint32_t p_Seed = 0) {
        engine.seed(p_Seed);
    }

    //ReSeed the random number generator
    //种子处理
    void Reset(const uint32_t p_Seed = 0) {
        engine.seed(p_Seed);
        RandUInt();
    }

    //Returns an unsigned integer from 0..RandomMax
    //0~RandMax uint 随机数
    uint32_t RandUInt() {
        return engine();
    }

    inline bool RandRatio(uint32_t uRatio, uint32_t uRatioMax = 10000) {
        if (uRatio == 0)
            return false;

        return (uRatio >= (uint32_t) RandRange(uRatioMax, 0));
    }

    ///[min.max]
    inline int RandRange(int min, int max) {
        std::uniform_int_distribution<int> dis(min, max);
        auto dice = std::bind(dis, engine);
        return dice();
    }

    ///[min,max)
    inline double RandDouble(double min, double max) {
        std::uniform_real_distribution<double> dis(min, max);
        auto dice = std::bind(dis, engine);
        return dice();
    }

    template<typename TKey, typename TWeight>
    inline TKey rand_weight(const std::map<TKey, TWeight> &data) {
        TWeight total = 0;
        for (auto &it : data)
        {
            total += it.second;
        }
        if (total == 0)
        {
            return 0;
        }
        TWeight rd = RandRange(1, total);

        TWeight tmp = 0;
        for (auto &it : data)
        {
            tmp += it.second;
            if (rd <= tmp)
            {
                return it.first;
            }
        }
        return 0;
    }

};

extern RandGen g_RandGen;



