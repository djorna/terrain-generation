#pragma once

#include <numeric>
#include <vector>

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
