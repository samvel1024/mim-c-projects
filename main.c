#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 1000
#define NONE '_'


/**
 * Stores the input data for the problem
 */
typedef struct incidence_matrix {

  int cols;
  int rows;
  bool filter[MAX_LEN];
  char matrix[MAX_LEN][MAX_LEN];

} Matrix;

/**
 * Read the line into array untill '\n' or EOF is reached
 * @return false if EOF is reached, true otherwise
 */
bool read_line(char buff[], int *len) {
  char c = getchar();
  *len = 0;
  while (c != '\n' && c != EOF) {
    buff[(*len)++] = c;
    c = getchar();
  }
  return c != EOF;
}

/**
 * Initialize a new input matrix by reading from stdin
 */
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

/**
 * Prints the solution after applying the filter for a set of rows stored in argument `taken`
 */
void Matrix_print_filtered(Matrix *self, const int *taken, int taken_len) {
  char selections[self->cols];
  for (int i = 0; i < taken_len; ++i) {
    for (int j = 0; j < self->cols; ++j) {
      char ch = self->matrix[taken[i]][j];
      if (ch == NONE) continue;
      selections[j] = ch;
    }
  }

  for (int i = 0; i < self->cols; ++i)
    if (self->filter[i])
      printf("%c", selections[i]);
  printf("\n");
}



//Utility functions for recursion
bool *mark_deleted(Matrix *m, bool deleted[], int selected_row);
bool *mark_covered(Matrix *m, bool covered[], int current, int selected_row);

/**
 * @param m Incidence matrix
 * @param taken Indicies of rows which are selected in the current cover
 * @param taken_len Length of taken
 * @param deleted Indicies of rows which are excluded because of having collisions with current cover
 * @param current The index of the column which is going to be covered in current step
 * @param covered Covered[i] is true if i-th column is included in cover
 */
void backtrack(Matrix *m, int taken[], int taken_len, bool deleted[], int current, bool covered[]) {

  if (current == m->cols) {
    Matrix_print_filtered(m, taken, taken_len);
    return;
  }

  for (int r = 0; r < m->rows; ++r) {
    if (deleted[r] || m->matrix[r][current] == NONE) continue;

    taken[taken_len] = r;
    bool *del_cpy = mark_deleted(m, deleted, r);
    bool *cov_cpy = mark_covered(m, covered, current, r);

    //find the next not covered column index
    int next_uncovered = current + 1;
    while (cov_cpy[next_uncovered] && next_uncovered < m->cols)
      ++next_uncovered;

    backtrack(m, taken, taken_len + 1, del_cpy, next_uncovered, cov_cpy);
    free(del_cpy);
    free(cov_cpy);
  }

}

/**
 * Returns an array denoting the excluded rows of matrix after the new selection
 */
bool *mark_deleted(Matrix *m, bool deleted[], int selected_row) {
  bool *del_cpy = malloc(sizeof(bool) * m->rows);
  memcpy(del_cpy, deleted, m->rows);
  del_cpy[selected_row] = true;
  for (int c = 0; c < m->cols; ++c) {
    if (m->matrix[selected_row][c] == NONE) continue;
    for (int r = 0; r < m->rows; ++r) {
      if (del_cpy[r]) continue;
      if (m->matrix[r][c] != NONE) {
        del_cpy[r] = true;
      }
    }
  }
  return del_cpy;
}

/**
 * Returns an array denoting covered columns after the new selected row
 */
bool *mark_covered(Matrix *m, bool covered[], int current_col, int selected_row) {
  bool *cov_cpy = malloc(sizeof(bool) * m->cols);
  memcpy(cov_cpy, covered, m->cols);
  for (int c = current_col; c < m->cols; ++c)
    if (m->matrix[selected_row][c] != NONE) cov_cpy[c] = true;
  return cov_cpy;
}


int main() {
  Matrix *m = Matrix_read();
  bool *del = calloc((size_t) m->rows, sizeof(bool));
  bool *covered = calloc((size_t) m->rows, sizeof(bool));
  int *taken = malloc(sizeof(int) * m->cols);
  backtrack(m, taken, 0, del, 0, covered);
  free(del);
  free(m);
  free(taken);
  return 0;
}
