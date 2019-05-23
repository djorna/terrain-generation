#if _MSC_VER // this is defined when compiling with Visual Studio
#define EXPORT_API __declspec(dllexport) // Visual Studio needs annotating exported functions with this
#else
#define EXPORT_API // XCode does not need annotating exported functions, so define is empty
#endif

#include "DiamondSquare.hpp"
#include "Voronoi.hpp"
#include "ThermalErosion.hpp"
#include "FastErosion.hpp"
#include "Perturb.hpp"
#include "Evaluator.hpp"

#include <opencv2/opencv.hpp>

#include <fstream>
#include <thread>

using namespace terrain;

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

  void EXPORT_API DiamondSquarePlugin(float *data, int n, float persistence, int seed)
  {
    DiamondSquare diamondSquare;
    cv::Mat heightmap = diamondSquare.generate(n, persistence, seed);
    extractData(heightmap, data);
  }

  void EXPORT_API VoronoiPlugin(float* data, float* points_x, float* points_y, int n, int n_points, int n_coeffs, float* coeffs,
    int point_seed, float keep, int mask_seed, float mean, float stdev, int shift_seed)
  {
    int rows = n, cols = n;
    std::vector<float> vcoeffs(coeffs, coeffs + n_coeffs);
    Voronoi vrn = Voronoi(rows, cols, vcoeffs, n_points);
    vrn.binaryMask(keep, mask_seed);
    // vrn.shiftHeightMask(mean, stdev, shift_seed);
    cv::Mat vrn_img = vrn.generate();
    vrn.
    extractData(vrn_img, data);
  }

  /*
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
  */


  void EXPORT_API CombinedPlugin(float* data1, float* data2, float* result, int rows, int cols, float alpha)
  {
    // Convert data to cv::Mat
    cv::Mat input_mat1(rows, cols, CV_32FC1, data1);
    cv::Mat input_mat2(rows, cols, CV_32FC1, data2);
    
    // Combine
    cv::Mat combined;
    cv::addWeighted(input_mat1, alpha, input_mat2, 1 - alpha, 0, combined);
    cv::normalize(combined, combined, 1, 0, cv::NORM_MINMAX);

    // Save to first matrix
    extractData(combined, result);
  }

  void EXPORT_API BaseTerrainPlugin(float* data, int n, int n_points, int n_coeffs, float* coeffs, float persistence,
    float keep, float mean, float stdev, int mask_seed, int shift_seed)
  {
    int rows = n, cols = n;
    // DiamondSquare
    DiamondSquare diamondSquare;
    cv::Mat heightmap_ds = diamondSquare.generate(n, persistence);

    // Voronoi
    std::vector<float> vcoeffs(coeffs, coeffs + n_coeffs);
    Voronoi vrn = Voronoi(rows, cols, vcoeffs, n_points);
    vrn.binaryMask(keep, mask_seed);
    vrn.shiftHeightMask(mean, stdev, shift_seed);
    cv::Mat heightmap_vrn = vrn.generate();

    // Combine
    cv::Mat combined;
    cv::addWeighted(heightmap_ds, 0.67, heightmap_vrn, 0.33, 0, combined);
    cv::normalize(combined, combined, 1, 0, cv::NORM_MINMAX);

    // Perturbation
    Perturb perturb_filter;
    combined = perturb_filter.apply(combined);

    extractData(combined, data);
  }

  void EXPORT_API PerturbPlugin(float* data, int rows, int cols, float mag)
  {
    cv::Mat input_mat(rows, cols, CV_32FC1, data);

    cv::Mat perturbed = Perturb::apply(input_mat, mag);

    extractData(perturbed, data);
  }

  void EXPORT_API MaskPlugin(float* data, int n, int n_points, int n_coeffs, float* coeffs, float persistence,
    const char* mask_file, int mask_file_len)
  {
    int rows = n, cols = n;

    // Convert data to cv::Mat
    cv::Mat input_img(rows, cols, CV_32FC1, data);

    // Apply binary mask
    std::string mask_file_str(mask_file);
    cv::Mat mask = cv::imread(mask_file_str, cv::IMREAD_GRAYSCALE);
    cv::Mat masked_img;
    cv::bitwise_and(input_img, input_img, masked_img, mask);

    cv::imshow("mask", mask);
    cv::imshow("masked_img", masked_img);
    cv::waitKey(0);

    extractData(masked_img, data);
  }

  void EXPORT_API SlopePlugin(float* data, float* slope, int rows, int cols)
  {
    cv::Mat input_img(rows, cols, CV_32FC1, data);
    cv::Mat slope_img = Evaluator::getSlope(input_img);
    extractData(slope_img, slope);
  }

  void EXPORT_API ThermalErosionPlugin(float* data, int rows, int cols)
  {
    cv::Mat eroded(rows, cols, CV_32FC1, data);
    ThermalErosion thermal(MOORE);
    thermal.apply(eroded);
    cv::normalize(eroded, eroded, 1, 0, cv::NORM_MINMAX);
    extractData(eroded, data);
  }

  void EXPORT_API FastErosionPlugin(float* data, int rows, int cols)
  {
    cv::Mat eroded(rows, cols, CV_32FC1, data);
    FastErosion erosion(VON_NEUMANN2);
    erosion.apply(eroded);
    cv::normalize(eroded, eroded, 1, 0, cv::NORM_MINMAX);
    extractData(eroded, data);
  }

} // extern "C"