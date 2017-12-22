


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


int Heap_right(int curr) {
  return (curr + 1) * 2;
}

int Heap_left(int curr) {
  return (curr + 1) * 2 - 1;
}


void Heap_max_heapify(Heap *self, int parent) {

  int *heap = self->arr;
  int left = Heap_left(parent);
  int right = Heap_right(parent);
  int max = parent;
  if (heap[left] > heap[parent])
    max = left;
  if (heap[right] > heap[max])
    max = right;

  if (max != parent) {
    swap(&heap[max], &heap[parent]);
    Heap_max_heapify(self, max);
  }
}

void Heap_build(Heap *self) {
  for (int i = self->size / 2; i >= 0; ++i) {
    Heap_max_heapify(self, i);
  }
}

Heap *Heap_new(int arr[], int size) {
  Heap *h = malloc(sizeof(Heap));
  h->arr = arr;
  h->size = size;
  return h;
}




int main() {
  int size;
  scanf("%d", &size);
  int arr[size];
  for (int i = 0; i < size; ++i) {
    scanf("%d", &arr[i]);
  }
  Heap *h = Heap_new(arr, size);
  for (int i = 0; i < size; ++i) {
    printf("%d ", arr[i]);
  }
  return 0;
}
