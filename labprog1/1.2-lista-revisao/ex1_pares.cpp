#include <cstdlib>
#include <iostream>

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    std::cout << "Usage: " << argv[0] << " n" << std::endl;
    return 1;
  }

  int n = atoi(argv[1]);

  for (int i = 2; i < n; i += 2)
    std::cout << i << " ";
  std::cout << std::endl;

  return 0;
}
