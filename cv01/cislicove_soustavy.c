#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

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

char *digits_to_char(array_t digits);

array_t reverse_array(array_t array);

array_t interactive();

array_t get_digits_from_string(char *char_digits, int base);

void testing_methods();

int digit_to_int(char digit_char);

int *split_to_prime_factors(int number);

array_t prime_factors_to_array(int *factors, int size);

int chunk_split_size(int base_a, int base_b);

bool test_base(int base) {
    return base < DIGIT_MAX_SIZE && base > 1;
}

bool DEBUG = false;
const char *help_text = "Convert number_t [A] from base [X] to base [Y]\nUsage: baseToBase [options]... [A]... [X] [Y]\n\t-d\tshow debug text\n"
                        "If A,X,Y is not supplied then program will ask for them while running\n";

int main(int argc, char **argv) {
    array_t result;

    char *numbers[3];
    int number_index = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[1], "-d") == 0) {
            DEBUG = true;
        } else if (strcmp(argv[1], "-h") == 0) {
            printf("%s", help_text);
            return EXIT_SUCCESS;
        } else {
            if (number_index == 3) {
                printf("%s", help_text);
                return EXIT_FAILURE;
            }
            numbers[number_index++] = argv[i];
        }
    }
    if (number_index == 0) {
        result = interactive();
    } else if (number_index == 3 || number_index == 2) {
        if (number_index == 2) {
            numbers[2] = numbers[1];
            numbers[1] = numbers[0];
            numbers[0] = "10";
        }
        int base = atoi(numbers[0]);
        int out_base = atoi(numbers[2]);

        array_t digits = get_digits_from_string(numbers[1], base);

        char *your_number_string = digits_to_char(reverse_array(digits));
        if (your_number_string == NULL) {
            printf("%s", help_text);
            return EXIT_FAILURE;
        }
        printf("Your number_t is: %s\n", your_number_string);
        free(your_number_string);

        result = base_to_base(digits, base, out_base);
        free(digits.data);
    } else {
        printf("%s", help_text);
        return EXIT_FAILURE;
    }
    char *result_string = digits_to_char(result);
    if (result_string == NULL) {
        printf("%s", help_text);
        return EXIT_FAILURE;
    }
    printf("Result is: %s\n", result_string);
    free(result_string);
    free(result.data);
    return EXIT_SUCCESS;
}

/**
 * Transforms char array[char_digits] to array_t of numbers and checks if the number is valid in #base
 * if there is invalid digit int the array it will ask in the console for new digit until valid one is given
 *
 * @param char_digits '\0' ended string
 * @param base number smaller then DIGIT_MAX_SIZE and bigger then 0
 * @return array_t with digits
 */
array_t get_digits_from_string(char *char_digits, int base) {
    int number_of_digits = strlen(char_digits);
    if (DEBUG) {
        printf("number_of_digits(%d)\n", number_of_digits);
    }
    array_t digits = {number_of_digits, (int *) malloc(sizeof(int) * number_of_digits)};
    for (int i = number_of_digits; i > 0; i--) {
        char digit_char = char_digits[i - 1];
        if (DEBUG) {
            printf("get_digits_from_string.digit_char(%c)=get_digits_from_string.digit_int(%d)\n", digit_char,
                   digit_char);
        }
        int digit_number = digit_to_int(digit_char);
        while (digit_number >= base || digit_number < 0) {
            printf("Please correct the (%d-th) digit_char from(%c):", i, digit_char);
            scanf(" %c", &digit_char);
            digit_number = digit_to_int(digit_char);
        }
        digits.data[number_of_digits - i] = digit_number;
    }
    return digits;
}

int digit_to_int(char digit_char) {
    return digit_char - ((digit_char >= 'A' && digit_char <= 'Z') ? 'A' - 10 : '0');
}

/**
 * 1) Ask for inputs using console and check if those inputs are valid if not ask again until user
 *    is polite enough to give valid input
 * 2) use the method base_to_base on the input
 * @return array_t with result from base_to_base conversion
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

    char *result_string = digits_to_char(reverse_array(digits));
    printf("Your number_t is: %s\n", result_string);
    free(result_string);

    int out_base = 0;
    do {
        printf("output base:");
        scanf("%d", &out_base);
    } while (!test_base(out_base));

    result = base_to_base(digits, base, out_base);
    free(input);
    free(digits.data);
    return result;
}

/**
 * Converts int array to char string using ASCII 0-9 then A-Z
 * the maximum is defined as "DIGIT_MAX_SIZE"
 *
 * @param digits
 * @return string ended with '\0'
 */
char *digits_to_char(array_t digits) {
    int size = digits.size;
    char *result = (char *) malloc(sizeof(char) * (size + 1));
    for (int i = 0; i < size; i++) {
        int digit = digits.data[i];
        if (digit > DIGIT_MAX_SIZE) {
            printf("\nError number_t should not be bigger then %d\n", DIGIT_MAX_SIZE);
            free(result);
            return NULL;
        }
        // if digit < 10 then add only 48 since 0-9 ASCII codes are 48-57
        // else add 64 A-Z
        result[i] = (char) (digit + (digit > 9 ? 'A' - 10 : '0'));
        if (DEBUG) {
            printf("=print_digits.int_digit(%d)\n", digit);
        }
    }
    result[size] = 0;
    return result;
}

/**
 * Converting to base Y[base] from decimal number_t[dec_number]
 * using division by the biggest power of Y and going down to Y^0
 *
 * @param dec_number
 * @param base
 * @return array_t with result digits
 */
array_t change(number_t dec_number, int base) {
    int exponent = number_of_digits_needed(dec_number, base);
    array_t output = {exponent, (int *) malloc(sizeof(int) * exponent)};
    exponent--;
    number_t power = pow_int(base, exponent);
    while (exponent >= 0) {
        output.data[exponent] = (int) (dec_number / power);
        dec_number %= power;
        exponent--;
        power /= base;
    }
    return reverse_array(output);
}


/**
 * @param base
 * @param exponent
 * @return power of the base to the exponent
 */
number_t pow_int(int base, int exponent) {
    number_t tmp = 1;
    for (number_t i = 0; i < exponent; i++) {
        tmp *= base;
    }
    return tmp;
}

/**
 * reversing array[array] in place
 * !!changes the ORIGINAL array!!
 * @param array
 * @return reversed array_t
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

/**
 * Calculates number of digits need to store base 10 number_t [dec_number] in base [base]
 *
 * @param dec_number
 * @param base
 * @return number of digits needed
 */
int number_of_digits_needed(number_t dec_number, int base) {
    int needed_digits = 1;
    for (number_t power = 1; dec_number >= power; needed_digits++, power *= base) {}
    return needed_digits - 1;
}

/**
 * Converts base-10 dec_number to base X[base]
 * using division by base and saving the remained
 *
 * @param dec_number
 * @param base
 * @return array_t with the digits of the result
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

/**
 * Converts any source_base input to out_base
 * using division by base and saving the remained
 * @param input
 * @param source_base
 * @param out_base
 * @return array_t with the digits of the result
 */
array_t base_to_base(array_t input, int source_base, int out_base) {
    int size = input.size;
    int chunk_size = chunk_split_size(source_base, out_base);
    if (chunk_size > 0) {
        array_t result = {size * chunk_size, malloc(sizeof(int) * size * chunk_size)};
        int index = 0;
        for (int i = 0; i < size; i++) {
            int digit = input.data[size - 1 - i];
            array_t out_base_chunk = reverse_array(dec_to_base(digit, out_base));
            if (DEBUG) {
                printf("%d=%s,%d\n", digit, digits_to_char(out_base_chunk), out_base_chunk.size);
            }
            for (int j = 0; j < chunk_size; j++) {
                if (j < out_base_chunk.size) {
                    result.data[result.size - 1 - index++] = out_base_chunk.data[j];
                } else {
                    result.data[result.size - 1 - index++] = 0;
                }
                if (DEBUG) {
                    printf("%s   %d  %d   %d \n", digits_to_char(result), j, j < out_base_chunk.size,
                           out_base_chunk.data[j]);
                }
            }
            free(out_base_chunk.data);
        }
        return result;
    }

    number_t base_ten = 0;
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

/**
 * Calculates the highest chunk split size that can be used to divide higher number into smaller parts
 * in order to change their base from base_a to base_b
 *
 * @param base_a bigger number
 * @param base_b smaller number
 * @returns chunk size or -1 if there is no split possible
 */
int chunk_split_size(int base_a, int base_b) {
    int *primes_a = split_to_prime_factors(base_a);
    int prime_a = -1;
    int power_a = 0;
    for (int i = 0; i < base_a - 1; i++) {
        if (primes_a[i]) {
            if (prime_a == -1) {
                prime_a = i + 2;
                power_a = primes_a[i];
            } else {
                free(primes_a);
                return -1;
            }
        }
    }

    int *primes_b = split_to_prime_factors(base_b);
    int prime_b = -1;
    int power_b = 0;
    for (int i = 0; i < base_b - 1; i++) {
        if (primes_b[i]) {
            if (prime_b == -1 && i + 2 == prime_a) {
                prime_b = i + 2;
                power_b = primes_b[i];
            } else {
                free(primes_a);
                free(primes_b);
                return -1;
            }
        }
    }
    free(primes_a);
    free(primes_b);
    if (power_a > power_b && power_a % power_b == 0) {
        return power_a / power_b;
    }
    return -1;
}

/**
 * Split number into prime factors which when multiplied should give back the same number
 *
 * @param number number bigger then 0 that should be prime factorized
 * @return returns array of primes where index is the **prime number - 2** and value is the power,
 * size of this array is source **number - 1**
 */
int *split_to_prime_factors(int number) {
    unsigned long size = sizeof(int) * (number - 1);
    int *primes = malloc(size);
    memset(primes, 0, size);
    int i = 2;
    while (i <= number) {
        if (number % i == 0) {
            primes[i - 2]++;
            number /= i;
        } else {
            i++;
        }
    }
    return primes;
}

/**
 * Converts array where index is the number nad value is number of times that the number
 * should be inside
 *
 * @param factors int array with size of size
 * @param size  size of factors array
 * @return array_t ready to be used by method digits_to_chars
 */
array_t prime_factors_to_array(int *factors, int size) {
    int number_of_primes = 0;
    for(int i=0;i<size;i++){
        if(factors[i]!=0){
            number_of_primes+=factors[i];
        }
    }
    array_t result = {number_of_primes, malloc(sizeof(int) * number_of_primes)};
    int index = 0;
    for (int i = 0; i < size; i++) {
        for (; factors[i]; factors[i]--) {
            result.data[index++] = i + 2;
        }
    }
    return result;
}

void testing_methods() {
    array_t digits;
    char *string_result;

    digits = change(12345, 2);
    string_result = digits_to_char(digits);
    assert(strcmp("11000000111001", string_result) == 0);
    free(digits.data);
    free(string_result);

    digits = change(12345, 4);
    string_result = digits_to_char(digits);
    assert(strcmp("3000321",string_result)==0);
    free(digits.data);
    free(string_result);

    digits = change(12345, 8);
    string_result = digits_to_char(digits);
    assert(strcmp("30071",string_result)==0);
    free(digits.data);
    free(string_result);

    digits = change(45, 2);
    string_result = digits_to_char(digits);
    assert(strcmp("101101",string_result)==0);
    free(digits.data);
    free(string_result);
}