#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

class Empregado {
public:
  friend ostream& operator<<(ostream&, const Empregado&);
  friend istream& operator>>(istream&, Empregado&);
  bool operator<(const Empregado& rhs);

  Empregado() {}

  Empregado(string nome, int salario, int ramal) :
    nome(nome), salario(salario), ramal(ramal)
  {}

private:
  string nome;
  int salario;
  int ramal;
};

bool Empregado::operator<(const Empregado& rhs) {
  return nome < rhs.nome;
}

istream& operator>>(istream& ifs, Empregado& e) {
  getline(ifs, e.nome, ',');
  ifs >> e.salario;
  ifs.get();
  ifs >> e.ramal;
  ifs.get();

  return ifs;
}

ostream& operator<<(ostream& os, const Empregado& e) {
  return os << e.nome << ", " << e.salario << ", " << e.ramal << endl;
}


int main() {
  ifstream file ("file.txt");

  vector<Empregado> empregados;

  while (file.good()) {
    Empregado e;
    file >> e;
    if (file.good())
      empregados.push_back(e);
  }

  sort(empregados.begin(), empregados.end());

  for (int i = 0; i < (int)empregados.size(); ++i) {
    cout << empregados[i];
  }

  Empregado* ptr = &empregados[0];
  ofstream binout ("binout.txt", ios::binary);
  binout.write((char*)ptr, sizeof(Empregado) * empregados.size());

  ofstream binout2 ("binout2.txt", ios::binary);
  for (auto& it : empregados) {
    binout2.write((char*)&it, sizeof(Empregado));
  }

  return 0;
}
