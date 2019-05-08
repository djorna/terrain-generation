#pragma once

#include <random>
#include <stdexcept>

// Not thread safe due to engines :/
// template<class T = int>
// Factory?
class Random
{
  // TODO: Error handling

enum DistributionType {UNIFORM_INT, UNIFORM_REAL, NORMAL};

public:
  Random();

  Random(int seed);
  
  Random(int min, int max, int seed);

  Random(float min, float max, int seed);

  ~Random();

  Random(const Random &random);

  Random& operator=(const Random &random);

  void seed(int seed);

  template<class T>
  T next()
  {
  switch (dis_type)
  {
  case UNIFORM_INT:
    return (*dis_int)(pseudo_rand_engine);
  case UNIFORM_REAL:
    return (*dis_float)(pseudo_rand_engine);
  case NORMAL:
    return (*dis_norm)(pseudo_rand_engine);
  default:
    throw std::invalid_argument("Invalid distribution type");
  }
  }

  template<class T>
  void next(T& val) { val = next<T>(); }

  float randf();

  int randi();

  void set_dis(int min, int max);

  void set_dis(float min, float max);

  void set_norm(const float mean, const float stdev);

  static std::random_device random_device;
  static std::mt19937 pseudo_rand_engine;
private:
  std::unique_ptr<std::uniform_real_distribution<float>> dis_float;
  std::unique_ptr<std::uniform_int_distribution<int>> dis_int;
  std::unique_ptr<std::normal_distribution<float>> dis_norm;
  DistributionType dis_type;
};

template<class Tp>
inline void shuffle(std::vector<Tp>& vec)
{
  std::shuffle(vec.begin(), vec.end(), Random::pseudo_rand_engine);
}
