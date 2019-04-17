#if _MSC_VER // this is defined when compiling with Visual Studio
#define EXPORT_API __declspec(dllexport) // Visual Studio needs annotating exported functions with this
#else
#define EXPORT_API // XCode does not need annotating exported functions, so define is empty
#endif

#include "DiamondSquare.hpp"
#include "Voronoi.hpp"

#include <opencv2/opencv.hpp>


template<typename T>
void extractData(const cv::Mat& mat, T* data)
{
  std::vector<T> array;
  if (mat.isContinuous())
  {
    array.assign((T*)mat.datastart, (T*)mat.dataend);
  }
  else
  {
    for (int i = 0; i < mat.rows; ++i)
      array.insert(array.end(), mat.ptr<T>(i), mat.ptr<T>(i) + mat.cols);
  }
  std::memcpy(data, array.data(), array.size() * sizeof(T));
}

extern "C"
{

void EXPORT_API DiamondSquarePlugin(uchar *data, int n, float persistence)
{
  DiamondSquare diamondSquare;
  cv::Mat heightmap = diamondSquare.generate(n, persistence);
  cv::Mat heightmap2;
  heightmap *= 255;
  heightmap.convertTo(heightmap2, CV_8UC1);
  extractData(heightmap2, data);
}

void EXPORT_API DiamondSquarePluginFloat(float *data, int n, float persistence)
{
  DiamondSquare diamondSquare;
  cv::Mat heightmap = diamondSquare.generate(n, persistence);
  extractData(heightmap, data);
}

void EXPORT_API VoronoiPlugin(float* data, int n, int n_points, int n_coeffs, float* coeffs)
{
  int rows = n, cols = n;
  std::vector<float> vcoeffs(coeffs, coeffs + n_coeffs);
  Voronoi vrn = Voronoi(rows, cols, vcoeffs, n_points);
  vrn.generatePoints(n_points, rows, cols);
  cv::Mat vrn_img = vrn.generate();
  extractData(vrn_img, data);
}

float EXPORT_API AddTwoFloats(float a, float b) {
	return a + b;
}

} // extern "C"