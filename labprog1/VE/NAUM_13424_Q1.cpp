#include <ctime>
#include <cstdlib>
#include <iostream>
#include <algorithm>
using namespace std;

class MetodoBusca {
public:
  MetodoBusca(int qnt) : qnt_(qnt), size_(0) { elems_ = new int[qnt_]; }
  ~MetodoBusca() { delete [] elems_; }

  bool inserir(int elem) {
    if (size_ >= qnt_)
      return false;

    elems_[size_++] = elem;
    return true;
  }

  bool remover(int index) {
    if (index < 0 || index >= size_)
      return false;

    size_--;
    for (int i = index; i < size_; ++i)
      elems_[i] = elems_[i+1];

    return true;
  }

  virtual bool busca(int elem) =0;

protected:
  int* elems_;
  int qnt_;
  int size_;
};

class BuscaLinear : public MetodoBusca {
public:
  BuscaLinear(int max) : MetodoBusca(max) {}

  virtual bool busca(int elem) {
    for (int i = 0; i < size_; ++i) {
      if (elem == elems_[i])
        return true;
    }
    return false;
  }
};

class BuscaBinaria : public MetodoBusca {
public:
  BuscaBinaria(int max) : MetodoBusca(max) {}

  virtual bool busca(int elem) {
    sort(elems_, elems_ + size_);

    int lo = 0, hi = size_;
    int mid, tmp;
    while (lo < hi) {
      mid = (lo + hi) / 2;
      tmp = elems_[mid];

      if (elem == tmp)
        return true;
      else if (elem > tmp)
        lo = mid + 1;
      else
        hi = mid;
    }

    return false;
  }
};

class BuscaAleatoria : public MetodoBusca {
public:
  BuscaAleatoria(int max) : MetodoBusca(max) {}

  virtual bool busca(int elem) {
    random_shuffle(elems_, elems_ + size_);

    for (int i = 0; i < size_; ++i)
      if (elem == elems_[i])
        return true;
    return false;
  }
};

bool buscar(MetodoBusca& mb, int* vet, int n, int k) {
  for (int i = 0; i < n; ++i)
    mb.inserir(vet[i]);
  return mb.busca(k);
}

int main() {
  srand(time(NULL));
  int n = 100, m = 50;
  int k;
  int v[100];

  BuscaAleatoria mb1 (n);
  BuscaLinear    mb2 (n);
  BuscaBinaria   mb3 (n);

  for (int i = 0; i < n; ++i)
    v[i] = rand() % m;

  k = rand() % m;

  cout << "BuscaAleatoria: " << (buscar(mb1, v, n, k) ? "Sim" : "Nao") << endl;
  cout << "BuscaLinear   : " << (buscar(mb2, v, n, k) ? "Sim" : "Nao") << endl;
  cout << "BuscaBinaria  : " << (buscar(mb3, v, n, k) ? "Sim" : "Nao") << endl;

  return 0;
}
