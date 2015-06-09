#include <cstdio>

struct Node {
  int key;
  int height;
  Node* left;
  Node* right;
  Node(int k) : key(k), height(1), left(0), right(0) {}
};

int height(Node* node) {
  return node ? node->height : 0;
}

int balance(Node* node) {
  return height(node->right) - height(node->left);
}

void updateHeight(Node* node) {
  int hl = height(node->left);
  int hr = height(node->right);
  node->height = 1 + (hl > hr ? hl : hr);
}

Node* rotateRight(Node* node) {
  Node* q    = node->left;
  node->left = q->right;
  q->right   = node;

  updateHeight(node);
  updateHeight(q);
  return q;
}

Node* rotateLeft(Node* node) {
  Node* q     = node->right;
  node->right = q->left;
  q->left     = node;

  updateHeight(node);
  updateHeight(q);
  return q;
}

Node* rebalance(Node* node) {
  updateHeight(node);
  if (balance(node) == 2) {
    if (balance(node->right) == -1)
      node->right = rotateRight(node->right);
    node = rotateLeft(node);
  } else if (balance(node) == -2) {
    if (balance(node->left) == 1)
      node->left = rotateLeft(node->left);
    node = rotateRight(node);
  }
  return node;
}

Node* insert(Node* node, int key) {
  if (!node)
    return new Node(key);
  if (key < node->key)
    node->left = insert(node->left, key);
  else
    node->right = insert(node->right, key);
  return rebalance(node);
}

Node* findMinimum(Node* node) {
  return node->left ? findMinimum(node->left) : node;
}

Node* removeMin(Node* node) {
  if (!node->left)
    return node->right;
  node->left = removeMin(node->left);
  return rebalance(node);
}

Node* remove(Node* node, int key) {
  if (!node)
    return 0;

  if (key < node->key)
    node->left = remove(node->left, key);
  else if (key > node->key)
    node->right = remove(node->right, key);
  else {
    Node* l = node->left;
    Node* r = node->right;
    delete node;
    if (!r)
      return l;
    Node* m  = findMinimum(r);
    m->right = r;
    m->left  = l;
    return rebalance(m);
  }

  return rebalance(node);
}

void deleteNode(Node* node) {
  if (!node)
    return;
  deleteNode(node->left);
  deleteNode(node->right);
  delete node;
}

bool search(Node* node, int key) {
  if (!node)
    return false;
  if (key == node->key)
    return true;
  if (key < node->key)
    return search(node->left, key);
  return search(node->right, key);
}

void print(Node* node) {
  if (!node)
    return;
  printf("Key: %2d\n",
         node->key);
  print(node->left);
  print(node->right);
}

class AVL {
public:
  AVL() : root_(0) {}
  ~AVL() { ::deleteNode(root_); }

  void insert(int key) {
    root_ = ::insert(root_, key);
  }

  void remove(int key) {
    root_ = ::remove(root_, key);
  }

  bool search(int key) {
    return ::search(root_, key);
  }

  void print() const {
    ::print(root_);
  }

private:
  Node* root_;
};

int main() {
  AVL avl;

  int x;
  for (;;) {
    printf("1> Insert key\n2> Delete key\n3> Search key\nanything else> Exit\n>>> ");
    scanf("%d", &x);
    if (x == 1) {
      printf("Key> ");
      scanf("%d", &x);
      avl.insert(x);
    } else if (x == 2) {
      printf("Key> ");
      scanf("%d", &x);
      avl.remove(x);
    } else if (x == 3) {
      printf("Key> ");
      scanf("%d", &x);
      printf("%s\n", avl.search(x) ? "Found!" : "Not found!");
    } else {
      break;
    }

    printf("\n");
    avl.print();
    printf("\n");
  }

  return 0;
}
