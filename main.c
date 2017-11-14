#include <stdio.h>

#define SEQUENCE_LEN 3

int main()
{
    int n;
    //m[a][b] shows the count of sequences of length a before index b
    int m[SEQUENCE_LEN][100000];
    int a[100000];

    scanf("%d", &n);
    int zeroCount = 0;
    for (int i = 0; i < n; ++i) {
        scanf("%d", &a[i]);
        m[0][i] = (a[i] == 0) ? ++zeroCount : zeroCount;
    }

    for (int len = 1; len < SEQUENCE_LEN; ++len) {
        int count = 0;
        for (int i = 0; i < n; ++i) {
            count = (a[i] == len) ? count + m[len - 1][i] : count;
            m[len][i] = count;
        }
    }

    printf("%d", m[SEQUENCE_LEN - 1][n - 1]);

    return 0;
}
