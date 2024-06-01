#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include "heap.h"
#include <fstream>
#include <map>
#include <string>
#include <vector>

class HuffmanNode {
public:
  char character;
  int frequency;
  HuffmanNode *left;
  HuffmanNode *right;

  HuffmanNode(char ch, int freq)
      : character(ch), frequency(freq), left(nullptr), right(nullptr) {}
  HuffmanNode(int freq, HuffmanNode *l, HuffmanNode *r)
      : character('\0'), frequency(freq), left(l), right(r) {}

  bool isLeaf() const { return !left && !right; }
};

class HuffmanCoding {
public:
  static HuffmanNode *buildTree(const std::map<char, long long> &freq);
  static void printCodes(HuffmanNode *root, const std::string &str);
  static void destroyTree(HuffmanNode *root);
  static void writeHeader(const std::map<char, long long> &freq,
                          std::ofstream &outputFile);
  static void writeCode(HuffmanNode *root, std::ofstream &outputFile);
  static std::map<char, std::string> buildMap(HuffmanNode *tree);
  static void processFile(std::map<char, std::string> &map,
                          std::ofstream &outputFile, std::ifstream &inputFile);

  static std::map<char, long long> readHeader(std::ifstream &inputFile);
  static void readCompressedFile(std::ifstream &inputFile, HuffmanNode *root,
                                 std::ofstream &outputFile);
};

#endif // HUFFMANTREE_H
