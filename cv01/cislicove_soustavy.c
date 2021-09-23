#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef long int number;
#define digit_max_limit 35
#define input_string_size 20

typedef struct {
    int size;
    int *data;
} Array;

Array dec_to_base(number dec_number, int base);

Array base_to_base(Array input, int source_base, int out_base);

Array change(number dec_number, int base);

number pow_int(int base, int exponent);

int number_of_digits_needed(number dec_number, int base);

void print_digits(Array digits);

Array reverse_array(Array array);

Array interactive();

Array get_digits_from_string(char *char_digits, int base);

void testingMethods();

bool test_base(int base) {
    return base < digit_max_limit && base > 1;
}

bool DEBUG = false;
const char *help_text = "Convert number [A] from base [X] to base [Y]\nUsage: baseToBase [options]... [A] [X] [Y]\n\t-d\tshow debug text\n"
                        "If A,X,Y is not supplied then program will ask for them while running\n";

int main(int argv, char **args) {
    struct Array result;
    if (argv >= 2) {
        if (argv == 2) {
            if (strcmp(args[1], "-d") == 0) {
                DEBUG = true;
                result = interactive();
            } else if (strcmp(args[1], "-h") == 0) {
                printf("%s", help_text);
                return EXIT_SUCCESS;
            } else {
                return EXIT_FAILURE;
            }

        } else {
            char *char_base;
            char *char_digits;
            char *char_out_base;
            if (argv == 4) {
                char_base = args[1];
                char_digits = args[2];
                char_out_base = args[3];
            } else if (argv == 5) {
                if (strcmp(args[1], "-d") == 0) {
                    DEBUG = true;
                }
                char_base = args[2];
                char_digits = args[3];
                char_out_base = args[4];
            } else {
                return EXIT_FAILURE;
            }
            int base = atoi(char_base);
            int out_base = atoi(char_out_base);

            Array digits = get_digits_from_string(char_digits, base);
            printf("Your number is:");
            print_digits(reverse_array(digits));
            printf("\n");

            result = base_to_base(digits, base, out_base);
        }
    } else {
        result = interactive();
    }
    printf("Result is:");
    print_digits(result);
    printf("\n");
    return EXIT_SUCCESS;
}

/*
 * Transforms char array[char_digits] to array of numbers and checks if the number is valid in base[base]
 * if there is invalid digit int the array it will ask in the console for new digit until valid one is given
 *
 * returns Array with digits
 */
Array get_digits_from_string(char *char_digits, int base) {
    int number_of_digits = strlen(char_digits);
    if (DEBUG) {
        printf("number_of_digits(%d)\n", number_of_digits);
    }
    Array digits = {number_of_digits, (int *) malloc(sizeof(int) * number_of_digits)};
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
 *      Converts any base-X number to base-Y
 *      using division by base and saving the remained
 *
 * returns Array with result from base_to_base conversion
 */
Array interactive() {
    Array result;
    int base = 0;
    do {
        printf("Base:");
        scanf("%d", &base);
    } while (!test_base(base));

    printf("Enter the number:");
    char *input = (char *) malloc(sizeof(char) * input_string_size);
    scanf("%s", input);

    Array digits = get_digits_from_string(input, base);
    printf("Your number is:");
    print_digits(reverse_array(digits));
    printf("\n");
    if (DEBUG) {
        printf("\n");
        print_digits(digits);
        printf("\n");
    }

    int out_base = 0;
    do {
        printf("output base:");
        scanf("%d", &out_base);
    } while (!test_base(out_base));

    result = base_to_base(digits, base, out_base);
    return result;
}

/*
 * Prints digits using ASCII 0-9 then A-Z
 * the maximum is defined as "digit_max_limit"
 */
void print_digits(Array digits) {
    int size = digits.size;
    for (int i = 0; i < size; i++) {
        int digit = digits.data[i];
        if (digit > digit_max_limit) {
            printf("\nError number should not be bigger then %d\n", digit_max_limit);
        }
        // if digit < 10 then add only 48 since 0-9 ASCII codes are 48-57
        // else add 64 A-Z
        printf("%c", (char) digit + (digit > 9 ? 'A' - 10 : '0'));
        if (DEBUG) {
            printf("=print_digits.int_digit(%d)\n", digit);
        }
    }
}


/* 
 * Converting to base Y[base] from decimal number[dec_number]
 * using division by the biggest power of Y and going down to Y^0
 */
Array change(number dec_number, int base) {
    int exponent = number_of_digits_needed(dec_number, base);
    Array output = {exponent, (int *) malloc(sizeof(int) * exponent)};
    number power = pow_int(base, exponent);
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
number pow_int(int base, int exponent) {
    number tmp = 1;
    for (number i = 0; i < exponent; i++) {
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
Array reverse_array(Array array) {
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
 * returns number of digits needed to store base 10 number [dec_number] in base [base]
 */
int number_of_digits_needed(number dec_number, int base) {
    int needed_digits = 1;
    for (number power = 1; dec_number >= power; needed_digits++, power *= base) {}
    return needed_digits - 1;
}

/*
 * Converts base-10 number to base X
 * using division by base and saving the remained
 *
 * returns Array with the digits of the result
 */
Array dec_to_base(number dec_number, int base) {
    // Get the number of digits that we are going to need and allocate array
    int needed_digits = number_of_digits_needed(dec_number, base);
    Array output = {needed_digits, (int *) malloc(sizeof(int) * needed_digits)};

    // reversed base X number using
    number tmp = 0;
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
 * Converts any base-X number to base-Y
 * using division by base and saving the remained
 * 
 * this method is using dec_to_base
 *
 * returns Array with the digits of the result
 */
Array base_to_base(Array input, int source_base, int out_base) {
    number base_ten = 0;
    int size = input.size;
    number power = 1;
    for (int i = 0; i < size; i++) {
        base_ten += input.data[size - 1 - i] * power;
        power *= source_base;
    }
    if (DEBUG) {
        printf("base_to_base.base_ten(%ld)\n", base_ten);
    }
    return dec_to_base(base_ten, out_base);
}

void testingMethods() {
    if (DEBUG) { // testing change function only in debug mode
        printf("\n");

        print_digits(change(

                12345, 2));
        printf("\n");

        print_digits(change(

                1, 2));
        printf("\n");

        print_digits(change(

                12345, 16));
    }
    if (DEBUG) { // testing digit printing
        Array test = {50, (int *) malloc(sizeof(int) * 50)};
        for (
                int i = 0;
                i < 50; i++) {
            test.data[i] =
                    i;
        }
        print_digits(test);
    }
}