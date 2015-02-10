#include <cstdlib>
#include <iostream>

void print(int n, int last)
{
  if (n < last)
  {
    std::cout << n << " ";
    print(n + 2, last);
  }
}

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    std::cout << "Usage: " << argv[0] << " n" << std::endl;
    return 1;
  }

  print(2, atoi(argv[1]));
  std::cout << std::endl;

  return 0;
}
