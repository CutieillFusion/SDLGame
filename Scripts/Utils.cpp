#include "Utils.h"

#include <algorithm>  //std::generate
#include <array>      //std::array
#include <functional> //std::ref
#include <random>     //std::mt19937, std::seed_seq, std::random_device

namespace Utils 
{
    std::mt19937& GetRNGEngine() 
    {
        static std::mt19937 mt = []() {
            std::array<std::mt19937::result_type, std::mt19937::state_size> data;
            std::random_device device;
            std::generate(data.begin(), data.end(), std::ref(device));
            std::seed_seq seed(data.begin(), data.end());
            mt.seed(seed);
            return mt;
        }();

        return mt;
    }

    int RandU(int limit)
    {
        return Random_(0, limit - 1);
    }

    unsigned int RandUI(unsigned int limit)
    {
        return Random_(0u, limit - 1u);
    }
}
