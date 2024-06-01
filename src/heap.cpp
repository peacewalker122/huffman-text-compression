#include "../include/heap.h"
#include <stdexcept>
#include <vector>

// Returns the size of the heap
const int MaxHeap::size() { return heap.size(); }

// Returns true if the heap is empty
bool MaxHeap::empty() { return heap.empty(); }

// Inserts a new element into the heap
void MaxHeap::insert(CharacterFrequency element) {
  heap.push_back(element);
  heapifyUp(size() - 1);
}

// Extracts the maximum element from the heap
CharacterFrequency MaxHeap::extractMax() {
  if (empty()) {
    throw std::runtime_error("Heap is empty");
  }

  CharacterFrequency maxElement = heap.front();
  heap[0] = heap.back();
  heap.pop_back();

  heapifyDown(0);

  return maxElement;
}

// Returns the maximum element without removing it
CharacterFrequency MaxHeap::getMax() {
  if (empty()) {
    throw std::runtime_error("Heap is empty");
  }

  return heap.front();
}
