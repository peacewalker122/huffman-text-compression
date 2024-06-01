# Introduction
This project implement the Huffman compression algorithm. Follow the guide from [codingchallenge](https://codingchallenges.fyi/challenges/challenge-huffman/).

# Building Project

1. `Clone the Repository`
```
git clone https://github.com/yourusername/huffman-compression.git
cd huffman-compression
```

2. `Compile the project`
```
cd build
cmake ..
make
```

3. `Run the project`
```
./out/compression 
```

# Usage
## Compress a File
`./out/compression -c -f input.txt -o output.huff`

## Decompress a File
`./out/compression -d -f input.huff -o output.txt`
