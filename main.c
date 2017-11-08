#include <stdio.h>
#include <limits.h> 

int next_element(int prev);
int get_first_one_index(int first);

int main()
{
    int a, b;
    scanf("%d %d", &a, &b);
    int maxOneIndex = INT_MIN;
    int minOneSeed;
    int maxInfSeed;
    for (int seed = a; seed<= b; ++seed) {
        int val = get_first_one_index(seed);
        if (val == 0) {
            printf("%d?", a);
            return 0;
        }
        else if(val > maxOneIndex){
            minOneSeed = seed;
            maxOneIndex = val;
        }
    }

    printf("%d %d", minOneSeed, maxOneIndex);
    return 0;
}

int get_first_one_index(int prev)
{
    int index = 0;
    while (1) {
        int next = next_element(prev);
        prev = next;
        index++;
        if (next == 0)
            return 0;
        if (next == 1)
            return index;
    }
}

int next_element(int prev)
{
    if (prev % 2 == 0)
        return prev / 2;
    else if ((prev * 3 + 1) < prev)
        return 0;
    else
        return prev * 3 + 1;
}