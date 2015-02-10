#include <cstdlib>
#include <iostream>
#include <vector>

int fibonacci(int n)
{
  // This is a cache like the recursive fibonacci
  // but instead of using a map (dict) it's better to use
  // a vector since we will create the fibonacci numbers in
  // order (and accessing on vector is constant time, versus the
  // logN time from map).
  static std::vector<int> cache;
  if (cache.size() < 2)
  {
    cache.push_back(1);
    cache.push_back(1);
  }

  // If we already calculated fibo of n, return it
  if (n <= cache.size())
    return cache[n - 1];

  // If we have not calculate it yet, let's use a for to calculate
  // Reserve the memory (allocate memory) for the whole fibonacci numbers
  // until n. Performance-wise, every time you know how many variables you
  // will hold inside the vector it's better to reserve the memory.
  cache.reserve(n);

  for (int i = cache.size(); i < n; ++i)
    cache[i] = cache[i - 1] + cache[i - 2];

  // We could not repeat this like (this is the same line as the one inside
  // the if statement).
  return cache[n - 1];
}

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    std::cout << "Usage: " << argv[0] << " n" << std::endl;
    return 1;
  }

  int n = atoi(argv[1]);

  std::cout << fibonacci(n) << std::endl;

  return 0;
}
