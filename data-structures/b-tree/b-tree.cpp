#include <cstdio>

template<typename T, std::size_t D>
class BTree {
public:
  BTree();
  ~BTree();

  void push(T);
  void pop(T);

  // Debug
  void print() const;
private:
  struct Page {
    std::size_t size, index;
    T values[2 * D + 1];
    Page* parent;
    Page* children[2 * D + 2];
  };

  Page* root_;

  Page* createPage();
  void deletePage(Page*);

  void insertValueInLeaf(T, Page*);
  void insertValueInPageIndex(T, Page*, std::size_t);

  // Debug
  mutable int pageCount;
  void printPage(Page*, int depth) const;
};

template<typename T, std::size_t D>
BTree<T, D>::
BTree()
{
  root_ = createPage();
}

template<typename T, std::size_t D>
void BTree<T, D>::
push(T value) {
  Page* page = root_;
  for (;;) {
    std::size_t i;
    // TODO(naum): Binary search
    for (i = 0; i < page->size; ++i)
      if (page->values[i] > value)
        break;
    if (page->children[i] == NULL)
      break;
    page = page->children[i];
  }

  insertValueInLeaf(value, page);

  // Split if page size is 2D + 1
  while (page->size == 2 * D + 1) {
    Page* brother = createPage();
    brother->size = D;
    for (std::size_t i = 0; i < D; ++i)
      brother->values[i] = page->values[i + D + 1];
    for (std::size_t i = 0; i < D + 1; ++i)
      brother->children[i] = page->children[i + D + 1];
    for (std::size_t i = 0; i < D + 1; ++i) {
      if (brother->children[i]) {
        brother->children[i]->index = i;
        brother->children[i]->parent = brother;
      }
    }

    page->size = D;

    if (page->parent) {
      insertValueInPageIndex(page->values[D], page->parent, page->index);
      page->parent->children[page->index + 1] = brother;
    } else {
      Page* parent = createPage();
      parent->size = 1;
      parent->values[0] = page->values[D];
      parent->children[0] = page;
      parent->children[1] = brother;

      page->index = 0;
      page->parent = parent;

      root_ = parent;
    }

    brother->parent = page->parent;
    brother->index = page->index + 1;

    page = page->parent;
  }
}

template<typename T, std::size_t D>
typename BTree<T, D>::Page* BTree<T, D>::
createPage() {
  Page* newpage = new Page;
  newpage->index  = 0;
  newpage->size   = 0;
  newpage->parent = NULL;
  for (std::size_t i = 0; i < 2 * D + 1; ++i)
    newpage->children[i] = NULL;
  return newpage;
}

template<typename T, std::size_t D>
BTree<T, D>::
~BTree() {
  deletePage(root_);
}

template<typename T, std::size_t D>
void BTree<T, D>::
deletePage(Page* page) {
  if (!page)
    return;
  for (std::size_t i = 0; i < page->size; ++i)
    deletePage(page->children[i]);
  delete page;
}


template<typename T, std::size_t D>
void BTree<T, D>::
insertValueInLeaf(T value, Page* page) {
  int low = 0,
      hi = page->size;
  int mid;
  while (low < hi) {
    mid = (low + hi) / 2;
    if (page->values[mid] > value) {
      hi = mid;
    } else {
      low = mid + 1;
    }
  }

  page->size++;
  for (int i = page->size - 1; i > low; --i)
    page->values[i] = page->values[i-1];

  page->values[low] = value;
}

// IMPORTANT(naum): This will not work in leaves!!! Use only in internal pages!
template<typename T, std::size_t D>
void BTree<T, D>::
insertValueInPageIndex(T value, Page* page, std::size_t index) {
  page->size++;
  for (std::size_t i = page->size - 1; i > index; --i)
    page->values[i] = page->values[i-1];
  for (std::size_t i = page->size; i > index + 1; --i) {
    page->children[i] = page->children[i-1];
    page->children[i]->index = i; // NOTE(naum): children[i] should always be valid
  }

  page->values[index] = value;
  page->children[index + 1] = NULL;
}

template<typename T, std::size_t D>
void BTree<T, D>::
print() const {
  pageCount = 0;
  printPage(root_, 0);
}

template<typename T, std::size_t D>
void BTree<T, D>::
printPage(Page* page, int depth) const {
  if (page == NULL)
    return;

  printf("%p %8p page %2d size %zu index %zu depth %d values:",
         page, page->parent,
         pageCount++, page->size, page->index, depth);
  for (std::size_t i = 0; i < page->size; ++i)
    printf(" %d", page->values[i]);
  printf(" children ");
  for (std::size_t i = 0; i < page->size + 1; ++i)
    printf("%d", page->children[i] ? 1 : 0);

  printf("\n");
  for (std::size_t i = 0; i < page->size + 1; ++i)
    printPage(page->children[i], depth + 1);
}

int main() {
  BTree<int, 1> btree;

  int x;
  while (scanf("%d", &x) != EOF && x) {
    btree.push(x);
    btree.print();
  }

  return 0;
}
