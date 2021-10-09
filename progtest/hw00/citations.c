#include <stdio.h>

int main(int argc, char *argv[]) {
    int index = 0;
    const char *citations[5] = {
            "noH QapmeH wo' Qaw'lu'chugh yay chavbe'lu' 'ej wo' choqmeH may' DoHlu'chugh lujbe'lu'.",
            "Qu' buSHa'chugh SuvwI', batlhHa' vangchugh, qoj matlhHa'chugh, pagh ghaH SuvwI''e'.",
            "qaStaHvIS wa' ram loS SaD Hugh SIjlaH qetbogh loD.",
            "Ha'DIbaH DaSop 'e' DaHechbe'chugh yIHoHQo'.",
            "leghlaHchu'be'chugh mIn lo'laHbe' taj jej."
    };
    printf("ml' nob:\n");
    if (scanf("%d", &index) == 1 && index >= 1 && index <= 5) {
        printf("Qapla'\n%s\n", citations[index - 1]);
        return 0;
    }
    printf("luj\n");
    return 1;
}