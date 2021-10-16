#include <stdio.h>

typedef struct {
    double width;
    double height;
} rectangle_t;


int count_needed(rectangle_t piece, rectangle_t desired, double overlap) {
    int width = 1;
    int height = 1;
    desired.width -= piece.width;
    desired.height -= piece.height;
    while (desired.width > 0 || desired.height > 0) {
        if (desired.width > 0) {
            if (piece.width <= overlap) {
                return -1;
            } else {
                width++;
                desired.width -= piece.width - overlap;
            }
        } else {
            if (piece.height <= overlap) {
                return -1;
            } else {
                height++;
                desired.height -= piece.height - overlap;
            }
        }
    }
    return width * height;
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
    return count < rotated_count && count != -1 ? count : rotated_count;
}

int verify_rectangle(rectangle_t *rectangle) {
    return rectangle->height > 0 && rectangle->width > 0;
}

int main(int argc, char **argv) {
    rectangle_t piece, desired;
    double overlap;
    printf("Velikost latky:\n");
    int needed = 0;
    if (scanf("%lf %lf", &piece.width, &piece.height) == 2 &&
        verify_rectangle(&piece)) {
        printf("Velikost plachty:\n");
        if (scanf("%lf %lf", &desired.width, &desired.height) == 2 &&
            verify_rectangle(&desired)) {
            needed = minimal_count(&piece, &desired, 0);
            if (needed > 1) {
                printf("Prekryv:\n");
                if (scanf("%lf", &overlap) == 1) {
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
