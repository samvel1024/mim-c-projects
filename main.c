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

  for (int i = 0; i < m->cols; ++i)
    if (m->filter[i])
      printf("%c", selections[i]);
  printf("\n");
}

void backtrack(Matrix *m, int taken[], int taken_len, bool deleted[], int current, bool covered[]) {

  if (current == m->cols) {
    print_filtered(m, taken, taken_len);
    return;
  }

  for (int r = 0; r < m->rows; ++r) {
    if (deleted[r] || m->matrix[r][current] == NONE) continue;
    bool del_cpy[m->rows];
    bool cov_cpy[m->rows];
    memcpy(cov_cpy, covered, m->rows);
    memcpy(del_cpy, deleted, m->rows);
    del_cpy[r] = true;
    taken[taken_len] = r;
    int curr_covered = 0, curr_del = 0;
    mark_deleted(m, del_cpy, r, &curr_del, &curr_covered);
    for (int c = current; c < m->cols; ++c)
      if (m->matrix[r][c] != NONE) cov_cpy[c] = true;
    int next_uncovered = current + 1;
    while (cov_cpy[next_uncovered] && next_uncovered < m->cols)
      ++next_uncovered;
    backtrack(m, taken, taken_len + 1, del_cpy, next_uncovered, cov_cpy);
  }

}


int main() {
  Matrix *m = Matrix_read();
  bool *del = calloc((size_t) m->rows, sizeof(bool));
  bool *covered = calloc((size_t) m->rows, sizeof(bool));
  int taken[m->cols];
  backtrack(m, taken, 0, del, 0, covered);
  free(del);
  free(m);
  return 0;
}
