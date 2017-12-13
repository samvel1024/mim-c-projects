#include <stdio.h>
#include <stdlib.h>

typedef struct list_t{
  int val;
  struct list_t *next;
}List;

List* List_new(int val, List *next){
  List *l = malloc(sizeof(List));
  l -> val = val;
  l -> next = next;
  return l;
}

List* List_new_stdin(){
  int size;
  scanf("%d", &size);
  List *l = NULL;
  while(size--){
    int val;
    scanf("%d", &val);
    l = List_new(val, NULL);


  }
  return l;
}


int main(){

}
