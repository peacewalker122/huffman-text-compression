#ifndef HEAP_H
#define HEAP_H

#include <stdexcept>
#include <vector>

// Define the struct CharacterFrequency
struct CharacterFrequency {
  char character;
  int frequency;

  // Constructor
  CharacterFrequency(char ch, int freq) : character(ch), frequency(freq) {}

  // Overload the < operator for min-heap
  bool operator<(const CharacterFrequency &other) const {
    return frequency > other.frequency;
  }
};

class MaxHeap {
private:
  std::vector<CharacterFrequency> heap;

  void heapifyUp(int index) {
    if (index && heap[index].frequency > heap[parent(index)].frequency) {
      std::swap(heap[index], heap[parent(index)]);
      heapifyUp(parent(index));
    }
  }

  void heapifyDown(int index) {
    int left = leftChild(index);
    int right = rightChild(index);
    int largest = index;
    if (left < heap.size() && heap[left].frequency > heap[largest].frequency) {
      largest = left;
    }
    if (right < heap.size() &&
        heap[right].frequency > heap[largest].frequency) {
      largest = right;
    }
    if (largest != index) {
      std::swap(heap[index], heap[largest]);
      heapifyDown(largest);
    }
  }

  int parent(int index) { return (index - 1) / 2; }
  int leftChild(int index) { return 2 * index + 1; }
  int rightChild(int index) { return 2 * index + 2; }

public:
  // Returns the size of the heap
  const int size();
  // Returns true if the heap is empty
  bool empty();
  // Inserts a new element into the heap
  void insert(CharacterFrequency element);
  // Extracts the maximum element from the heap
  CharacterFrequency extractMax();
  // Returns the maximum element without removing it
  CharacterFrequency getMax();
};

#endif
