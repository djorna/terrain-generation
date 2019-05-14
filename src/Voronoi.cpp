#include "Voronoi.hpp"

#include <iostream>

namespace terrain
{

Voronoi::Voronoi(int rows, int cols, std::vector<float> coeffs, int n_points, int seed)
  : rows(rows), cols(cols), coeffs(coeffs), seed(seed)
{
  generatePoints(n_points, rows, cols);
  n_coeffs = coeffs.size();
  multipliers.resize(n_points, 1);
}

Voronoi::Voronoi(int rows, int cols, std::vector<float> coeffs, PointList points, int seed)
  : rows(rows), cols(cols), coeffs(coeffs), points(points), seed(seed)
{
  n_coeffs = coeffs.size();
  multipliers.resize(points.size(), 1);
}

Voronoi::~Voronoi() {}

void Voronoi::generatePoints(int n_points, int rows, int cols)
{
  // std::random_device random_device;
  // std::mt19937 pseudo_rand_engine;
  /*
  if (seed > 0)
    pseudo_rand_engine.seed(seed);
  else
    pseudo_rand_engine.seed(random_device());
  */
  // seed_rand(seed);

  std::uniform_int_distribution<int> row_distribution(0, rows);
  std::uniform_int_distribution<int> col_distribution(0, cols);
  // Random rand_row(0, rows, seed);
  // Random rand_col(0, cols, seed);
  for (int i = 0; i < n_points; ++i)
  {// 
    int x = col_distribution(pseudo_rand_engine);
    int y = row_distribution(pseudo_rand_engine);
    // int x = rand_row.next<int>();
    // int y = rand_col.next<int>();
    points.push_back(Point(x, y));
  }
}


void Voronoi::drawPoints(cv::Mat& img)
{
  if (img.type() != CV_32FC1)
  {
    std::cerr << "Wrong image type required: CV_32FC1";
    return;
  }
  try 
  {
    for (Point point : points)
    {
      std::cout << "Point:" << point << std::endl;
      img.at<float>(point.y, point.x) = 1;
    }
  }
  catch(...)
  {
    std::cerr << "Point not initialized. Run generatePoints before draw.";
  }
}


void Voronoi::binaryMask(float keep, int seed)
{
  if (keep >= 1) return;
  std::vector<int> indices(points.size());
  std::iota(std::begin(indices), std::end(indices), 0); // Get numbers from 0 to n
  // seed_rand(seed);
  // random.seed(seed);
  // Random random(seed);
  // std::shuffle(indices.begin(), indices.end(), pseudo_rand_engine); // Uniformly shuffle indices
  // random.shuffle(indices); // Uniformly shuffle indices
  for (int i = 0; i < (1.0f - keep) * indices.size(); ++i)
    multipliers[indices[i]] = 0;
}


void Voronoi::shiftHeightMask(float mean, float stdev, int seed)
{
  std::uniform_real_distribution<float> uniform(0, 1);
  std::normal_distribution<float> normal(mean, stdev);
  // random.seed(seed);
  // Random random(seed);
  // random.set_norm(mean, stdev);
  // seed_rand(seed);
  for (int i = 0; i < multipliers.size(); ++i)
  {
    if (multipliers[i] > 0)
    {
      float value = normal(pseudo_rand_engine);
      // float value = random.randf();
      if (value < 0) value = 0;
      if (value > 1) value = 1;
      multipliers[i] *= value;
    }
  }
}

cv::Mat Voronoi::generate()
{
  heatmap = cv::Mat::zeros(rows, cols, CV_32FC1);

  // http://answers.opencv.org/question/107683/how-to-use-cvflann-radiussearch-to-find-all-neighbouring-points-within-radius-r-in-2d-using-euclidean-distance/
  // https://docs.opencv.org/4.1.0/db/d18/classcv_1_1flann_1_1GenericIndex.html#a50c3ce570adfb7b19c7cda4a320a3d9e

  cv::flann::SearchParams searchParams(128, 0, false);
  cv::flann::KDTreeIndexParams indexParams;

  cv::Mat_<float> features(0, 2);

  for (auto && point : points) {
    //Fill matrix
    float norm_x = static_cast<float>(point.x) / cols;
    float norm_y = static_cast<float>(point.y) / rows;
    cv::Mat row = (cv::Mat_<float>(1, 2) << norm_x, norm_y);
    features.push_back(row);
  }
  cv::flann::Index flann_index(features, cv::flann::KDTreeIndexParams(2), cvflann::EUCLIDEAN);

  const float max_dist = 0.2;

  // Brute force approach
  for (int i = 0; i < rows; ++i)
  {
    for (int j = 0; j < cols; ++j)
    {
      cv::Mat query = (cv::Mat_<float>(1, 2) << (float)j/(float)cols, (float)i/(float)rows);
      cv::Mat indices, dists;
      flann_index.knnSearch(query, indices, dists, n_coeffs, searchParams);
      // int n_neighbours = flann_index.radiusSearch(query, indices, dists, 1, n_coeffs, searchParams);

      // int index_0 = indices.at<int>(0);
      // int index_1 = indices.at<int>(1);

      float pixel_value = 0;
      for (int c = 0; c < n_coeffs; ++c) {
        float dist = dists.at<float>(0, c);
        // std::cout << dist << '\n';
        //if (dist < max_dist)
          pixel_value += coeffs[c] * dists.at<float>(0, c);
      }
      /*
      for (int c = 0; c < n_neighbours; ++c)
        pixel_value += coeffs[c] * dists.at<float>(0, c);
      */

      heatmap.at<float>(i, j) = pixel_value * multipliers[indices.at<int>(0)];
    }
  }
  
  cv::normalize(heatmap, heatmap, 1, 0, cv::NORM_MINMAX);

  return heatmap;
}

} // namespace terrain