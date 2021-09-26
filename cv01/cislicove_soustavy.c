#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef long int number_t;
#define DIGIT_MAX_SIZE 35
#define INPUT_MAX_SIZE 100

typedef struct {
    int size;
    int *data;
} array_t;

array_t dec_to_base(number_t dec_number, int base);

array_t base_to_base(array_t input, int source_base, int out_base);

array_t change(number_t dec_number, int base);

number_t pow_int(int base, int exponent);

int number_of_digits_needed(number_t dec_number, int base);

char* digits_to_char(array_t digits);

array_t reverse_array(array_t array);

array_t interactive();

array_t get_digits_from_string(char *char_digits, int base);

void testing_methods();

bool test_base(int base) {
    return base < DIGIT_MAX_SIZE && base > 1;
}

bool DEBUG = false;
const char *help_text = "Convert number_t [A] from base [X] to base [Y]\nUsage: baseToBase [options]... [A] [X] [Y]\n\t-d\tshow debug text\n"
                        "If A,X,Y is not supplied then program will ask for them while running\n";

int main(int argc, char **argv) {
    array_t result;
    if (argc >= 2) {
        if (argc == 2) {
            if (strcmp(argv[1], "-d") == 0) {
                DEBUG = true;
                result = interactive();
            } else if (strcmp(argv[1], "-h") == 0) {
                printf("%s", help_text);
                return EXIT_SUCCESS;
            } else {
                return EXIT_FAILURE;
            }

        } else {
            char *char_base;
            char *char_digits;
            char *char_out_base;
            if (argc == 4) {
                char_base = argv[1];
                char_digits = argv[2];
                char_out_base = argv[3];
            } else if (argc == 5) {
                if (strcmp(argv[1], "-d") == 0) {
                    DEBUG = true;
                }
                char_base = argv[2];
                char_digits = argv[3];
                char_out_base = argv[4];
            } else {
                return EXIT_FAILURE;
            }
            int base = atoi(char_base);
            int out_base = atoi(char_out_base);

            array_t digits = get_digits_from_string(char_digits, base);
            printf("Your number_t is: %s\n", digits_to_char(reverse_array(digits)));

            result = base_to_base(digits, base, out_base);
        }
    } else {
        result = interactive();
    }
    printf("Result is: %s\n", digits_to_char(result));
    return EXIT_SUCCESS;
}

/*
 * Transforms char array[char_digits] to array of numbers and checks if the number_t is valid in base[base]
 * if there is invalid digit int the array it will ask in the console for new digit until valid one is given
 *
 * returns array_t with digits
 */
array_t get_digits_from_string(char *char_digits, int base) {
    int number_of_digits = strlen(char_digits);
    if (DEBUG) {
        printf("number_of_digits(%d)\n", number_of_digits);
    }
    array_t digits = {number_of_digits, (int *) malloc(sizeof(int) * number_of_digits)};
    for (int i = number_of_digits; i > 0; i--) {
        char digit_char = char_digits[i - 1];
        int digit_number = digit_char - ((digit_char >= 'A' && digit_char <= 'Z') ? 'A' - 10 : '0');
        if (DEBUG) {
            printf("get_digits_from_string.digit_char(%c)=get_digits_from_string.digit_int(%d)\n", digit_char,
                   digit_char);
        }
        while (digit_char < '0' || (digit_char > '9' && digit_char < 'A') || digit_char > 'Z' || digit_number >= base) {
            printf("Please correct the (%d-th) digit_char from(%c):", i, digit_char);
            scanf("%c", &digit_char);
        }
        digits.data[number_of_digits - i] = digit_number;
    }
    return digits;
}

/*
 * 1) Ask for inputs using console and check if those inputs are valid if not ask again until user
 *    is polite enough to give us valid input
 * 2) use the method base_to_base:
 *      Converts any base-X number_t to base-Y
 *      using division by base and saving the remained
 *
 * returns array_t with result from base_to_base conversion
 */
array_t interactive() {
    array_t result;
    int base = 0;
    do {
        printf("Base:");
        scanf("%d", &base);
    } while (!test_base(base));

    printf("Enter the number_t:");
    char *input = (char *) malloc(sizeof(char) * INPUT_MAX_SIZE);
    scanf("%s", input);

    array_t digits = get_digits_from_string(input, base);
    printf("Your number_t is: %s\n",digits_to_char(reverse_array(digits)));

    int out_base = 0;
    do {
        printf("output base:");
        scanf("%d", &out_base);
    } while (!test_base(out_base));

    result = base_to_base(digits, base, out_base);
    return result;
}

/*
 * Converts int array to char string using ASCII 0-9 then A-Z
 * the maximum is defined as "DIGIT_MAX_SIZE"
 */
char* digits_to_char(array_t digits) {
    int size = digits.size;
    char *result = (char *) malloc(sizeof(char) * size);
    for (int i = 0; i < size; i++) {
        int digit = digits.data[i];
        if (digit > DIGIT_MAX_SIZE) {
            printf("\nError number_t should not be bigger then %d\n", DIGIT_MAX_SIZE);
        }
        // if digit < 10 then add only 48 since 0-9 ASCII codes are 48-57
        // else add 64 A-Z
        result[i] = (char) (digit + (digit > 9 ? 'A' - 10 : '0'));
        if (DEBUG) {
            printf("=print_digits.int_digit(%d)\n", digit);
        }
    }
    return result;
}


/*
 * Converting to base Y[base] from decimal number_t[dec_number]
 * using division by the biggest power of Y and going down to Y^0
 */
array_t change(number_t dec_number, int base) {
    int exponent = number_of_digits_needed(dec_number, base);
    array_t output = {exponent, (int *) malloc(sizeof(int) * exponent)};
    number_t power = pow_int(base, exponent);
    while (exponent >= 0) {
        output.data[exponent] = (int) (dec_number / power);
        dec_number %= power;
        exponent--;
        power /= base;
    }
    return reverse_array(output);
}

/*
 * returns power of the base to the exponent
 */
number_t pow_int(int base, int exponent) {
    number_t tmp = 1;
    for (number_t i = 0; i < exponent; i++) {
        tmp *= base;
    }
    return tmp;
}

/*
 * reversing array[array] in place
 * !!changes the ORIGINAL array!!
 *
 * returns reversed array
 */
array_t reverse_array(array_t array) {
    int tmp;
    int size = array.size;
    for (int i = 0; i < size / 2; i++) {
        tmp = array.data[i];
        array.data[i] = array.data[size - 1 - i];
        array.data[size - 1 - i] = tmp;
    }
    return array;
}

/*
 * returns number_t of digits needed to store base 10 number_t [dec_number] in base [base]
 */
int number_of_digits_needed(number_t dec_number, int base) {
    int needed_digits = 1;
    for (number_t power = 1; dec_number >= power; needed_digits++, power *= base) {}
    return needed_digits - 1;
}

/*
 * Converts base-10 number_t to base X
 * using division by base and saving the remained
 *
 * returns array_t with the digits of the result
 */
array_t dec_to_base(number_t dec_number, int base) {
    // Get the number_t of digits that we are going to need and allocate array
    int needed_digits = number_of_digits_needed(dec_number, base);
    array_t output = {needed_digits, (int *) malloc(sizeof(int) * needed_digits)};

    // reversed base X number_t using
    number_t tmp = 0;
    while (tmp != needed_digits) {
        int zb = (int) dec_number % base;
        output.data[tmp++] = zb;
        dec_number /= base;
    }
    if (DEBUG) {
        printf("dec_to_base.tmp(%ld) .needed_digits(%d)\n", tmp, needed_digits);
    }

    return reverse_array(output);
}

/*
 * Converts any base-X number_t to base-Y
 * using division by base and saving the remained
 *
 * this method is using dec_to_base
 *
 * returns array_t with the digits of the result
 */
array_t base_to_base(array_t input, int source_base, int out_base) {
    number_t base_ten = 0;
    int size = input.size;
    number_t power = 1;
    for (int i = 0; i < size; i++) {
        base_ten += input.data[size - 1 - i] * power;
        power *= source_base;
    }
    if (DEBUG) {
        printf("base_to_base.base_ten(%ld)\n", base_ten);
    }
    return dec_to_base(base_ten, out_base);
}

void testing_methods() {
    if (DEBUG) { // testing change function only in debug mode
        printf("\n");

        printf("%s\n", digits_to_char(change(12345, 2)));
        printf("\n");

        printf("%s\n", digits_to_char(change(1, 2)));
        printf("\n");

        printf("%s\n", digits_to_char(change(12345, 16)));
        printf("\n");
    }
    if (DEBUG) { // testing digit printing
        array_t test = {50, (int *) malloc(sizeof(int) * 50)};
        for (int i = 0; i < 50; i++) {
            test.data[i] = i;
        }
        printf("%s\n", digits_to_char(test));
    }
}