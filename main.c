


#include <stdlib.h>
#include <stdio.h>

void swap(int *a, int *b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

typedef struct heap_t {
  int *arr;
  int size;
} Heap;


int Heap_extract(Heap *self, int parent){

}

void Heap_heapify(Heap *self, int parent) {

  int left = parent * 2 + 1;
  int right = left + 1;

  int *arr = self->arr;

  int largest = parent;
  if (left < self->size && arr[left] > arr[largest])
    largest = left;
  if (right < self->size && arr[right] > arr[largest])
    largest = right;

  if (largest != parent) {
    swap(&arr[largest], &arr[parent]);
    Heap_heapify(self, largest);
  }

}

void Heap_init(int arr[], int size) {
  Heap *h = malloc(sizeof(Heap));
  h->arr = arr;
  h->size = size;

  for (int i = size / 2; i >= 0; --i) {
    Heap_heapify(h, i);
  }

}


void Heap_add(Heap *self, int parent) {

}


int main() {

}
