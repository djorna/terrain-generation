#include "Voronoi.hpp"

Voronoi::Voronoi(int rows, int cols, std::vector<float> coeffs, int n_points)
  : rows(rows), cols(cols), coeffs(coeffs)
{
  generatePoints(n_points, rows, cols);
  n_coeffs = coeffs.size();
  srand(999);
}

Voronoi::Voronoi(int rows, int cols, std::vector<float> coeffs, PointList points)
  : rows(rows), cols(cols), coeffs(coeffs), points(points)
{
  
}

Voronoi::~Voronoi() {}

void Voronoi::generatePoints(int n_points, int rows, int cols)
{
  // std::default_random_engine generator;
  std::random_device generator;
  std::uniform_int_distribution<int> row_distribution(0, rows);
  std::uniform_int_distribution<int> col_distribution(0, cols);
  for (int i = 0; i < n_points; ++i)
  {
    int x = col_distribution(generator);
    int y = row_distribution(generator);
    points.push_back(cv::Point2i(x, y));
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
    for (cv::Point2i point : points)
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

cv::Mat Voronoi::generate()
{
  heatmap = cv::Mat::zeros(rows, cols, CV_32FC1);

  // http://answers.opencv.org/question/107683/how-to-use-cvflann-radiussearch-to-find-all-neighbouring-points-within-radius-r-in-2d-using-euclidean-distance/
  // https://docs.opencv.org/4.1.0/db/d18/classcv_1_1flann_1_1GenericIndex.html#a50c3ce570adfb7b19c7cda4a320a3d9e

  cv::flann::SearchParams searchParams(64, 0, false);
  cv::flann::KDTreeIndexParams indexParams;

  cv::Mat_<float> features(0, 2);

  for (auto && point : points) {
    //Fill matrix
    float norm_x = point.x / cols;
    float norm_y = point.y / rows;
    cv::Mat row = (cv::Mat_<float>(1, 2) << norm_x, norm_y);
    features.push_back(row);
  }
  cv::flann::Index flann_index(features, cv::flann::KDTreeIndexParams(1), cvflann::FLANN_DIST_EUCLIDEAN);

  // Brute force approach
  for (int i = 0; i < rows; ++i)
  {
    for (int j = 0; j < cols; ++j)
    {
      cv::Mat query = (cv::Mat_<float>(1, 2) << (float)j/(float)cols, (float)i/(float)rows);
      cv::Mat indices, dists;
      flann_index.knnSearch(query, indices, dists, n_coeffs, searchParams);

      int index_0 = indices.at<int>(0);
      int index_1 = indices.at<int>(1);

      float pixel_value = 0;
      for (int c = 0; c < n_coeffs; ++c)
        pixel_value += coeffs[c] * dists.at<float>(0, c);

      heatmap.at<float>(i, j) = pixel_value;
    }
  }
  
  cv::normalize(heatmap, heatmap, 1, 0, cv::NORM_MINMAX);
  return heatmap;
}