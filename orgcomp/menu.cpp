#include "questao1.h"
#include "questao2.h"
#include "questao3.h"

int main() {
  cout << "Programas:\n";
  cout << "1> Armazenar e recuperar mensagem em Big Endian / Little Endian\n";
  cout << "2> Armazenar e recuperar mensagem utilizando código de detecção de 1 erro\n";
  cout << "3> Armazenar e recuperar mensagem utilizando RAID 5\n";

  int e = 0;
  do {
    cout << "Escolha: ";
    cin >> e;
  } while (e < 1 || e > 3);
  cin.ignore(1);

  if (e == 1) questao1();
  if (e == 2) questao2();
  if (e == 3) questao3();

  return 0;
}
