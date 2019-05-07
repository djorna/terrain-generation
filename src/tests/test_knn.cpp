#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/flann.hpp>

using namespace std;

int main(int argc, char* argv[]) {

  // Sample data
  std::vector<cv::Point2d> points;
  points.emplace_back(438.6, 268.8);
  points.emplace_back(439.1, 268.6);
  points.emplace_back(438.2, 268.1);
  points.emplace_back(498.3, 285.9);
  points.emplace_back(312.9, 245.9);
  points.emplace_back(313.4, 245.7);
  points.emplace_back(313.1, 245.5);
  points.emplace_back(312.5, 245.4);
  points.emplace_back(297.6, 388.1);
  points.emplace_back(291.7, 309.8);
  points.emplace_back(194.1, 369.8);
  points.emplace_back(439.9, 314.9);
  points.emplace_back(312.8, 246.0);

  // Create an empty Mat for the features that includes dimensional
  // space for an x and y coordinate
  cv::Mat_<float> features(0, 2);

  for (auto && point : points) {

    //Fill matrix
    cv::Mat row = (cv::Mat_<float>(1, 2) << point.x, point.y);
    features.push_back(row);
  }
  std::cout << features << std::endl;

  cv::flann::Index flann_index(features, cv::flann::KDTreeIndexParams(1));


  unsigned int max_neighbours = 10;
  cv::Mat query = (cv::Mat_<float>(1, 2) << 313.0, 245.6);
  cv::Mat indices, dists; //neither assume type nor size here !
  double radius = 2.0;

  flann_index.radiusSearch(query, indices, dists, radius, max_neighbours,
    cv::flann::SearchParams(32));

  cerr << indices.type() << endl << indices << endl;
  cerr << dists.type() << endl << dists << endl;
}