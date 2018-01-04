#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_LEN 1000

#define NONE '_'

typedef struct incidence_matrix {

  int cols;
  int rows;
  bool filter[MAX_LEN];
  char matrix[MAX_LEN][MAX_LEN];

} Matrix;


bool read_line(char buff[], int *len) {
  char c = getchar();
  *len = 0;
  while (c != '\n' && c != EOF) {
    buff[(*len)++] = c;
    c = getchar();
  }
  return c != EOF;
}

void Matrix_print(Matrix *self) {
  for (int i = 0; i < self->cols; ++i)
    printf("%d", self->filter[i]);
  printf("\n");
  for (int i = 0; i < self->rows; ++i) {
    for (int j = 0; j < self->cols; ++j)
      printf("%c", self->matrix[i][j]);
    printf("\n");
  }
}

Matrix *Matrix_read() {
  Matrix *m = malloc(sizeof(Matrix));
  m->rows = m->cols = 0;
  char buff[MAX_LEN];
  int len =  0;
  read_line(buff, &len);
  m->cols = len;
  for (int i = 0; i < m->cols; ++i) {
    m->filter[i] = buff[i] == '+';
  }


  bool has_next = read_line(buff, &len);
  while (has_next) {
    if (len == m->cols) {
      for (int i = 0; i < m->cols; ++i) {
        m->matrix[m->rows][i] = buff[i];
      }
      m->rows++;
    }
    has_next = read_line(buff, &len);
  }

  return m;
}


int main() {
  Matrix *m = Matrix_read();
  Matrix_print(m);
  return 0;
}
