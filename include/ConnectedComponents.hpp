
#include <opencv2/core/core.hpp>

class ConnectedComponents
{

enum Connectivity {C4, C8};

public:
  ConnectedComponents(cv::Mat& img, Connectivity connectivity);
  ~ConnectedComponents();

  

private:

  void oneScan();

  std::vector<cv::Point2i> neighbours(cv::Point2i point);


  cv::Mat img;
};