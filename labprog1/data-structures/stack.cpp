#include <iostream>

class Stack
{
public:
  Stack() : size_ (1), cur_size_ (0)
  {
    stack_ = new int[size_];
  }

  ~Stack()
  {
    delete [] stack_;
  }

  void push(int elem)
  {
    if (cur_size_ >= size_)
    {
      int* backup = new int[size_];
      int old_size = size_;
      for (int i = 0; i < size_; ++i)
        backup[i] = stack_[i];

      size_ <<= 1; // size_ = size_ * 2;
      delete [] stack_;
      stack_ = new int[size_];
      for (int i = 0; i < old_size; ++i)
        stack_[i] = backup[i];
    }

    stack_[cur_size_++] = elem;
  }

  bool pop(int* elem)
  {
    if (cur_size_ == 0)
      return false;

    *elem = stack_[--cur_size_];
    return true;
  }

  int size() const
  {
    return cur_size_;
  }
private:
  int* stack_;
  int size_,
      cur_size_;
};

int main()
{
  Stack s;
  s.push(1);
  s.push(2);
  s.push(3);

  int a;
  while (s.pop(&a))
    std::cout << a << " ";
  std::cout << std::endl;

  return 0;
}
