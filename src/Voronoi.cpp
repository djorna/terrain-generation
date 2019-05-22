#include "Voronoi.hpp"

#include <opencv2/flann/defines.h>
#include <opencv2/flann/flann_base.hpp>
#include <opencv2/flann/kdtree_single_index.h>
#include <opencv2/flann/dist.h>

#include <omp.h> // OpenMP for multithreading

#include <numeric>


namespace terrain
{

Voronoi::Voronoi(int rows, int cols, std::vector<float> coeffs, int n_points, int seed)
  : rows(rows), cols(cols), coeffs(coeffs), seed(seed)
{
  generatePoints(n_points, rows, cols);
  multipliers.resize(n_points, 1);
}

Voronoi::Voronoi(int rows, int cols, std::vector<float> coeffs, PointList points, int seed)
  : rows(rows), cols(cols), coeffs(coeffs), points(points), seed(seed)
{
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

  std::uniform_int_distribution<int> dist(0, rows * cols - 1);

  // Random rand_row(0, rows, seed);
  // Random rand_col(0, cols, seed);
  for (int i = 0; i < n_points; ++i)
  {
    int index = dist(pseudo_rand_engine);
    int x = index % cols;
    int y = index / cols;
    // int x = rand_row.next<int>();
    // int y = rand_col.next<int>();
    points.push_back(Point(x, y));
    points_norm.push_back(static_cast<float>(x) / cols);
    points_norm.push_back(static_cast<float>(y) / rows);
  }
}


void Voronoi::drawPoints(cv::Mat& img)
{
  if (img.type() != CV_32FC1)
  {
    std::cerr << "Wrong image type. Required: CV_32FC1";
    return;
  }
  try 
  {
    for (Point point : points)
    {
      img.at<float>(point.y, point.x) = 1;
    }
  }
  catch(...)
  {
    std::cerr << "Point not initialized. Run generatePoints() before draw().";
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
  seed_rand(seed);
  std::shuffle(indices.begin(), indices.end(), pseudo_rand_engine); // Uniformly shuffle indices
  // random.shuffle(indices); // Uniformly shuffle indices
  for (int i = 0; i < (1.0f - keep) * static_cast<float>(indices.size()); ++i)
    multipliers[indices[i]] = 0;
}


void Voronoi::shiftHeightMask(float mean, float stdev, int seed)
{
  seed_rand(seed);
  // std::uniform_real_distribution<float> uniform(0, 1);
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
    else
    {
      pseudo_rand_engine.discard(1);
    }
  }
}

cv::Mat Voronoi::generate()
{
  heatmap = cv::Mat::zeros(rows, cols, CV_32FC1);

  cvflann::SearchParams searchParams(1, 0.5, false);
  cvflann::KDTreeSingleIndexParams indexParams(10);

  cvflann::Matrix<float> features(&points_norm[0], points.size(), 2);

  // Construct ad KD-Tree using squared euclidean distance (L2)
  cvflann::KDTreeSingleIndex<cvflann::L2_Simple<float>> flann_index(features, indexParams);
  flann_index.buildIndex(); // Why is this not part of the constructor??

  const int knn = coeffs.size();

  cvflann::Matrix<float> query;
  query.rows = 1;
  query.cols = 2; 
  cvflann::Matrix<int> indices; // rows: query #, col: ith nearest neighbour
  indices.rows = 1;
  indices.cols = knn;
  cvflann::Matrix<float> dists; // rows: query #, col: ith nearest neighbour
  dists.rows = 1;
  dists.cols = knn;
  std::array<float, 2> query_data;
  std::vector<int> indices_data(knn);
  std::vector<float> dists_data(knn);

#pragma omp parallel for num_threads(4) firstprivate(indices, dists, query, indices_data, dists_data, query_data)
  for (int i = 0; i < rows; ++i)
  {
    for (int j = 0; j < cols; ++j)
    {
      query_data = { static_cast<float>(j) / cols, static_cast<float>(i) / rows };
      query.data = &query_data[0];
      indices.data = &indices_data[0];
      dists.data = &dists_data[0];

      flann_index.knnSearch(query, indices, dists, knn, searchParams);

      // Multiply nearest neighbours by their respective coefficients
      float pixel_value = 0;
      for (int c = 0; c < coeffs.size(); ++c) {
        pixel_value += coeffs[c] * dists[0][c];
      }

      heatmap.at<float>(i, j) = pixel_value * multipliers[indices[0][0]];
    }
  }
  
  cv::normalize(heatmap, heatmap, 1, 0, cv::NORM_MINMAX);

  return heatmap;
}

} // namespace terrain