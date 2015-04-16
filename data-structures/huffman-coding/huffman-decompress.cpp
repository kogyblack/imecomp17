#include <cstdio>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <fstream>
#include <map>
#include <string>

struct Node
{
  unsigned id;

  bool is_leaf;
  char value;

  /*
  bool parent_left;
  int parent_id;
  */

  int left_id;
  int right_id;
};

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    fprintf(stderr, "Usage: %s <file>\n", argv[0]);
    return 1;
  }

  std::ifstream comprFile {argv[1]};
  if (!comprFile.good())
  {
    fprintf(stderr, "Could not open file: %s\n", argv[1]);
    return 1;
  }

  // Create huffman tree
  std::map<int, char> valueNode;
  std::vector<Node> nodes;
  //std::priority_queue<Node> huffmanQueue;

  int numLeaves = 0;

  // Create output file
  std::string origName = argv[1];
  origName += "-decompressed";

  std::ofstream origFile {origName, std::ios::binary};

  unsigned qntBits = 0;
  std::stack<bool> binStack;
  std::queue<bool> bin;

  // Write huffman tree
  comprFile >> numLeaves;
  nodes.resize(numLeaves * 2 - 1);
  for (int i = 0; i < numLeaves; ++i)
  {
    comprFile >> nodes[i].value;
    nodes[i].id = i;
    nodes[i].is_leaf = true;
    nodes[i].right_id = -1;
    nodes[i].left_id  = -1;
  }

  std::function<void (int)> loadTree = [&, numLeaves] (int id)
  {
    if (id < numLeaves)
      binStack.push(true);
    else
    {
      binStack.push(false);
      saveTree(nodes[id].left_id);
      saveTree(nodes[id].right_id);
    }
  };

  auto qntPos = comprFile.tellp();
  comprFile << qntBits; // Reserving space

  origFile.open(argv[1]);

  unsigned char c;
  while (origFile.get(extracted))
  {
    //printf("extracted: %c ", extracted);
    int cur  = valueNode[extracted];
    while (nodes[cur].parent_id != -1)
    {
      binStack.push(nodes[cur].parent_left);
      cur = nodes[cur].parent_id;
    }
    //printf("\n");

    qntBits += binStack.size();

    unsigned binsize = binStack.size();
    for (unsigned i = 0; i < binsize; ++i)
    {
      bin.push(binStack.top());
      binStack.pop();
    }

    // NOTE(naum): This stack should always be empty at this point

    while (bin.size() >= 8)
    {
      c = 0;
      for (int i = 0; i < 8; ++i)
      {
        c <<= 1;
        c += bin.front();
        bin.pop();
      }

      comprFile << c;
    }
  }

  // Incomplete byte, if any
  if (bin.size() > 0)
  {
    c = 0;
    unsigned extrabits = bin.size();
    unsigned extratobyte = 8 - extrabits;
    while (extrabits--)
    {
      c <<= 1;
      c += bin.front();
      bin.pop();
    }

    while (extratobyte--)
    {
      c <<= 1;
      c += 0;
    }
  }

  comprFile.seekp(qntPos);
  comprFile << qntBits;

  origFile.close();
  comprFile.close();

  return 0;
}
