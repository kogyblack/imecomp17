#include <iostream>

class List
{
public:
  struct Node
  {
    int value;
    Node* next;
  };

  List() : head (NULL) // Obrigatory!!
  {}

  ~List()
  {
    cleanup(head);
  }

  void insert(int elem)
  {
    Node* cur = head;
    if (head)
    {
      while (cur->next)
      {
        cur = cur->next;
      }
      cur->next = new Node;
      cur->next->value = elem;
      cur->next->next = NULL; // head->next->next = nullptr; //C++11
    }
    else
    {
      head = new Node;
      head->value = elem;
      head->next = NULL; // head->next = nullptr; //C++11
    }
  }

  bool remove(int elem)
  {
    Node* prev = NULL; //Node* prev = nullptr; //C++11
    Node* cur = head;

    while (cur)
    {
      if (cur->value == elem)
      {
        if (prev)
          prev->next = cur->next;
        else // cur == head
          head = head->next;
        delete cur;
        return true;
      }

      prev = cur;
      cur = cur->next;
    }

    return false;
  }

  int size() const
  {
    int s = 0;
    Node* cur = head;
    while (cur)
    {
      s++;
      cur = cur->next;
    }

    return s;
  }

  int get(int index) // int operator[](int index)
  {
    Node* cur = head;
    for (int i = 0; i < index && cur; ++i)
      cur = cur->next;

    if (cur)
      return cur->value;
    return -1; // Return error? Think a better way...
  }

private:
  void cleanup(Node* node)
  {
    if (node)
    {
      cleanup(node->next);
      delete node;
    }
  }

  Node* head;
};

int main()
{
  List l;
  l.insert(1);
  l.insert(2);
  l.insert(3);
  l.insert(4);

  for (int i = 0; i < l.size(); ++i) // Bad code!! size() is calculated every loop
    std::cout << l.get(i) << " ";
  std::cout << std::endl;

  l.remove(1);

  for (int i = 0; i < l.size(); ++i) // Bad code!! size() is calculated every loop
    std::cout << l.get(i) << " ";
  std::cout << std::endl;

  l.remove(4);
  for (int i = 0; i < l.size(); ++i) // Bad code!! size() is calculated every loop
    std::cout << l.get(i) << " ";
  std::cout << std::endl;

  return 0;
}
