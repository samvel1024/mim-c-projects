#include <stdio.h>
#include <stdlib.h>

int main()
{

  int n;
  scanf("%d", &n);
  int *incr = (int *)malloc(sizeof(int) * n);
  int *decr = (int *)malloc(sizeof(int) * n);
  for (int i = 0; i < n; ++i)
  {
    scanf("%d", &incr[i]);
    scanf("%d", &decr[i]);
  }
  int pos = -1;
  for (int i = 0; i < n; ++i)
  {
    if (incr[i] >= decr[i])
    {
      pos = i;
      break;
    }
  }
  int i, j;
  i = j = pos;
  int found = 0;
  int sumIncr = incr[pos];
  int sumDecr = decr[pos];
  while (i >= 0 && j < n)
  {

    if (sumIncr == sumDecr)
    {
      found = 1;
      break;
    }
    else if (sumIncr > sumDecr)
    {
      --i;
      sumIncr += (i >= 0) ? incr[i] : 0;
      sumDecr += (i >= 0) ? decr[i] : 0;
    }
    else
    {
      ++j;
      sumIncr += (j < n) ? incr[j] : 0;
      sumDecr += (j < n) ? decr[j] : 0;
    }
  }
  if (found)
  {
    printf("true\n%d %d", i, j);
  }
  else
    printf("false\n");
  return 0;
}
