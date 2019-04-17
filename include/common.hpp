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

 /** \brief Convenience method to determine whether point is valid
   *
   * \param r The row index
   * \param c The column index
   * \param r_max The number of rows in the matrix
   * \param r_min The number of columns in the matrix
   */
inline bool pointInRange(int r, int c, int r_max, int c_max)
{
  return (r >= 0) && (r < r_max) && (c >= 0) && (c < c_max);
}
