#include "Random.hpp"

#include <iostream>
#include <numeric>



using namespace std;

int main()
{
  int min = 0, max = 10, seed = 123;
  Random rand(min, max, seed);

  for (int i = 0; i < 10; ++i)
    cout << rand.randi() << ' ';

  vector<int> indices { 0,1,2,3 };
  std::iota(std::begin(indices), std::end(indices), 0); // Get numbers from 0 to n
  rand.seed(5);
  // rand.shuffle(indices); // Uniformly shuffle indices
  shuffle(indices); // Uniformly shuffle indices

  cout << '\n';
  for (auto index : indices)
    cout << index << ' ';

  Random rand2(seed);
  rand2.set_norm(0.5, 0.2);
  cout << '\n';
  for (int i = 0; i < 10; ++i)
    cout << rand2.randf();
}