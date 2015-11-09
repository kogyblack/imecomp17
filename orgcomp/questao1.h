#include <algorithm>
#include <iostream>
#include <vector>
#include <bitset>
#include <string>
#include <cctype>
using namespace std;

//Palavra de 4 bytes

struct Word {
  //Pode ser um inteiro
  Word(int i) {
    memory.i = i;
    is_int = true; //Verdade se for inteiro
  }

  //Ou 4 char
  Word(char c[4]) {
    for (int i = 0; i < 4; ++i)
      memory.c[i] = c[i];
    is_int = false; //Falso se for 4 char
  }

  //Tipo que reserva o mesmo espaço de memória tanto para o int, quanto para
  //os 4 char
  union {
    int i;
    struct {
      char c[4];
    };
  } memory;

  //Flag que representa se a palavra é um inteiro ou 4 char
  bool is_int;
};

class Q1Message {
public:
  //Função que recebe a mensagem passada pelo usuário e armazena na memória
  //simulando big endian, se big_endian = true, ou little endian, caso con-
  //trário
  void receive_message(bool big_endian, string message) {
    big_endian_ = big_endian;

    for (int i = 0; i < (int)message.length(); ++i) {
      if (isdigit(message[i])) { //Se a parte lida da mensagem for um número
        //Transformar todos os dígitos em um só inteiro
        int number = 0;
        while (i < (int)message.length() && isdigit(message[i])) {
          number = number*10 + message[i++]-'0';
        }
        i--;
        //Armazenar no vector de palavras o inteiro lido
        words_.push_back(Word(number));
      } else { //Se a parte lida da mensagem for um char
        char c[4] = {}; //Não havendo 4 char seguidos, o resto de 4 é vazio
        int j, m = min((int)message.length()-i, 4);

        //Guarda até quatro caracteres da mensagem (para formar uma palavra)
        for (j = 0; j < m && !isdigit(message[i+j]); ++j)
          c[j] = message[i+j];
        i += j-1;

        //Se for little endian inverte o armazenamento da palavra
        if (!big_endian)
          for (int j = 0; j < 2; ++j)
            swap(c[j], c[3-j]);

        //Armazenar no vector de palavras os 4 char lidos
        words_.push_back(Word(c));
      }
    }
  }

  //Função que realiza a leitura do que foi armazenado como big endian, se 
  //big_endian = true, ou como little endian, caso contrário
  string get(bool big_endian) const {
    string s;
    for (int i = 0; i < (int)words_.size(); ++i) {
      Word w = words_[i]; //Pega cada palavra da memória

      if (w.is_int) //Se a palavra for um inteiro
        s += to_string(w.memory.i);
      else {//Se a palavra for o grupo de 4 char
        if (!big_endian) {//Se for little endian inverte a ordem da leitura
          for (int j = 0; j < 2; ++j)
            swap(w.memory.c[j], w.memory.c[3-j]);
        }

        for (int j = 0; j < 4; ++j)
          if (w.memory.c[j])
            s += w.memory.c[j];
      }
    }

    return s;
  }

private:
  bool big_endian_;
  vector<Word> words_;
};

void questao1() {
  string message;
  int escolha;
  cout << "Digite a mensagem:" << endl;
  getline(cin, message);

  cout << "Escolha o modo de armazenamento\n1 - Big Endian\n2 - Little Endian" << endl;
  cin >> escolha;
  Q1Message m;
  if(escolha == 1)
    m.receive_message(true, message);
  else
    m.receive_message(false, message);

  cout << "Big Endian: " << m.get(true) << "\n" << "Little Endian: " << m.get(false) << endl;
}
