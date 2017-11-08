#include <stdio.h>

int main(){
    int i;
    scanf("%d", &i);
    printf("First breakpoint passsed");
    while(1){
        ++i;
    }
    printf("Qwerty %d", i);
    printf("\n");
    return 0;
}
