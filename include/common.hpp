#pragma once

#include <random>
#include <numeric>

template<typename T>
inline T rand(T min, T max) { return min + static_cast <T> (rand()) / (static_cast <T> (RAND_MAX/(max - min))); }

inline float randf(float min = 0, float max = 1) { return rand<float>(max, min); }

inline int randi(int min, int max) { return rand<int>(min, max); }

inline float average(std::vector<float> vec)
{ 
  float sum = 0;
  for (auto val : vec)
    sum += val;
  sum /= static_cast<float>(vec.size());
  return sum;
}
