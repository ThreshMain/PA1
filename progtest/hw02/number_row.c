#include <stdio.h>
#include <stdlib.h>

typedef struct {
    long number;
    int index;
    int digits;
} result;

void print_number(long number, int base, int digits_count) {
    if (number == 0) {
        putchar('0');
        return;
    }
    char *digits = (char *) malloc(sizeof(int) * digits_count);
    int index = 0;
    while (number > 0) {
        long digit = number % base;
        digits[index++] = (char) (digit + (digit > 9 ? 'a' - 10 : '0'));
        number /= base;
    }
    for (int g = index - 1; g >= 0; g--) {
        putchar(digits[g]);
    }
    free(digits);
}

result get_number(long index, int base) {
    long current_index;
    long power = base;
    int digits = 1;
    while (1) {
        current_index = (digits++) * power;
        if (current_index > index) {
            digits--;
            result result;
            result.number = index / digits;
            result.index = index % digits;
            result.digits = digits;
            return result;
        }
        index += power;
        power *= base;
    }
}

int main(void) {
    long position;
    int base;
    printf("Pozice a soustava:\n");
    int err;
    while ((err = scanf("%ld %d", &position, &base)) == 2 && position >= 0 && base >= 2 && base <= 36) {
        result result = get_number(position, base);
        print_number(result.number, base, result.digits);
        putchar('\n');
        for (int i = 0; i < result.index; ++i) {
            putchar(' ');
        }
        printf("%c\n", '^');
    }
    if (err != EOF) {
        printf("Nespravny vstup.\n");
        return 1;
    }
    return 0;
}