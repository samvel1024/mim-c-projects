#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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
  int len = 0;
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


void mark_deleted(Matrix *m, bool deleted[], int selected_row, int *deletion, int *cover) {
  for (int c = 0; c < m->cols; ++c) {
    if (m->matrix[selected_row][c] == NONE) continue;
    (*cover)++;
    for (int r = 0; r < m->rows; ++r) {
      if (deleted[r]) continue;
      if (m->matrix[r][c] != NONE) {
        deleted[r] = true;
        (*deletion)++;
      }
    }
  }
}

void print_filtered(Matrix *m, int taken[], int taken_len) {
  char selections[m->cols];
  for (int i = 0; i < taken_len; ++i) {
    for (int j = 0; j < m->cols; ++j) {
      char ch = m->matrix[taken[i]][j];
      if (ch == NONE) continue;
      selections[j] = ch;
    }
  }

  for (int i=0; i<m->cols; ++i)
    if (m->filter[i])
      printf("%c", selections[i]);
  printf("\n");
}

void backtrack(Matrix *m, int taken[], int taken_len, bool deleted[], int covered, int deletions) {

  if (covered == m->cols) {
    print_filtered(m, taken, taken_len);
    return;
  }

  int initial_row = taken_len == 0 ? 0 : taken[taken_len - 1];
  for (int r = initial_row; r < m->rows; ++r) {
    if (deleted[r]) continue;
    bool del_cpy[m->rows];
    memcpy(del_cpy, deleted, m->rows);
    del_cpy[r] = true;
    taken[taken_len] = r;
    int curr_covered = 0, curr_del = 0;
    mark_deleted(m, del_cpy, r, &curr_del, &curr_covered);
    backtrack(m, taken, taken_len + 1, del_cpy, covered + curr_covered, deletions + curr_del);
  }

}


int main() {
  Matrix *m = Matrix_read();
  bool *del = calloc((size_t) m->rows, sizeof(bool));
  int taken[m->cols];
  backtrack(m, taken, 0, del, 0, 0);
  free(del);
  free(m);
  return 0;
}
