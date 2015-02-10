#include <cstdlib>
#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    std::cout << "Usage: " << argv[0] << " filename" << std::endl;
    return 1;
  }

  // Total sum and product
  int sum = 0;
  int product = 1;

  // File
  std::ifstream file;
  file.open(argv[1]);
  if (!file.is_open())
  {
    std::cout << "Couldn\'t open file: " << argv[1] << std::endl;
    return 2;
  }

  // Buffer to read file lines
  char buffer[256];

  // Get first line
  file.getline(buffer, 256);
  while (file.good())
  {
    // Convert string to number
    int num = atoi(buffer);

    if (num % 2 == 0) // If even, multiply
      product *= num;
    else              // If odd, sum
      sum += num;

    // Get next line
    file.getline(buffer, 256);
  }

  // Print results
  std::cout << product << std::endl
            << sum << std::endl;

  return 0;
}
