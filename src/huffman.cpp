#include "../include/huffmantree.h"
#include <bitset>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <ostream>
#include <queue>
#include <string>
#include <vector>

// Comparator for the min-heap
struct CompareFrequency {
  bool operator()(HuffmanNode *l, HuffmanNode *r) {
    // NOTE: intended for min-heap
    return l->frequency > r->frequency;
  }
};

HuffmanNode *HuffmanCoding::buildTree(const std::map<char, long long> &freq) {
  std::priority_queue<HuffmanNode *, std::vector<HuffmanNode *>,
                      CompareFrequency>
      minHeap;

  for (const auto &pair : freq) {
    minHeap.push(new HuffmanNode(pair.first, pair.second));
  }

  while (minHeap.size() > 1) {
    auto left = minHeap.top();
    minHeap.pop();
    auto right = minHeap.top();
    minHeap.pop();

    long long sum = left->frequency + right->frequency;
    minHeap.push(new HuffmanNode(sum, left, right));
  }

  return minHeap.top();
}

void HuffmanCoding::printCodes(HuffmanNode *root, const std::string &str) {
  if (!root) {
    return;
  }

  if (root->isLeaf()) {
    std::cout << root->character << ": " << str << std::endl;
  }

  printCodes(root->left, str + "0");
  printCodes(root->right, str + "1");
}

void HuffmanCoding::writeHeader(const std::map<char, long long> &freq,
                                std::ofstream &outputFile) {
  // Write the size of the frequency table
  const char magicNumber[4] = {'h', 'u', 'f', 'f'};
  outputFile.write(magicNumber, sizeof(magicNumber));

  size_t size = freq.size();
  outputFile.write(reinterpret_cast<const char *>(&size), sizeof(size));

  // Write each character and its frequency
  for (auto it = freq.rbegin(); it != freq.rend(); ++it) {
    outputFile.write(&it->first, sizeof(it->first));
    outputFile.write(reinterpret_cast<const char *>(&it->second),
                     sizeof(it->second));
  }
}

// TODO:
// Function to encode the input text
// Into packed byte into the outputFile
void HuffmanCoding::writeCode(HuffmanNode *root, std::ofstream &outputFile) {
  if (!root) {
    return;
  }
}

// Function to destroy the Huffman tree
void HuffmanCoding::destroyTree(HuffmanNode *root) {
  if (!root) {
    return;
  }

  destroyTree(root->left);
  destroyTree(root->right);
  delete root;
}

std::map<char, std::string> HuffmanCoding::buildMap(HuffmanNode *tree) {
  std::map<char, std::string> map;

  // Lambda function for DFS traversal
  std::function<void(HuffmanNode *, const std::string &)> dfs =
      [&](HuffmanNode *root, const std::string &str) {
        if (!root) {
          return;
        }

        if (root->isLeaf()) {
          map[root->character] = str;
          // std::cout << root->character << ": " << str << std::endl;
        }

        dfs(root->left, str + "0");
        dfs(root->right, str + "1");
      };

  // Start DFS traversal
  dfs(tree, "");
  return map;
}

// NOTE: firstly read the inputFile and proceed it
// Then write the encoded text into the outputFile
// and must be packed on 8 bits
void HuffmanCoding::processFile(std::map<char, std::string> &map,
                                std::ofstream &outputFile,
                                std::ifstream &inputFile) {
  inputFile.clear();
  inputFile.seekg(0, std::ios::beg);

  std::string encodedText;
  std::string line;
  while (std::getline(inputFile, line)) {
    for (char c : line) {
      if (map.find(c) != map.end()) {
        encodedText += map.at(c);
      } else {
        std::cerr << "Character '" << c << "' not found in Huffman map."
                  << std::endl;
        return;
      }
    }
    encodedText += map['\n'];
  }

  // Pack the encoded text into bytes and write to the output file
  std::bitset<8> bits;
  int bitIndex = 0;
  for (char bit : encodedText) {
    bitIndex++;

    // WARN: current problem. When we encode the packed byte it's not packed the
    // actual bytes instead it's packed the hexadecimal representation of
    // the bit.
    if (bitIndex % 8 == 0) {
      std::bitset<8> bits(encodedText.substr(bitIndex - 8, 8));
      long long decimal = bits.to_ulong();
      char val = static_cast<char>(decimal);
      outputFile.write(&val, sizeof(char));
      bits.reset();
    }
  }

  // Write the remaining bits (if any)
  if (bitIndex > 0) {
    std::bitset<8> bits(encodedText.substr(bitIndex - 8, 8));
    long long decimal = bits.to_ulong();
    char val = static_cast<char>(decimal);
    outputFile.write(&val, sizeof(char));
  }
}

void HuffmanCoding::readCompressedFile(std::ifstream &inputFile,
                                       HuffmanNode *root,
                                       std::ofstream &outputFile) {
  // WARN: the actual error happens because of we must decode the packed byte by
  // the map.
  //
  // Step 2: Read the encoded data
  std::string encodedText;
  std::string line;
  while (std::getline(inputFile, line)) {
    for (char c : line) {
      std::bitset<8> bits(static_cast<unsigned char>(c));
      encodedText += bits.to_string();
    }
  }

  // Step 3: Decode the data
  std::string decodedText;
  HuffmanNode *currentNode = root;
  for (char bit : encodedText) {
    if (bit == '0') {
      currentNode = currentNode->left;
    } else {
      currentNode = currentNode->right;
    }

    if (currentNode->isLeaf()) {
      decodedText += currentNode->character;
      currentNode = root;
    }
  }

  outputFile << decodedText;
}

std::map<char, long long> HuffmanCoding::readHeader(std::ifstream &inputFile) {
  char magic[4];
  inputFile.read(magic, 4);

  if (std::string(magic, 4) != "huff") {
    std::cerr << "Invalid file format." << std::endl;
    return {};
  }

  size_t charsCount;
  inputFile.read(reinterpret_cast<char *>(&charsCount), sizeof(charsCount));

  std::map<char, long long> frequencies;
  for (uint8_t i = 0; i < charsCount; ++i) {
    char character;
    long long frequency;

    inputFile.read(&character, sizeof(character));
    inputFile.read(reinterpret_cast<char *>(&frequency), sizeof(frequency));

    frequencies[character] = frequency;
  }

  return frequencies;
}
