#include <stdio.h>
#include <math.h>
#include <float.h>

typedef struct {
    double width;
    double height;
} rectangle_t;

int equals(double x, double y) {
    return fabs(x - y) < (DBL_EPSILON * fabs(x + y));
}

int custom_round(double number) {
    int int_number = (int) number;
    if (equals(int_number, number)) {
        return int_number;
    }
    return int_number + 1;
}

int bigger_then(double number, double bigger_then) {
    return number > bigger_then && !equals(number, bigger_then);
}

int count_needed(rectangle_t piece, rectangle_t desired, double overlap) {
    int width = 1;
    int height = 1;
    desired.width -= piece.width;
    desired.height -= piece.height;
    if (bigger_then(desired.width, 0) || bigger_then(desired.height, 0)) {
        if (bigger_then(desired.width, 0)) {
            if (piece.width <= overlap) {
                return -1;
            } else {
                piece.width -= overlap;
                width += custom_round(desired.width / piece.width);
            }
        }
        if (bigger_then(desired.height, 0)) {
            if (piece.height <= overlap) {
                return -1;
            } else {
                piece.height -= overlap;
                height += custom_round(desired.height / piece.height);
            }
        }
        return height * width;
    }
    return 1;
}

void rotate_rectangle(rectangle_t *rectangle) {
    double tmp = rectangle->height;
    rectangle->height = rectangle->width;
    rectangle->width = tmp;
}

int minimal_count(rectangle_t *piece, rectangle_t *desired, double overlap) {
    int count = count_needed(*piece, *desired, overlap);
    rotate_rectangle(piece);
    int rotated_count = count_needed(*piece, *desired, overlap);
    return count < rotated_count && count > 0 ? count : rotated_count;
}

int verify_rectangle(rectangle_t *rectangle) {
    return bigger_then(rectangle->height, 0) && bigger_then(rectangle->width, 0);
}

int main(int argc, char **argv) {
    rectangle_t piece, desired;
    double overlap;
    printf("Velikost latky:\n");
    int needed;
    if (scanf("%lf %lf", &piece.width, &piece.height) == 2 &&
        verify_rectangle(&piece)) {
        printf("Velikost plachty:\n");
        if (scanf("%lf %lf", &desired.width, &desired.height) == 2 &&
            verify_rectangle(&desired)) {
            needed = minimal_count(&piece, &desired, 0);
            if (needed > 1) {
                printf("Prekryv:\n");
                if (scanf("%lf", &overlap) == 1 && overlap >= 0) {
                    needed = minimal_count(&piece, &desired, overlap);
                    if (needed != -1) {
                        printf("Pocet kusu latky: %d\n", needed);
                        return 0;
                    } else {
                        printf("Nelze vyrobit.\n");
                        return 1;
                    }
                }
            } else {
                printf("Pocet kusu latky: %d\n", needed);
                return 0;
            }
        }
    }
    printf("Nespravny vstup.\n");
    return 1;
}
