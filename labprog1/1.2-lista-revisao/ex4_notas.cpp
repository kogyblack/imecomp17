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

  // Sum of grades and number of exams
  int sumMale = 0;
  int cntMale = 0;
  int sumFemale = 0;
  int cntFemale = 0;

  // File
  std::ifstream file;
  file.open(argv[1]);
  if (!file.is_open())
  {
    std::cout << "Couldn\'t open file: " << argv[1] << std::endl;
    return 2;
  }

  // Temp vars to be read
  char sex;
  int grade;

  while (file >> sex >> grade)
  {
    if (sex == 'M')
    {
      sumMale += grade;
      cntMale++;
    }
    else if (sex == 'F')
    {
      sumFemale += grade;
      cntFemale++;
    }
    else
    {
      std::cout << "Unknown sex: " << sex
                << ". Ignoring grade!" << std::endl;
    }
  }

  // Should verify if cntMale and cntFemale were different than zero!!!
  float averageMale = static_cast<float>(sumMale) / cntMale;
  float averageFemale = static_cast<float>(sumFemale) / cntFemale;

  if (averageMale > averageFemale)
    std::cout << "M " << averageMale << std::endl;
  else if (averageMale < averageFemale)
    std::cout << "F " << averageFemale << std::endl;
  else
    std::cout << "Same average: " << averageMale << std::endl;

  return 0;
}
