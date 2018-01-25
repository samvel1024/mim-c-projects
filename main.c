#include <stdio.h>
#include <string.h>


char first[1000];
int first_len;

char second[1000];
int second_len;

char best[1000];
int best_len = 0;

int find_in_second_after(int s, char c) {
  for (int i = s; i < second_len; ++i)
    if (second[i] == c)
      return i;

  return -1;
}


void naive(char common[], int common_len, int curr, int last_selected_in_second) {

  if (curr == first_len) {
    if (common_len > best_len) {
      strcpy(best, common);
      best_len = common_len;
    }
    return;
  }
  char curr_char = first[curr];
  int in_second = find_in_second_after(last_selected_in_second, curr_char);

  if (in_second == -1) {
    naive(common, common_len, curr + 1, last_selected_in_second);
  } else {
    last_selected_in_second = in_second;
    common[common_len] = curr_char;
    naive(common, common_len + 1, curr + 1, last_selected_in_second);
    naive(common, common_len, curr + 1, last_selected_in_second);
  }


}

int lcs( char *X, char *Y, int m, int n )
{
  int L[m+1][n+1];
  int i, j;

  /* Following steps build L[m+1][n+1] in bottom up fashion. Note
     that L[i][j] contains length of LCS of X[0..i-1] and Y[0..j-1] */
  for (i=0; i<=m; i++)
  {
    for (j=0; j<=n; j++)
    {
      if (i == 0 || j == 0)
        L[i][j] = 0;

      else if (X[i-1] == Y[j-1])
        L[i][j] = L[i-1][j-1] + 1;

      else
        L[i][j] = max(L[i-1][j], L[i][j-1]);
    }
  }

  /* L[m][n] contains length of LCS for X[0..n-1] and Y[0..m-1] */
  return L[m][n];
}





int main() {
  char *first = "adfmkpl";
  char *second = "jdumpkp";
  first_len = second_len = 7;
  char common[7];
  naive(common, 7, 0, 0);

  best[best_len] = '\0';
  printf("%s", best);
  return 0;
}
