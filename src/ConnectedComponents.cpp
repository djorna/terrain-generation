#include "ConnectedComponents.hpp"

void ConnectedComponents::oneScan()
{
  // Store a single point from each group, and its color
  // Output: list of (point, colour) pairs
  // Eventual output is a list of binary masks
  cv::Mat labels = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
  int current_label = 1;
  std::queue<cv::Point2i> point_queue;

  cv::Point2i curr_point = cv::Point2i(0, 0);
  while((curr_point = nextPixel(point)) != nullptr) {
      if(labels.at<uchar>(i, j) > 0 || img.at<uchar>(i, j) == 0)
      {
        curr_point = nextPixel(curr_point);
      }
      else
      {
        while(!point_queue.empty())
        {
          auto pt = point_queue.front();
          // Get neighours of point
          auto nbs = neightbours(pt);

          for (nb : nbs)
          {
            if(labels.at<uchar>(nb.y, nb.x) > 0 || img.at<uchar>(nb.y, nb.x) == 0)
            {
              point_queue.push(nb);
              labels.at<uchar>(nb.y, nb.x) = current_label;
            }
          }
        }


      }
  }





}

cv::Point2i ConnectedComponents::nextPixel(cv::Point2i point)
{
  // Iterate left -> right, then up -> down
  int next_x = (point.x + 1) % img.cols;
  int next_y = next_x == 0 ? point.y + 1 : point.y; //
  if(next_y > img.rows) return nullptr;
  cv::Point2i nextPoint = cv::Point2i((point.x + 1) % img.cols, );
  return nextPoint;
}