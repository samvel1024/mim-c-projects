#include <stdio.h>


int _strlen(char *c){
  int n =0;
  while(*(c+(n++)) != '\0');
  return n-1;
}

int main(){
    char a[21] = "r32";
    printf("%d", _strlen("qwerqwer"));
    return 0;
}
