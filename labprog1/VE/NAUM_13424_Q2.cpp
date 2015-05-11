#include <cmath>
#include <iostream>
#include <string>
using namespace std;

class Damas {
public:
  enum Peca {
    PECA_NENHUMA,
    PECA_BRANCO,
    PECA_PRETO,
    PECA_RAINHA_BRANCO,
    PECA_RAINHA_PRETO
  };

  enum Jogador {
    JOGADOR_NENHUM,
    JOGADOR_BRANCO,
    JOGADOR_PRETO
  };

  Damas() {
    for (int i = 0; i < 8; ++i) {
      for (int j = 0; j < 8; ++j) {
        if (i < 3 && (i + j) % 2 == 1)
          tabuleiro_[i][j] = PECA_BRANCO;
        else if (i >= 5 && (i + j) % 2 == 1)
          tabuleiro_[i][j] = PECA_PRETO;
        else
          tabuleiro_[i][j] = PECA_NENHUMA;
      }
    }

    jogada_branco_ = true;
  }

  void print() const {
    string tab;
    const char* l = "----------\n";
    tab.append(l);
    for (int i = 0; i < 8; ++i) {
      tab.push_back('|');
      for (int j = 0; j < 8; ++j) {
        Peca p = tabuleiro_[i][j];
        if      (p == PECA_NENHUMA)         tab.push_back(' ');
        else if (p == PECA_BRANCO)          tab.push_back('o');
        else if (p == PECA_PRETO)           tab.push_back('O');
        else if (p == PECA_RAINHA_BRANCO)   tab.push_back('r');
        else if (p == PECA_RAINHA_PRETO)    tab.push_back('R');
      }
      tab.append("| ");
      tab.push_back(i + '0');
      tab.push_back('\n');
    }
    tab.append(l);

    tab.push_back(' ');
    for (int i = 0; i < 8; ++i)
      tab.push_back(i + '0');
    tab.push_back('\n');
    tab.push_back('\n');

    cout << tab.c_str();
  }

  bool jogar(int x0, int y0, int x1, int y1) {
    // Verifica se a posição está dentro do tabuleiro
    if (x0 < 0 || x0 >= 8 ||
        y0 < 0 || y0 >= 8 ||
        x1 < 0 || x1 >= 8 ||
        y1 < 0 || y1 >= 8)
      return false;

    // Verifica se a posição de destino é igual à de origem
    if (x0 == x1 && y0 == y1)
      return false;

    // Verifica se o movimento é diagonal
    int dx = abs(x1 - x0),
        dy = abs(y1 - y0);
    if (dx != dy)
      return false;

    Peca& src = tabuleiro_[y0][x0];
    Peca& dst = tabuleiro_[y1][x1];

    // Verifica se é uma peça
    if (src == PECA_NENHUMA)
      return false;

    // Verifica se o destino coincide com uma peça
    if (dst != PECA_NENHUMA)
      return false;

    // Verifica se a peça é do jogador atual
    if (jogada_branco_ &&
        (src != PECA_BRANCO && src != PECA_RAINHA_BRANCO))
      return false;
    if (!jogada_branco_ &&
        (src != PECA_PRETO  && src != PECA_RAINHA_PRETO))
      return false;

    // Verifica se o sentido do movimento está correto (caso não seja rainha)
    bool vpos = (y1 - y0) > 0,
         hpos = (x1 - x0) > 0;

    if (jogada_branco_) {
      if (src == PECA_BRANCO && !vpos)
        return false;
    } else {
      if (src == PECA_PRETO  &&  vpos)
        return false;
    }

    // Verificação do movimento de peça não-rainha
    if (src == PECA_PRETO || src == PECA_BRANCO) {
      if (dx == 2) { // Andou 2 casas na diagonal
        Peca& p = tabuleiro_[y0 + (vpos ? 1 : -1)][x0 + (hpos ? 1 : -1)];

        // Verifica se a peça pulada é do adversário
        if (( jogada_branco_ && p != PECA_PRETO  && p != PECA_RAINHA_PRETO ) ||
            (!jogada_branco_ && p != PECA_BRANCO && p != PECA_RAINHA_BRANCO))
          return false;

        // Remove a peça do adversário
        p = PECA_NENHUMA;

      } else if (dx > 2){ // Andou mais de 2 casas
        return false;
      }

      // Torna a peça uma rainha caso atravesse o tabuleiro
      if ( jogada_branco_ && y1 == 7)
        src = PECA_RAINHA_BRANCO;
      if (!jogada_branco_ && y1 == 0)
        src = PECA_RAINHA_PRETO;
    } else { // Verificação do movimento da rainha
      for (int d = 1; d < dx - 1; ++d)
        if (tabuleiro_[y0 + (vpos ? d : -d)][x0 + (hpos ? d : -d)] != PECA_NENHUMA)
          return false;

      if (dx > 1) {
        Peca& p = tabuleiro_[y0 + (vpos ? dx - 1 : 1 - dx)][x0 + (hpos ? dx - 1 : 1 - dx)];
        if (p != PECA_NENHUMA) {
          if ( jogada_branco_ && p != PECA_PRETO  && p != PECA_RAINHA_PRETO )
            return false;
          if (!jogada_branco_ && p != PECA_BRANCO && p != PECA_RAINHA_BRANCO)
            return false;

          // Remove a peça do adversário
          p = PECA_NENHUMA;
        }
      }
    }

    // Efetua a jogada
    dst = src;
    src = PECA_NENHUMA;

    // Troca de jogador
    jogada_branco_ ^= 1;

    return true;
  }

  bool vitoria(bool* branco) const {
    bool b = false,
         p = false;
    for (int i = 0; i < 8; ++i) {
      for (int j = 0; j < 8; ++j) {
        Peca t = tabuleiro_[i][j];
        if (t == PECA_BRANCO || t == PECA_RAINHA_BRANCO)
          b = true;
        else if (t == PECA_PRETO || t == PECA_RAINHA_PRETO)
          p = true;

        // Tendo peça de ambos os jogadores
        if (b && p)
          return false;
      }
    }

    if (b)
      *branco = true;
    else
      *branco = false;
    // O caso de b == false && p == false deve ser desconsiderado
    // por ser um estado impossível, dado o estado inicial

    return true;
  }

  bool jogadaBranco() const { return jogada_branco_; }

private:
  Peca tabuleiro_[8][8];
  bool jogada_branco_;
};

int main()
{
  Damas jogo;

  cout << "--DAMAS--\n" << endl;

  int x0, y0, x1, y1;
  while (true)
  {
    jogo.print();

    bool b;
    if (jogo.vitoria(&b))
    {
      cout << "Vencedor: " << (b ? "Branco" : "Preto") << endl;
      break;
    }

    cout << "Jogada " << (jogo.jogadaBranco() ? "branco" : "preto") << " (x0, y0, x1, y1): ";
    cin >> x0 >> y0 >> x1 >> y1;

    if (!jogo.jogar(x0, y0, x1, y1))
      cout << "Jogada invalida!" << endl;

    cout << endl;
  }

  return 0;
}
