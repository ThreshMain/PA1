#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

typedef struct {
    double width;
    double height;
} rectangle_t;

/**
 * Compares 2 doubles with accounting for double inaccuracy
 * @param x double
 * @param y double
 * @return 0 or 1 based on if 2 numbers are the same
 */
int equals(double x, double y) {
    return fabs(x - y) < (DBL_EPSILON * fabs(x + y));
}

/**
 * Rounds up the number to the closes integer
 * that is bigger or equal to the param
 * @param number double
 * @return rounded number
 */
int custom_round(double number) {
    int int_number = (int) number;
    if (equals(int_number, number)) {
        return int_number;
    }
    return int_number + 1;
}

/**
 * Compares 2 numbers with accounting for double inaccuracy
 * and returns if first[number] is bigger then the second one[bigger_then].
 * @param number double
 * @param bigger_then double
 * @return 1 if the number is bigger otherwise 0
 */
int bigger_then(double number, double bigger_then) {
    return number > bigger_then && !equals(number, bigger_then);
}

/**
 * Compares 2 numbers with accounting for double inaccuracy
 * and returns if first[number] is smaller or equal then the second one[smaller_then].
 * @param number double
 * @param smaller_then double
 * @return 1 if the number is smaller or equal otherwise 0
 */
int smaller_or_equal(double number, double smaller_then) {
    return number < smaller_then || equals(number, smaller_then);
}

/**
 * Calculate the number of rectangles needed to cover the area of desired rectangle
 * with accounting for the overlap of rectangles if needed.
 * @param piece piece of rectangle to be user for covering the area
 * @param desired rectangle size of desired area
 * @param overlap how much should 2 pieces overlap while covering the area
 * @return number of rectangles needed or -1 if overlap is too big
 */
int count_needed(rectangle_t piece, rectangle_t desired, double overlap) {
    int width = 1;
    int height = 1;
    desired.width -= piece.width;
    desired.height -= piece.height;

    piece.width -= overlap;
    piece.height -= overlap;
    if (bigger_then(desired.width, 0) || bigger_then(desired.height, 0)) {
        if (bigger_then(desired.width, 0)) {
            if (smaller_or_equal(piece.width,0)) {
                return -1;
            } else {
                width += custom_round(desired.width / piece.width);
            }
        }
        if (bigger_then(desired.height, 0)) {
            if (smaller_or_equal(piece.height,0)) {
                return -1;
            } else {
                height += custom_round(desired.height / piece.height);
            }
        }
        return height * width;
    }
    return 1;
}

/**
 * Rotates rectangle by 90°
 * this will change the original rectangle
 * @param rectangle to be rotated
 */
void rotate_rectangle(rectangle_t *rectangle) {
    double tmp = rectangle->height;
    rectangle->height = rectangle->width;
    rectangle->width = tmp;
}

/**
 * Calculates the minimal number of rectangles needed to cover an area by rotating rectangle
 * @param piece rectangle will be rotated and changed
 * @param desired rectangle area that should be covered
 * @param overlap how much should 2 pieces overlap while covering the area
 * @return number of rectangles needed or -1 if overlap is too big
 */
int minimal_count(rectangle_t *piece, rectangle_t *desired, double overlap) {
    int count = count_needed(*piece, *desired, overlap);
    rotate_rectangle(piece);
    int rotated_count = count_needed(*piece, *desired, overlap);
    return count < rotated_count && count > 0 ? count : rotated_count;
}

/**
 * Verifies if rectangle dimensions are reasonable
 * @param rectangle
 * @return 1 if rectangle is valid otherwise 0
 */
int verify_rectangle(rectangle_t *rectangle) {
    return bigger_then(rectangle->height, 0) && bigger_then(rectangle->width, 0);
}

/**
 * Loads rectangle by printing the message to console and scanning for user input separated by white space
 * @param message to be printed to the user or null if non message should be shown
 * @param result pointer to rectangle to which the result will be written to
 * @return EXIT_SUCCESS if loaded successfully otherwise EXIT_FAILURE
 */
int load_rectangle(char *message,rectangle_t *result) {
    if (message != NULL) {
        printf("%s", message);
    }
    if (scanf("%lf %lf", &result->width, &result->height) == 2 &&
        verify_rectangle(result)) {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int main(int argc, char **argv) {
    rectangle_t piece, desired;
    double overlap;
    int needed;
    if (load_rectangle("Velikost latky:\n",&piece) == EXIT_SUCCESS &&
        load_rectangle("Velikost plachty:\n",&desired) == EXIT_SUCCESS) {
        needed = minimal_count(&piece, &desired, 0);
        if (needed > 1) {
            printf("Prekryv:\n");
            if (scanf("%lf", &overlap) == 1 && overlap >= 0) {
                needed = minimal_count(&piece, &desired, overlap);
                if (needed != -1) {
                    printf("Pocet kusu latky: %d\n", needed);
                    return EXIT_SUCCESS;
                } else {
                    printf("Nelze vyrobit.\n");
                    return EXIT_FAILURE;
                }
            }
        } else {
            printf("Pocet kusu latky: %d\n", needed);
            return EXIT_SUCCESS;
        }
    }
    printf("Nespravny vstup.\n");
    return EXIT_FAILURE;
}