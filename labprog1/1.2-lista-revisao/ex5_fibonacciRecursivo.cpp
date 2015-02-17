#include <cstdlib>
#include <iostream>
#include <map>

// This is the naive approach
// DON'T DO LIKE THIS!
// This algorithm has O(N!) complexity.
// It's extremely slow, and you can only find very small
// numbers using this algorithm
int fibonacciDontDoLikeThis(int n)
{
  if (n <= 2)
    return 1;
  return fibonacciDontDoLikeThis(n - 1) + fibonacciDontDoLikeThis(n - 2);
}

// Use this one for recursive fibonacci!
// This algorithm uses a technique called memoization.
// It consists in storing all previously calculated results and
// getting them instead of recalculating.
// This algorithm has O(N logN) complexity, because map has O(logN)
// complexity
int fibonacci(int n)
{
  static std::map<int, int> cache;
  // Not recomended to test this every function call!
  // Could be better cache globally or wrap this function
  if (cache.size() < 2)
  {
    cache[1] = 1;
    cache[2] = 1;
  }

  // Search if n is already in cache
  if (cache.find(n) != cache.end())
    return cache[n];

  // If not in cache, calculate fibonacci of n
  cache[n] = fibonacci(n - 1) + fibonacci(n - 2);

  return cache[n];
}

// Using arguments instead of basic input because
// it's very annoying to write the input instead of
// repeating the command line arguments
int main(int argc, char** argv)
{
  if (argc < 2)
  {
    std::cout << "Usage: " << argv[0] << " n" << std::endl;
    return 1;
  }

  int n = atoi(argv[1]);

  std::cout << fibonacci(n) << std::endl;

  // If you want to compare
  // This program only works for n <= 46 (to use more, use unsigned int,
  // long or unsigned long).
  // TEST n = 46

  //std::cout << fibonacciDontDoLikeThis(n) << std::endl;

  return 0;
}
