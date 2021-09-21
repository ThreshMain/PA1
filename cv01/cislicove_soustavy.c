#include <stdio.h>
#include <stdlib.h>

#define DEBUG 0
#define number long int
#define digit_max_limit 35

struct Array dec_to_base(number, number);

struct Array base_to_base(struct Array, number, number);

struct Array change(number, number);

number pow_int(number, number);

number number_of_digits_needed(number dec_number, number base);

void print_digits(struct Array digits);

struct Array reverse_array(struct Array array);

struct Array {
    number size;
    number *data;
};

int main(int argv, char **args) {
    struct Array result;
    number base = 0;
    do {
        printf("Base:");
        scanf("%ld", &base);
    } while (base >= digit_max_limit || base < 1);

    number number_of_digits = 0;
    do {
        printf("Number of digits:");
        scanf("%ld", &number_of_digits);
    } while (number_of_digits <= 0);

    printf("Enter the number:");
    char *input = (char *) malloc(sizeof(char) * number_of_digits);
    scanf("%s", input);

    struct Array digits = {number_of_digits, malloc(sizeof(number) * number_of_digits)};

    for (number i = number_of_digits; i > 0; i--) {
        char digit = input[i - 1];
        if(DEBUG){
            printf("%c=%d\n",digit,digit);
        }
        while (digit < '0' || (digit > '9' && digit < 'A') || digit > 'Z') {
            printf("Please correct the (%ld-th) digit from(%c):", i,digit);
            scanf("%c", &digit);
        }
        digits.data[number_of_digits - i] = digit - ((digit >= 'A' && digit <= 'Z') ? 'A' - 10 : '0');
    }
    printf("Your number is:");
    print_digits(reverse_array(digits));
    printf("\n");
    if (DEBUG) {
        printf("\n");
        print_digits(digits);
        printf("\n");
    }

    number outbase=0;
    do {
        printf("output base:");
        scanf("%ld", &outbase);
    } while (outbase >= digit_max_limit || outbase < 1);

    result = base_to_base(digits, base, outbase);
    print_digits(result);
    if (DEBUG) { // testing change function only in debug mode
        printf("\n");
        print_digits(change(12345, 2));
        printf("\n");
        print_digits(change(1, 2));
        printf("\n");
        print_digits(change(12345, 16));
    }
    if (DEBUG) { // testing digit printing
        struct Array test = {50, (number *) malloc(sizeof(number) * 50)};
        for (int i = 0; i < 50; i++) {
            test.data[i] = i;
        }
        print_digits(test);
    }
    return 0;

}

/*
 * Prints digits using ASCII 0-9 then A-Z
 */
void print_digits(struct Array digits) {
    number size = digits.size;
    for (number i = 0; i < size; i++) {
        number digit = digits.data[i];
        if (digit > digit_max_limit) {
            printf("\nError number should not be bigger then %d\n", digit_max_limit);
        }
        // if digit < 10 then add only 48 since 0-9 ASCII codes are 48-57
        // else add 64 A-Z
        printf("%c", digit + (digit > 9 ? 'A' - 10 : '0'));
        if (DEBUG) {
            printf("=%d\n", digit);
        }
    }
}


/* 
 * Converting to base Y[base] from decimal number[dec_number]
 * using division by the biggest power of Y and going down to Y^0
 */
struct Array change(number dec_number, number base) {
    number exponent = number_of_digits_needed(dec_number, base);
    struct Array output = {exponent, (number *) malloc(sizeof(number) * exponent)};
    number power = pow_int(base, exponent);
    while (exponent >= 0) {
        output.data[exponent] = dec_number / power;
        dec_number %= power;
        exponent--;
        power /= base;
    }
    return reverse_array(output);
}
/*
 * returns power of the base to the exponent
 */
number pow_int(number base, number exponent) {
    number tmp = 1;
    for (number i = 0; i < exponent; i++) {
        tmp *= base;
    }
    return tmp;
}

/*
 * reversing array[array] in place
 */
struct Array reverse_array(struct Array array) {
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
number number_of_digits_needed(number dec_number, number base) {
    number needed_digits = 1;
    for (number power = 1; dec_number >= power; needed_digits++, power *= base) {}
    return needed_digits - 1;
}

/*
 * Converts base-10 number to base X
 */
struct Array dec_to_base(number dec_number, number base) {
    // Get the number of digits that we are going to need and alcate array
    number needed_digits = number_of_digits_needed(dec_number, base);
    struct Array output = {needed_digits, (number *) malloc(sizeof(number) * needed_digits)};

    // reversed base X number using
    number tmp = 0;
    while (tmp != needed_digits) {
        number zb = dec_number % base;
        output.data[tmp++] = zb;
        dec_number /= base;
    }
    if (DEBUG) {
        printf("dec_to_base.tmp(%ld) .needed_digits(%ld)\n", tmp, needed_digits);
    }

    return reverse_array(output);
}

/*
 * Converts any base-X number to base-Y
 * using division by base and saving the remained
 * 
 * this method is using dec_to_base
 *
 * returns pointer to number array where the -1-th element is the size
 */
struct Array base_to_base(struct Array input, number sourcebase, number outbase) {
    number base_ten = 0;
    number size = input.size;
    number power = 1;
    for (number i = 0; i < size; i++) {
        base_ten += input.data[size - 1 - i] * power;
        power *= sourcebase;
    }
    if (DEBUG) {
        printf("base_to_base.base_ten(%ld)\n", base_ten);
    }
    return dec_to_base(base_ten, outbase);
}
