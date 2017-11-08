#include <stdio.h>
#include <stdlib.h>

int main()
{

  
  int n;
  scanf("%d", &n);
  int *incr = (int*)malloc(sizeof(int) * n);
  int *decr = (int*)malloc(sizeof(int) * n);
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

  if (incr[pos] == -1)
  {
    printf("false");
    return 0;
  }
  int incrSum = incr[pos];
  int decrSum = decr[pos];
  if (incrSum == decrSum)
    printf("true");
  else if (pos >= 1 && incrSum + incr[pos - 1] == decrSum + decr[pos - 1])
    printf("true");
  else printf("false");

  return 0;
}
