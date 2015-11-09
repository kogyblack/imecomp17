#include <iostream>
#include <string>
#include <bitset>

using namespace std;
const int HD_SIZE = 1048576; // 1MB

class RAID {
public:
  // Constructor
  RAID(): pointer(0) {
    for(int i=0; i<5; ++i) integrate[i] = true;
  }

  // The idea of having 5 HDs is to simplify the storage of the characters of the message.
  // Each character have 8 bits so its possible to split each character in 2 nibbles and
  // store one bit of each nibble (plus a parity bit) in each of the 5 HDs.

  // Using 9 HDs would be simplier, but Kurose used 5 in his example on the book and there
  // were no urging need to make the problem simplier so we stuck with 5.

  // Visualization:
  // pointer = n+1;
  //                    pos 0     pos 1     pos 2     pos 3      ...      pos n    pos n+1
  // HD 1  (disk[0]) |    0    |    1    |    0    |    1    |   ...   |    0    |         |
  // HD 2  (disk[1]) |    0    |    0    |    0    |    1    |   ...   |    1    |         |
  // HD 3  (disk[2]) |    1    |    1    |    1    |    1    |   ...   |    0    |         |
  // HD 4  (disk[3]) |    0    |    0    |    0    |    0    |   ...   |    1    |         |
  // HD 5  (disk[4]) |    1    |    1    |    1    |    0    |   ...   |    1    |         |
  //                        char 1              char 2           ...                  ^
  //                                                                                  |
  //                                                                               pointer

  // Store the message, storing char by char.
  void store_message(string message) {
    for(int i=0; i<message.length(); ++i) store_char(message[i]);
  }

  // Recover the message, recovering each char of the message from the HDs.
  string recover_message() {
    string message;
    for(int i=0; i<pointer/2; ++i) {
      message += recover_char(2*i);
    }
    return message;
  }

  // Corrupt a disk, setting all its bits to 0 and setting the flag "integrate" to false.
  void corrupt_disk(int i) {
    disks[i].reset();
    integrate[i] = false;
  }

  // Attempt to recover from disk corruption.
  // If more than one disk were corrupted returns false (wasn't able to correct it).
  // If one or less disk was corrupted returns true and (if needed) recover it.
  bool recover_disk() {
    int corrupted_disks=0, corrupted;
    for(int i=0; i<5; ++i) if (!integrate[i]) corrupted_disks++, corrupted = i;
    if (corrupted_disks > 1) return false;

    for(int i=0; i<pointer; ++i) {
      bool recovered = false;
      for(int j=0; j<5; ++j) recovered ^= disks[j][i];
      disks[corrupted][i] = recovered;
    }
    return true;
  }

  int print() {
    cout << "-- HDS --\n1 2 3 4 5\n---------" << endl;
    for (int i = 0; i < pointer; ++i) {
      cout << disks[0][i] << " "
           << disks[1][i] << " "
           << disks[2][i] << " "
           << disks[3][i] << " "
           << disks[4][i] << endl;
    }
  }

private:
  // Each bitset simulates a HD with 1 MB.
  bitset<HD_SIZE> disks[5];
  // This array describes wich of the HDs are still in functional.
  bool integrate[5];
  // Pointer is the index of the first empty position in the HDs.
  int pointer;



  // Store a character in two positions of each of the 5 HDs.
  // Each char can be seen as 2 nibbles. Since we have 5 HDs this makes the problem easier.
  void store_char(char c) {
    bitset<8> bits;
    bitset<4> nibble;
    bits = char_to_bit(c);
    for(int i=0; i<4; ++i) nibble[i] = bits[i];
    store_nibble(nibble);
    for(int i=0; i<4; ++i) nibble[i] = bits[4+i];
    store_nibble(nibble);
  }

  // Store a nibble in the first empty position of the HDs.
  void store_nibble(bitset<4> bits) {
    bool parity = false;
    for(int i=0; i<4; ++i) parity ^= bits[i];
    for(int i=0; i<parity_bit(pointer); ++i) disks[i][pointer] = bits[i];
    disks[parity_bit(pointer)][pointer] = parity;
    for(int i=parity_bit(pointer); i<4; ++i) disks[i+1][pointer] = bits[i];
    pointer++;
  }

  // Given the position of the first nibble, recover the char store in it.
  char recover_char(int position) {
    int bit_pos = 0;
    int c = 0;
    for(int i=0; i<5; ++i)
      if (i == parity_bit(position)) continue;
      else c += (disks[i][position] << bit_pos++);
    position++;
    for(int i=0; i<5; ++i)
      if (i == parity_bit(position)) continue;
      else c += (disks[i][position] << bit_pos++);
    return char(c);
  }

  /***********************
   * Auxiliary Functions *
   **********************/

  // Given a character return the ASCII byte associated with it.
  bitset<8> char_to_bit(char c) {
    bitset<8> bits;
    for(int i=0; i<8; ++i) bits[i] = c & (1 << i);
    return bits;
  }

  // Given a position in the HD return which HD stores the parity bit.
  int parity_bit(int position) {return position % 5;}
};


void questao3() {
  string message, recovered_message;
  int hd, success;
  RAID raid = RAID();

  cout << "Insira a mensagem a ser armazenada: ";
  getline(cin, message);
  raid.store_message(message);

  recovered_message = raid.recover_message();
  //cout << "A mensagem armazenada foi:" << endl << recovered_message << endl;
  cout << "Estado dos HDs com a mensagem armazenada:" << endl;

  raid.print();

  cout << "Escolha um HD para ser corrompido (de 1 a 5): " << endl;
  cin >> hd;
  raid.corrupt_disk(hd-1);

  raid.print();

  recovered_message = raid.recover_message();
  cout << "A mensagem corrompida eh:" << endl << recovered_message << endl;

  success = raid.recover_disk();
  if (success) {
    recovered_message = raid.recover_message();
    cout << "Recuperacao do HD " << hd << " feita com sucesso!";
    cout << "A mensagem recuperada eh:" << endl << recovered_message << endl;
  }
  else {
    cout << "Não foi possível recuperar o HD" << endl;
  }
}
