#if _MSC_VER // this is defined when compiling with Visual Studio
#define EXPORT_API __declspec(dllexport) // Visual Studio needs annotating exported functions with this
#else
#define EXPORT_API // XCode does not need annotating exported functions, so define is empty
#endif

#include "DiamondSquare.hpp"
#include "Voronoi.hpp"
#include "ThermalErosion.hpp"

#include <opencv2/opencv.hpp>

#include <fstream>

/*
 * Convert char* to string
 */
std::string convert(char* data, int len)
{
   return std::string(data, std::find(data, data + len, '\0'));
}

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

  void EXPORT_API CombinedPlugin(float* data, int n, int n_points, int n_coeffs, float* coeffs, float persistence)
  {
    int rows = n, cols = n;
    std::vector<float> vcoeffs(coeffs, coeffs + n_coeffs);
    Voronoi vrn = Voronoi(rows, cols, vcoeffs, n_points);
    cv::Mat heightmap_vrn = vrn.generate();

    DiamondSquare ds;
    cv::Mat heightmap_ds = ds.generate(n, persistence);

    cv::Mat combined;
    cv::addWeighted(heightmap_ds, 0.67, heightmap_vrn, 0.33, 0, combined);
    cv::normalize(combined, combined, 1, 0, cv::NORM_MINMAX);

    extractData(combined, data);
  }

  void EXPORT_API MaskPlugin(float* data, int n, int n_points, int n_coeffs, float* coeffs, float persistence,
    const char* mask_file, int mask_file_len)
  {
    // BUG: Silently failed if file does not exist
    int rows = n, cols = n;
    std::vector<float> vcoeffs(coeffs, coeffs + n_coeffs);
    Voronoi vrn = Voronoi(rows, cols, vcoeffs, n_points);
    cv::Mat heightmap_vrn = vrn.generate();

    DiamondSquare ds;
    cv::Mat heightmap_ds = ds.generate(n, persistence);

    cv::Mat combined;
    cv::addWeighted(heightmap_ds, 0.67, heightmap_vrn, 0.33, 0, combined);
    cv::normalize(combined, combined, 1, 0, cv::NORM_MINMAX);

    cv::imshow("combined", combined);
    cv::waitKey(0);

    // Apply binary mask
    //std::string mask_file_str = convert(mask_file, mask_file_len);
    std::string mask_file_str(mask_file);
    cv::Mat mask = cv::imread(mask_file_str, cv::IMREAD_GRAYSCALE);
    cv::Mat masked_img;
    cv::bitwise_and(combined, combined, masked_img, mask);
    // mask.convertTo(masked_img, CV_32F);

    cv::imshow("mask", mask);
    cv::imshow("masked_img", masked_img);
    cv::waitKey(0);

    extractData(masked_img, data);
  }

  void EXPORT_API ThermalErosionPlugin(float* data, int rows, int cols)
  {
    cv::Mat eroded(rows, cols, CV_32FC1, data);
    ThermalErosion thermal(MOORE);
    thermal.apply(eroded);
    cv::normalize(eroded, eroded, 1, 0, cv::NORM_MINMAX);
    extractData(eroded, data);
  }

}// extern "C"