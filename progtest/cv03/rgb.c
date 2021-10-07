#include <stdio.h>

int rgbConvert() {
    int r, g, b;
    char c;
    printf("Zadejte barvu v RGB formatu:\n");
    if (scanf(" rgb ( %d , %d , %d %c", &r, &g, &b, &c) == 4) {
        if (r < 0 || g < 0 || b < 0 || r > 255 || g > 255 || b > 255 || c != ')') {
            return -1;
        }
        printf("#%02X%02X%02X\n", r, g, b);
        return 1;
    }
    return -1;
}

int main() {
    if (rgbConvert() == 1) return 0;
    printf("Nespravny vstup.\n");
    return 1;
}

