# Huffman Compression Project Documentation

## Project Overview

This project implements the Huffman coding algorithm for file compression and decompression. Huffman coding is a lossless data compression algorithm that assigns variable-length codes to input characters based on their frequencies - characters that occur more frequently are assigned shorter codes.

## Algorithm Explanation

### Huffman Coding

Huffman coding works by creating a binary tree of nodes:

1. **Frequency Analysis**: Count the frequency of each character in the input.
2. **Build Priority Queue**: Create a min-heap (priority queue) where each node contains a character and its frequency.
3. **Build Huffman Tree**: 
   - Extract two nodes with the lowest frequencies from the queue.
   - Create a new internal node with these two nodes as children, with frequency equal to the sum of the two nodes' frequencies.
   - Add this new node back to the priority queue.
   - Repeat until only one node remains (the root of the Huffman tree).
4. **Generate Codes**: Traverse the tree to assign codes (0 for left edges, 1 for right edges) to each character.
5. **Encode Data**: Replace each character in the input with its corresponding code.

### Compression Process

1. Read the input file and count frequency of each character.
2. Build a Huffman tree based on these frequencies.
3. Generate a mapping of characters to their binary codes.
4. Write a header to the output file containing:
   - Magic number "huff" to identify the file format
   - Character frequency table for later decompression
5. Encode the input file using the generated codes.
6. Pack the bits into bytes and write to the output file.

### Decompression Process

1. Read the header from the compressed file to extract character frequencies.
2. Rebuild the same Huffman tree used during compression.
3. Read the compressed data and traverse the Huffman tree to decode:
   - For each bit, move left (0) or right (1) in the tree.
   - When a leaf node is reached, output its character and return to the root.
4. Write the decoded text to the output file.

## Code Structure

The project is organized into the following components:

### Core Classes

- **HuffmanNode**: Represents a node in the Huffman tree.
  - Contains character, frequency, and pointers to left and right children.
  - Leaf nodes contain actual characters.

- **HuffmanCoding**: Static class with methods for Huffman coding operations:
  - `buildTree`: Constructs a Huffman tree from frequency data.
  - `buildMap`: Generates character-to-code mapping from the tree.
  - `writeHeader`: Writes the file header with character frequencies.
  - `processFile`: Encodes the input file and writes to output.
  - `readHeader`: Reads the header from a compressed file.
  - `readCompressedFile`: Decodes compressed data using a Huffman tree.
  - `destroyTree`: Cleans up memory by deleting the tree.

- **MaxHeap**: Implementation of a max heap for priority queue operations.
  - Not directly used in the current implementation (the project uses STL's priority_queue instead).

### Main Executable

The main program supports two operations:
- **Compression**: Convert a text file to a Huffman-encoded file.
- **Decompression**: Restore a Huffman-encoded file back to its original text.

## Installation and Usage

### Prerequisites

- C++ compiler with C++11 support
- CMake (version 3.0 or higher)

### Building the Project

1. Clone the repository:
   ```
   git clone https://github.com/peacewalker122/huffman-compression.git
   cd huffman-compression
   ```

2. Create a build directory and compile:
   ```
   mkdir -p build && cd build
   cmake ..
   make
   ```

### Usage Examples

#### Compress a File
```
./out/compression -c -f input.txt -o output.huff
```

#### Decompress a File
```
./out/compression -d -f input.huff -o output.txt
```

## Implementation Details

### File Format

Compressed files follow this format:
1. **Magic Number**: 4 bytes containing "huff" to identify the file format.
2. **Character Count**: Size of the frequency table.
3. **Frequency Table**: Each entry contains:
   - Character (1 byte)
   - Frequency (8 bytes as long long)
4. **Encoded Data**: Bit stream of Huffman codes packed into bytes.

### Command-Line Arguments

- `-c`: Compress mode
- `-d`: Decompress mode
- `-f [filename]`: Input file
- `-o [filename]`: Output file

### Limitations and Considerations

- The current implementation works best with text files.
- The compression ratio depends on the character distribution in the input file.
- Very small files might not benefit from compression due to the overhead of storing the frequency table.

## Performance and Benchmarks

As noted in the code comments, the implementation achieved:
- First benchmark: 2.524s for compression
- Second benchmark: 2.157s for compression

Compression efficiency varies based on:
- The size of the input file
- Character frequency distribution
- Implementation optimizations

## Future Improvements

Potential enhancements to consider:
1. Adaptive Huffman coding for better compression of files with changing character distributions
2. Block-based compression for handling very large files
3. Multi-threading support for faster processing
4. Better error handling and recovery mechanisms
5. Supporting compression of binary files
6. Adding integrity checks for compressed data

## References

- [Coding Challenge: Huffman Coding](https://codingchallenges.fyi/challenges/challenge-huffman/)
- Huffman, D. A. (1952). "A Method for the Construction of Minimum-Redundancy Codes". Proceedings of the IRE. 40 (9): 1098–1101.
