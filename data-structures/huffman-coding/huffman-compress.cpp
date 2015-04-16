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
  int freq;

  bool parent_left;
  int parent_id;

  int left_id;
  int right_id;
};

bool operator< (const Node& a, const Node& b)
{
  return a.freq > b.freq;
}

void print(const std::vector<Node>& nodes, int id)
{
  printf("%2d (%3d) %3d %2d %2d %2d %d\n",
         id, (int)nodes[id].value, nodes[id].freq,
         nodes[id].parent_id, nodes[id].left_id, nodes[id].right_id,
         nodes[id].parent_left);
  if (nodes[id].left_id >= 0)
    print(nodes, nodes[id].left_id);
  if (nodes[id].right_id >= 0)
    print(nodes, nodes[id].right_id);
}

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    fprintf(stderr, "Usage: %s <file>\n", argv[0]);
    return 1;
  }

  std::ifstream origFile {argv[1]};
  if (!origFile.good())
  {
    fprintf(stderr, "Could not open file: %s\n", argv[1]);
    return 1;
  }

  unsigned file_size;
  origFile.seekg (0, origFile.end);
  file_size = origFile.tellg();
  origFile.seekg (0, origFile.beg);

  // Extraction
  std::map<char, unsigned> freqMap;
  char extracted;

  while (origFile.get(extracted))
  {
    if (freqMap.find(extracted) != freqMap.end())
      freqMap[extracted]++;
    else
      freqMap[extracted] = 1;
  }

  origFile.close();

  // Create huffman tree
  std::map<char, int> valueNode;
  std::vector<Node> nodes;
  std::priority_queue<Node> huffmanQueue;

  int numLeaves = 0;
  for (auto it = freqMap.begin(); it != freqMap.end(); ++it)
  {
    nodes.push_back({});
    Node& node = nodes.back();

    node.id = nodes.size() - 1;
    node.is_leaf = true;
    node.value = it->first;
    node.freq = it->second;
    node.parent_id = -1;
    node.left_id   = -1;
    node.right_id  = -1;

    valueNode[node.value] = node.id;

    huffmanQueue.push(node);

    numLeaves++;
  }

  while (huffmanQueue.size() > 1)
  {
    int lid = huffmanQueue.top().id; huffmanQueue.pop();
    int rid = huffmanQueue.top().id; huffmanQueue.pop();

    nodes.push_back({});
    Node& c = nodes.back();

    c.id = nodes.size() - 1;

    c.is_leaf = false;
    c.freq = nodes[lid].freq + nodes[rid].freq;
    c.parent_id = -1;

    c.left_id = lid;
    nodes[lid].parent_id = c.id;
    nodes[lid].parent_left = false;

    c.right_id = rid;
    nodes[rid].parent_id = c.id;
    nodes[rid].parent_left = true;

    huffmanQueue.push(c);
  }

  //
  printf("file size / symbols: %u %zd\n", file_size, nodes.size());
  /*
  for (unsigned i = 0; i < nodes.size(); ++i)
    printf("%2d: %2d (%3d) %3d %2d %2d %2d %d\n",
           i, nodes[i].id, (int)nodes[i].value, nodes[i].freq,
           nodes[i].parent_id, nodes[i].left_id, nodes[i].right_id,
           nodes[i].parent_left);
  printf("\n");
  */


  // Create output file
  std::string comprName = argv[1];
  comprName += "-compressed";

  std::ofstream comprFile {comprName, std::ios::binary};

  unsigned qntBits = 0;
  std::stack<bool> binStack;
  std::queue<bool> bin;

  // Write huffman tree
  comprFile << numLeaves;
  for (int i = 0; i < numLeaves; ++i)
    comprFile << nodes[i].value;

  std::function<void (int)> saveTree = [&, numLeaves] (int id)
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
