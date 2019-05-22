#pragma once

#include <random>

static std::random_device random_device;
static std::mt19937 pseudo_rand_engine;

template<typename T>
inline T rand(const T min, const T max) { return min + static_cast <T> (rand()) / (static_cast<T>(RAND_MAX/(max - min))); }

inline float randf(const float min=0, const float max=1) { return rand<float>(max, min); }
//inline float randf() { return (*dis_float)(pseudo_rand_engine); }

inline int randi(const int min, const int max) { return rand<int>(min, max); }

inline void seed_rand(int seed)
{
  // seed > 0 ? srand(seed) : srand(time(0));
  if (seed > 0)
    pseudo_rand_engine.seed(seed);
  else
    pseudo_rand_engine.seed(random_device());
}