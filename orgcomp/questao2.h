#include <iostream>
#include <vector>
#include <bitset>
#include <string>
#include <utility>
using namespace std;

int parity_bit = 7;
int hamming_bits[] = {0, 1, 3};
int data_bits[] = {2, 4, 5, 6};

struct Byte {
  Byte() {}
  Byte(bitset<8> b) : bits(b) {}
  Byte(char nibble) {
    // data
    bool d[4];
    for (int i = 0; i < 4; ++i)
      d[3-i] = nibble & (1<<i);

    for (int i = 0; i < 4; ++i)
      bits.set(data_bits[i], d[i]);

    // hamming
    bits.set(hamming_bits[0], d[0] ^ d[1] ^ d[3]);
    bits.set(hamming_bits[1], d[0] ^ d[2] ^ d[3]);
    bits.set(hamming_bits[2], d[1] ^ d[2] ^ d[3]);

    // parity
    bool parity = false;
    for (int i = 0; i < 7; ++i)
      parity = parity ^ bits[i];
    bits.set(parity_bit, !parity);
  }

  int recover() {
    bool p, h[3];

    // hamming
    h[0] = bits[2] ^ bits[4] ^ bits[6];
    h[1] = bits[2] ^ bits[5] ^ bits[6];
    h[2] = bits[4] ^ bits[5] ^ bits[6];


    // Error detection
    int error = 0;

    // hamming
    for (int i = 0; i < 3; ++i) {
      if (h[i] != bits[hamming_bits[i]])
        error += (1<<i);
    }

    if (error != 0)
      bits.flip(error-1); // 

    // parity
    bool parity = false;
    for (int i = 0; i < 7; ++i)
      parity = parity ^ bits[i];
    p = !parity;

    if (p != bits[parity_bit]) {
      if (error) return -1; // Can't recover, 2 errors
      bits.flip(parity_bit);
      error = parity_bit;
    }

    return error;
  }

  string to_string() const {
    string s;
    for (int i = 0; i < 8; ++i)
      s += bits[i] + '0';
    return s;
  }

  char get_data() {
    char c = 0;
    for (int i = 0; i < 4; ++i)
      c += (bits[data_bits[i]]<<(3-i));
    return c;
  }

  bitset<8> bits; // bits:  0,1,3 - hamming; 2,3,5,6 - data; 7 - parity;
};

class Message {
public:
  void set_original_message(string original_message) {
    original_message_ = original_message;
    message_.clear();

    message_.reserve(2 * original_message_.length());

    for (int i = 0; i < original_message_.length(); ++i) {
      char c = original_message_[i];
      message_.push_back(Byte(c>>4));
      message_.push_back(Byte(c));
    }
  }

  void from_message(string message) {
    message_.clear();

    message_.reserve(message.length()/8);
    for (int i = 0; i < (int)message.length(); i+=8) {
      bitset<8> b;
      for (int j = 0; j < 8; ++j)
        b.set(j, message[i+j]=='1');
      message_.push_back(Byte(b));
    }

    original_message_.clear();
    for (int i = 0; i < message_.size(); i+=2)
      original_message_ += (message_[i].get_data()<<4) + message_[i+1].get_data();
  }

  bool recover(vector< pair<int, int> >& errors) {
    errors.clear();
    for (int i = 0; i < message_.size(); ++i) {
      int error = message_[i].recover();
      if (error < 0)
        return false;
      else if (error > 0)
        errors.push_back(make_pair(i, error-1));
    }

    original_message_.clear();
    for (int i = 0; i < message_.size(); i+=2)
      original_message_ += (message_[i].get_data()<<4) + message_[i+1].get_data();

    return true;
  }

  const vector<Byte>& get() const {
    return message_;
  }

  void corrupt(int byte, int bit) {
    message_[byte].bits.flip(bit);
  }

  void print_message() {
    cout << original_message_ << endl;
  }

  void print_bits() {
    for (int i = 0; i < (int)message_.size(); ++i) {
      if (i%10==0 && i != 0)
        cout << i;
      else
        cout << " " << i%10;
      cout << ": " << message_[i].to_string() << "\n";
    }
    cout << endl;
  }

private:
  string original_message_;
  vector<Byte> message_;
};

void questao2() {
  string message;
  cout << "Digite a mensagem a ser transmitida:" << endl;
  getline(cin, message);

  Message detection_message;
  detection_message.set_original_message(message);

  cout << "Bits com código de detecção:" << endl;
  detection_message.print_bits();

  int l, c;
  cout << "Insira a linha e coluna para corromper (l c):" << endl;
  cin >> l >> c;

  detection_message.corrupt(l, c);

  cout << "Mensagem corrompida:" << endl;
  detection_message.print_bits();

  vector< pair<int, int> > errors;
  bool could_recover = detection_message.recover(errors);

  if (could_recover) {
    cout << "Mensagem conseguiu ser recuperada.\n";
    cout << "Erros encontrados (linha coluna):\n";
    for (auto error : errors)
      cout << "linha: " << error.first << " coluna: " << error.second << "\n";
    cout << "\nBits da mensagem:" << endl;
    detection_message.print_bits();

    cout << "Mensagem recuperada:" << endl;
    detection_message.print_message();
  } else {
    cout << "Mensagem não pôde ser recuperada!" << endl;
  }
}
