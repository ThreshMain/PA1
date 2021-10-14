#include <stdio.h>
#include <math.h>

double calculator() {
    double x, y, result=0;
    char symbol, equals;
    printf("Zadejte vzorec:\n");
    if (scanf("%lf %c %lf %c", &x, &symbol, &y, &equals) == 4 && equals == '=') {
        switch (symbol) {
            case '+':
                result = x + y;
                break;
            case '-':
                result = x - y;
                break;
            case '*':
                result = x * y;
                break;
            case '/':
                if (y == 0) {
                    return 1;
                }
                ;
                result = (result=x/y)>0?floor(result): ceil(result);
                break;
            default:
                return 1;
        }
        printf("%.12g\n",result);
        return 0;
    }
    return 1;
}

int main(int argc, char **argv) {
    if (calculator() == 1) {
        printf("Nespravny vstup.\n");
        return 1;
    }
    return 0;
}
