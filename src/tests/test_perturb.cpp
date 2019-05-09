#include "DiamondSquare.hpp"
#include "Voronoi.hpp"
#include "SimplexNoise.hpp"

#include <opencv2/opencv.hpp>
#include <cmath>
#include <thread>

void remove_nans(cv::Mat& img)
{
  cv::Mat mask = cv::Mat(img != img);
  img.copyTo(img, mask);
}

void clamp(float& val, const float min_range, const float max_range)
{
  if (val < min_range) val = min_range;
  if (val > max_range) val = max_range;
}
/**
 * \brief Find Mat value at point via bilinear interpolation.
 * https://en.wikipedia.org/wiki/Bilinear_interpolation
 */
float interp_value(const cv::Mat& img, float x, float y)
{
  clamp(x, 0, static_cast<float>(img.cols - 2));
  clamp(y, 0, static_cast<float>(img.rows - 2));
  float xi[2] = { floor(x), floor(x + 1) };
  float yi[2] = { floor(y), floor(y + 1) };
  auto f = [&](int cx, int cy) {
    return img.at<float>(static_cast<int>(yi[cy]), static_cast<int>(xi[cx]));
  };
  float fxy0 = (xi[1] - x) * f(0, 0) + (x - xi[0]) * f(1, 0);
  float fxy1 = (xi[1] - x) * f(0, 1) + (x - xi[0]) * f(1, 1);
  float value = (yi[1] - y) * fxy1 + (y - yi[0]) * fxy1;
  return value;
}

float get_value(const cv::Mat& img, float x, float y)
{
  clamp(x, 0, static_cast<float>(img.cols - 2));
  clamp(y, 0, static_cast<float>(img.rows - 2));
  return img.at<float>(static_cast<int>(y), static_cast<int>(x));
}

cv::Mat perturb(cv::Mat& img, float mag=0.25)
{
  cv::Mat new_img(img.rows, img.cols, CV_32FC1);
  img.copyTo(new_img);
  float offset_i = 1.5;
  float offset_j = 6.7;
  float offset_v = 3.4;
  for (int i = 0; i < img.rows; ++i)
  {
    for (int j = 0; j < img.cols; ++j)
    {
      float val = img.at<float>(i, j);
      float s = static_cast<float>(i) / (float)img.rows;
      float t = static_cast<float>(j) / (float)img.cols;

      float ss = s + mag * SimplexNoise::noise(s, t, val);
      float tt = t + mag * SimplexNoise::noise(s + offset_i, t + offset_j, val+ offset_v);

      float y2 = std::min(static_cast<float>(img.rows), std::max(0.0f, ss * (float)img.rows));
      float x2 = std::min(static_cast<float>(img.cols), std::max(0.0f, tt * (float)img.cols));

      // new_img.at<float>(i, j) = interp_value(img, x2, y2);
      new_img.at<float>(i, j) = get_value(img, x2, y2);
    }
  }
  return new_img;
}

int main(int argc, char** argv)
{
  std::string example_filename(argv[1]);
  float mag = std::atof(argv[2]);

  cv::Mat heightmap(513, 513, CV_32FC1);
  for (int i = 0; i < heightmap.rows; ++i)
  {
    for (int j = 0; j < heightmap.cols; ++j)
    {
      heightmap.at<float>(i, j) = SimplexNoise::noise(i, j);
    }
  }

  cv::Mat example_img = cv::imread(example_filename, cv::IMREAD_GRAYSCALE);
  cv::Mat example_img_f;
  example_img.convertTo(example_img_f, CV_32FC1);
  cv::normalize(example_img_f, example_img_f, 1, 0, cv::NORM_MINMAX);
  remove_nans(example_img_f);
  cv::Mat new_img = perturb(example_img_f, mag);
  //std::cout << SimplexNoise::noise()

  // Get Combined
  int n = pow(2, 9) + 1; // 513 x 513 image
  int rows, cols; rows = cols = n;
  float persistence = 0.5; // Higher = more jagged terrain

  cv::Mat heightmap_ds;
  std::thread ds_thread([&] {
    DiamondSquare diamondSquare;
    heightmap_ds = diamondSquare.generate(n, persistence);
  });
  // auto heightmap_ds = diamondSquare.generate(n, persistence);

  int n_points = 50;

  cv::Mat heightmap_vrn;
  std::vector<float> coeffs = { -1, 1 };
  std::thread vrn_thread([&] {
    Voronoi vrn(rows, cols, coeffs, n_points);
    heightmap_vrn = vrn.generate();
  });

  // cv::Mat combined = combine({ heightmap_ds, heightmap_vrn }, { 1 / 3, 2 / 3 });
  cv::Mat combined;
  ds_thread.join();
  vrn_thread.join();
  cv::addWeighted(heightmap_ds, 0.67, heightmap_vrn, 0.33, 0, combined);

  cv::normalize(combined, combined, 1, 0, cv::NORM_MINMAX);
  /*
  (float frequency = 1.0f,
    float amplitude = 1.0f,
    float lacunarity = 2.0f,
    float persistence = 0.5f) :
  int count = 100;
  cv::createTrackbar("frequency", "trackbars", int* value, count);
    cv::createTrackbar("amplitude", "trackbars", int* value, count);
    cv::createTrackbar("lacunarity", "trackbars", int* value, count);
    cv::createTrackbar("persistence", "trackbars", int* value, count);
    */
  cv::Mat terrain_pt = perturb(combined, mag);

  cv::imshow("Simplex noise", heightmap);
  cv::imshow("Example Image", example_img_f);
  cv::imshow("Perturbed", new_img);
  cv::imshow("Terrain", combined);
  cv::imshow("Perturbed terrain", terrain_pt);

  cv::waitKey(0);
  
}