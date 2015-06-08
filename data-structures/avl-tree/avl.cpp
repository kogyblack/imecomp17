#include <cstdio>
#include <algorithm>

template<typename T>
class AVLTree {
public:
  AVLTree();
  ~AVLTree();

  void insert(T);
  void remove(T);
  bool search(T) const;
  void print() const;
private:
  struct Node {
    T value;
    int h;
    Node* parent;
    Node* left;
    Node* right;
  };

  void printNode(Node*) const;

  void createNode(T, Node*, Node*&);
  void deleteNode(Node*);

  void updateHeight(Node*);
  int height (Node*) const;
  int balance(Node*) const;

  void rotateRight(Node*&);
  void rotateLeft (Node*&);

  Node* root_;
};

template<typename T>
AVLTree<T>::
AVLTree() : root_(NULL) {}

template<typename T>
AVLTree<T>::
~AVLTree() {
  deleteNode(root_);
}

template<typename T>
void AVLTree<T>::
insert(T) {
}

template<typename T>
void AVLTree<T>::
remove(T) {
}

template<typename T>
bool AVLTree<T>::
search(T value) const {
  Node* node = root_;
  while (node) {
    if (node->value < value)
      node = node->right;
    else if (node->value > value)
      node = node->left;
    else
      return true;
  }

  return false;
}

template<typename T>
void AVLTree<T>::
print() const {
  printNode(root_);
}

template<typename T>
void AVLTree<T>::
printNode(Node* node) const {
  if (!node)
    return;
  printf("%9p %9p value %2d h %2d children %9p %9p\n",
         node, node->parent, node->value, node->h,
         node->left, node->right);
  printNode(node->left);
  printNode(node->right);
}

template<typename T>
void AVLTree<T>::
createNode(T, Node*, Node*&) {
}

template<typename T>
void AVLTree<T>::
deleteNode(Node*) {
}

template<typename T>
void AVLTree<T>::
updateHeight(Node*) {
}

template<typename T>
int AVLTree<T>::
height (Node* node) const {
  if (node)
    return node->h;
  return 0;
}

template<typename T>
int AVLTree<T>::
balance(Node* node) const {
  return height(node->left) - height(node->right);
}

template<typename T>
void AVLTree<T>::
rotateRight(Node*&) {
}

template<typename T>
void AVLTree<T>::
rotateLeft (Node*&) {
}

int main() {
  AVLTree<int> avl;

  int x;
  while (scanf("%d", &x) != EOF) {
    avl.insert(x);
    avl.print();
  }

  return 0;
}
