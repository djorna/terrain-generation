#include "Random.hpp"

std::random_device Random::random_device;
std::mt19937 Random::pseudo_rand_engine;

Random::Random() { seed(-1); }

Random::Random(int seed_val) { seed(seed_val); }

Random::Random(int min, int max, int seed_val)
{
  set_dis(min, max);
  seed(seed_val);
}

Random::Random(float min, float max, int seed_val)
{
  set_dis(min, max);
  seed(seed_val);
}

Random::~Random() {}

Random::Random(const Random &random)
{
  // dis_float = random.dis_float.get();
  dis_float.reset(random.dis_float.get());
  dis_int.reset(random.dis_int.get());
  dis_norm.reset(random.dis_norm.get());
  dis_type = random.dis_type;
}


Random& Random::operator=(const Random &random)
{
  Random random2;
  random2.dis_float.reset(random.dis_float.get());
  random2.dis_int.reset(random.dis_int.get());
  random2.dis_norm.reset(random.dis_norm.get());
  random2.dis_type = random.dis_type;
  return random2;
}

void Random::set_dis(int min, int max)
{
  dis_int = std::make_unique<std::uniform_int_distribution<int>>(std::uniform_int_distribution<int>(min, max));
  dis_type = UNIFORM_INT;
}

void Random::set_dis(float min, float max)
{
  dis_float = std::make_unique<std::uniform_real_distribution<float>>(std::uniform_real_distribution<float>(min, max));
  dis_type = UNIFORM_REAL;
}

void Random::set_norm(const float mean, const float stdev)
{
  dis_norm = std::make_unique<std::normal_distribution<float>>(mean, stdev);
  dis_type = NORMAL;
}

float Random::randf()
{
  return next<float>();
}

int Random::randi()
{
  return next<int>();
}

void Random::seed(int seed) 
{
  seed > 0 ? pseudo_rand_engine.seed(seed) : pseudo_rand_engine.seed(random_device());
}

