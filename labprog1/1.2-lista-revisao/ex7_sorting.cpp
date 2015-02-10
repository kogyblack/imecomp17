#include <cstdlib>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>

// We have a lot of sorting algorithms!


// Selection sort
// This is the most basic sorting algorithm (ignore bubblesort. This is the same
// thing and is better).
// It consists in finding the smallest number in the array and putting it on the
// left most position. On further iterations you ignore the numbers already in
// position and find the smallest on the remaining array to position on the left.
// Time complexity: O(N^2)
// Space complexity: O(1)
template<typename T>
void selectionSort(std::vector<T>& vec)
{
  int vecSize = vec.size();
  for (int i = 0; i < vecSize - 1; ++i)
    for (int j = i + 1; j < vecSize; ++j)
      if (vec[j] < vec[i])
        std::swap(vec[j], vec[i]);
}


// Insertion sort
// This is another basic sorting algorithm too.
// It consists in always maintain an ordered subarray on the left and inserting
// the number on the right position inside the sorted subarray.
// It's the algorithm most people use when plays cards.
// Time complexity: O(N^2)
// Space complexity: O(1)
template<typename T>
void insertionSort(std::vector<T>& vec)
{
  int vecSize = vec.size();
  for (int i = 1; i < vecSize; ++i)
    for (int j = i; j > 0 && (vec[j] < vec[j - 1]); --j)
      std::swap(vec[j], vec[j - 1]);
}


// Shell sort
// This one is a little more complicated.
// It consists in using insertion sort in sparsed subarrays.
// This algorithm is very complex to be studied, even only being insertion sort
// on it's bases.
// Time complexity: ?
// Space complexity: O(1)
template<typename T>
void shellSort(std::vector<T>& vec)
{
  int vecSize = vec.size();
  int h = 1;
  while (3 * h < vecSize)
    h = 3 * h + 1;

  while (h > 0)
  {
    for (int i = h; i < vecSize; ++i)
      for (int j = i; j >= h && (vec[j] < vec[j - h]); j -= h)
        std::swap(vec[j], vec[j - h]);

    h /= 3;
  }
}


// Merge sort
// This one is a divide-and-conquer algorithm.
// It consists in dividing the array in two subarrays, sorting the
// subarrays and merging the subarrays together.
// It works very well for large arrays.
// Time complexity: O(N logN)
// Space complexity: O(N)
template<typename T>
void merge(std::vector<T>& vec, int low, int mid, int high)
{
  // Create auxiliar array that copies from low to high
  std::vector<T> aux (vec.begin() + low, vec.begin() + high + 1);

  // We have to work from 0 to (high - low), so we reduce high and mid
  // from low.
  high -= low;
  mid -= low;

  int i = 0,
      j = mid + 1;

  for (int k = low; k <= high; ++k)
  {
    if (i > mid)              vec[k] = aux[j++];
    else if (j > high)        vec[k] = aux[i++];
    else if (aux[j] < aux[i]) vec[k] = aux[j++];
    else                      vec[k] = aux[i++];
  }
}

template<typename T>
void mergeSortInternal(std::vector<T>& vec, int low, int high)
{
  if (high <= low)
    return;

  int mid = low + (high - low) / 2;
  mergeSortInternal(vec, low, mid);
  mergeSortInternal(vec, mid + 1, high);
  merge(vec, low, mid, high);
}

template<typename T>
void mergeSort(std::vector<T>& vec)
{
  mergeSortInternal(vec, 0, vec.size() - 1);
}


// Quick sort
// This is the fastest sorting algorithm for large arrays.
// Even though the worst case for quick sort is O(N^2), it's extremely
// hard to fall on the worst case in real applications. Also it can
// be probabilistic avoided shuffling the array before sorting.
// This one consists in taking a random number from the array and putting
// all numbers lesser than this sentinel number on it's left (thus all
// numbers on the right will be equal or greater than it) and doing this
// recursively.
template<typename T>
void quickSortInternal(std::vector<T>& vec, int low, int high)
{
  if (low >= high)
    return;

  int pivot = low;
  T pivotValue = vec[pivot];

  int pos = low;

  for (int i = low + 1; i <= high; ++i)
    if (vec[i] <= pivotValue)
      std::swap(vec[i], vec[++pos]);

  std::swap(vec[pivot], vec[pos]);

  quickSortInternal(vec, low, pos - 1);
  quickSortInternal(vec, pos + 1, high);
}

template<typename T>
void quickSort(std::vector<T>& vec)
{
  quickSortInternal(vec, 0, vec.size() - 1);
}


int main()
{
  srand(time(NULL));

  std::cout << "Entre com n: ";

  int n;
  std::cin >> n;

  // Create n random numbers between 0 and n - 1
  std::vector<int> vec;
  vec.reserve(n);
  for (int i = 0; i < n; ++i)
    vec.push_back(rand() % n);

  std::vector<int> vecCopy = vec;

  clock_t init = clock();
  // STL already has a sort method. Testing you can see that it is slower than
  // both quick and merge sorts
  std::sort(vec.begin(), vec.end());
  std::cout << "Sort da STL: " << static_cast<float>(clock() - init) / CLOCKS_PER_SEC << std::endl;

  vec = vecCopy;
  init = clock();
  quickSort(vec);
  std::cout << "Quick sort: " << static_cast<float>(clock() - init) / CLOCKS_PER_SEC << std::endl;

  vec = vecCopy;
  init = clock();
  mergeSort(vec);
  std::cout << "Merge sort: " << static_cast<float>(clock() - init) / CLOCKS_PER_SEC << std::endl;

  vec = vecCopy;
  init = clock();
  shellSort(vec);
  std::cout << "Shell sort: " << static_cast<float>(clock() - init) / CLOCKS_PER_SEC << std::endl;

  vec = vecCopy;
  init = clock();
  insertionSort(vec);
  std::cout << "Insertion sort: " << static_cast<float>(clock() - init) / CLOCKS_PER_SEC << std::endl;

  vec = vecCopy;
  init = clock();
  selectionSort(vec);
  std::cout << "Selection sort: " << static_cast<float>(clock() - init) / CLOCKS_PER_SEC << std::endl;


  for (unsigned int i = 0; i < vec.size(); ++i)
    std::cout << vec[i] << " ";
  std::cout << std::endl;

  return 0;
}

