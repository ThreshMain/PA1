#ifndef __PROGTEST__

#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <stdint.h>

#endif /* __PROGTEST__ */


long long avg3(long long a, long long b, long long c) {
    long long reminder = a % 3 + b % 3 + c % 3;
    long long result = a / 3 + b / 3 + c / 3;
    printf("%lld\n", result);
    printf("%lld\n", reminder);

    result += reminder / 3;
    reminder %= 3;
    printf("%lld\n", result);
    printf("%lld\n", reminder);

    while (reminder < 0 && result > 0) {
        result--;
        reminder += 3;
    }

    while (reminder > 0 && result < 0) {
        result++;
        reminder -= 3;
    }
    printf("%lld\n", reminder);
    printf("%lld\n", result);
    return result;
}

#ifndef __PROGTEST__

int main(int argc, char *argv[]) {
    printf("---------\n");
    assert (avg3(1, 2, 3) == 2);
    printf("---------\n");
    assert (avg3(-100, 100, 30) == 10);
    printf("---------\n");
    assert (avg3(1, 2, 2) == 1);
    printf("---------\n");
    assert (avg3(-1, -2, -2) == -1);
    printf("---------\n");
    assert (avg3(LLONG_MAX, LLONG_MAX, LLONG_MAX) == LLONG_MAX);
    printf("---------\n");
    assert (avg3(9223372036854775800, 9223372036854775800, -8) == 6148914691236517197);
    printf("---------\n");
    assert (avg3(9223372036854775800, -8, -4) == 3074457345618258596);
    printf("---------\n");
    assert(avg3(9223372036854775800, -8, -8) == 3074457345618258594);
    printf("---------\n");
    assert(avg3(9223372036854775800, -4611686018427387905, -4611686018427387896) == 0);
    printf("---------\n");
    assert(avg3(9223372036854775800, -9223372036854775807, 2) == -1);
    printf("---------\n");
    assert(avg3(3, 9, -2) == 3);
    return 0;
}

#endif /* __PROGTEST__ */

