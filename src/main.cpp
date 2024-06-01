#include "../include/huffmantree.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

// Function to parse command-line arguments
std::map<std::string, std::string> deserializeArgs(int argc, char *argv[]) {
  std::map<std::string, std::string> argsMap;
  std::map<std::string, std::string> optionMap = {{"-c", "compress"},
                                                  {"-d", "decompress"},
                                                  {"-f", "file"},
                                                  {"-o", "output"}};
  std::string currentKey;

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];

    if (arg[0] == '-') {
      if (optionMap.find(arg) != optionMap.end()) {
        currentKey = optionMap[arg];
        argsMap[currentKey] = "";
      } else {
        currentKey = arg;
        argsMap[currentKey] = "";
      }
    } else {
      if (!currentKey.empty()) {
        argsMap[currentKey] = arg;
        currentKey.clear();
      }
    }
  }

  return argsMap;
}

// usage: compression -c -f input.txt -o output.huf for compression
// usage: compression -d -f input.huf -o output.txt for decompression
//
// NOTE:
// first bencmark we got 2.524s for compression.
// second benchmark we got 2.157s for compression.
int main(int argc, char *argv[]) {
  std::map<char, long long> charTable;
  std::map<std::string, std::string> argsMap = deserializeArgs(argc, argv);
  std::ifstream inputFile;
  std::ofstream outputFile;

  if (argsMap.find("compress") != argsMap.end()) {
    if (argsMap.find("file") != argsMap.end()) {
      std::string file_name = argsMap["file"];

      if (file_name.find(".txt") == std::string::npos) {
        std::cerr << "Error: File must be a .txt file" << std::endl;
        return 1;
      }

      inputFile.open(file_name);
    }

    if (argsMap.find("output") != argsMap.end()) {
      std::string output_file = argsMap["output"];
      outputFile.open(output_file, std::ios::binary);
    }

    if (!inputFile) {
      std::cerr << "Error opening input file! " << std::strerror(errno)
                << std::endl;
      return 1;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
      for (char c : line) {
        if (charTable.find(c) == charTable.end()) {
          charTable[c] = 1;
        } else {
          charTable[c]++;
        }
        charTable['\n']++;
      }
    }

    HuffmanNode *root = HuffmanCoding::buildTree(charTable);
    HuffmanCoding::writeHeader(charTable, outputFile);
    auto map = HuffmanCoding::buildMap(root);
    HuffmanCoding::processFile(map, outputFile, inputFile);

    inputFile.close();
    if (outputFile) {
      outputFile.close();
    }

    HuffmanCoding::destroyTree(root);
  } else if (argsMap.find("decompress") != argsMap.end()) {
    if (argsMap.find("file") != argsMap.end()) {
      inputFile.open(argsMap["file"], std::ios::binary);
    }

    if (argsMap.find("output") != argsMap.end()) {
      std::string output_file = argsMap["output"];
      outputFile.open(output_file);
    }

    if (!inputFile) {
      std::cerr << "Error opening input file! " << std::strerror(errno)
                << std::endl;
      return 1;
    }

    // NOTE: decoding strategy
    // 1. read all the headers and store it on map.
    // 2. create the huffman tree from the headers_map.
    // 3. read the compressed file and decode it using the huffman tree
    // 4. wrote the decoded data to the output file
    std::map<char, long long> frequencies =
        HuffmanCoding::readHeader(inputFile);
    HuffmanNode *root = HuffmanCoding::buildTree(frequencies);
    HuffmanCoding::readCompressedFile(inputFile, root, outputFile);

    inputFile.close();
    if (outputFile) {
      outputFile.close();
    }

    // HuffmanCoding::destroyTree(root);
  } else {
    std::cerr
        << "Usage: compression -c -f input.txt -o output.huf for compression"
        << std::endl;
    std::cerr
        << "Usage: compression -d -f input.huf -o output.txt for decompression"
        << std::endl;
    return 1;
  }

  return 0;
}
