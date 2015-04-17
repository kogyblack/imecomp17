#include <iostream>

class FixedSizeQueue
{
public:
  FixedSizeQueue(int size) : size_ (size), cur_size_ (0), first_(0)
  {
    queue_ = new int[size_];
  }

  ~FixedSizeQueue()
  {
    delete [] queue_;
  }

  bool push(int elem)
  {
    if (cur_size_ >= size_)
      return false;

    queue_[(first_ + cur_size_) % size_] = elem;
    cur_size_++;

    return true;
  }

  bool pop(int* elem)
  {
    if (cur_size_ == 0)
      return false;

    *elem = queue_[first_];

    first_ = (first_ + 1) % size_;
    cur_size_--;

    return true;
  }

  int size() const
  {
    return cur_size_;
  }

private:
  int* queue_;
  int size_,
      cur_size_;
  int first_;
};

class VariableSizeQueue
{
public:
  VariableSizeQueue() : size_ (1), cur_size_ (0), first_ (0)
  {
    queue_ = new int[size_];
  }

  ~VariableSizeQueue()
  {
    delete [] queue_;
  }

  void push(int elem)
  {
    if (cur_size_ >= size_)
    {
      int* backup = new int[size_];
      int old_size = size_;
      for (int i = first_; i < size_; ++i)
        backup[i - first_] = queue_[i];
      for (int i = 0; i < first_; ++i)
        backup[i + size_ - first_] = queue_[i];

      first_ = 0;
      size_ *= 2;
      delete [] queue_;
      queue_ = new int[size_];
      for (int i = 0; i < old_size; ++i)
        queue_[i] = backup[i];
    }

    queue_[(first_ + cur_size_) % size_] = elem;
    cur_size_++;
  }

  bool pop(int* elem)
  {
    if (cur_size_ == 0)
      return false;

    *elem = queue_[first_];

    first_ = (first_ + 1) % size_;
    cur_size_--;

    return true;
  }

  int size() const
  {
    return cur_size_;
  }

private:
  int* queue_;
  int size_,
      cur_size_;
  int first_;
};

int main()
{
  FixedSizeQueue q(10);
  q.push(1);
  q.push(2);
  q.push(3);

  int a;
  while (q.pop(&a))
    std::cout << a << " ";
  std::cout << std::endl;

  VariableSizeQueue vq;
  vq.push(11);
  vq.push(12);
  vq.push(13);
  while (vq.pop(&a))
    std::cout << a << " ";
  std::cout << std::endl;

  return 0;
}
