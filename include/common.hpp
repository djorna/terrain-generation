#pragma once

#include <random>
#include <numeric>
#include <ctime>

static std::random_device random_device;
static std::mt19937 pseudo_rand_engine;

template<typename T>
inline T rand(const T min, const T max) { return min + static_cast <T> (rand()) / (static_cast<T>(RAND_MAX/(max - min))); }

inline float randf(const float min=0, const float max=1) { return rand<float>(max, min); }

inline int randi(const int min, const int max) { return rand<int>(min, max); }

inline void seed_rand(int seed)
{
  // seed > 0 ? srand(seed) : srand(time(0));
  if (seed > 0)
    pseudo_rand_engine.seed(seed);
  else
    pseudo_rand_engine.seed(random_device());
}

inline float average(const std::vector<float> vec)
{ 
  return std::accumulate(vec.begin(), vec.end(), 0.0) / static_cast<float>(vec.size());
}

inline float sum(const std::vector<float> vec)
{
  return std::accumulate(vec.begin(), vec.end(), 0.0);
}

 /** \brief Convenience method to determine whether point is valid
   *
   * \param r The row index
   * \param c The column index
   * \param r_max The number of rows in the matrix
   * \param r_min The number of columns in the matrix
   */
inline bool pointInRange(const int r, const int c, const int r_max, const int c_max)
{
  return (r >= 0) && (r < r_max) && (c >= 0) && (c < c_max);
}
