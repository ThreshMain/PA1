#include <stdio.h>
#include <stdlib.h>
int* dec_to_base(int,int);
int* base_to_base(int*,int,int);
void change(int,int,int *);
int pow_int(int,int);

int main(int argv,char **args){
    int *result;
    int *digits;
    printf("Base:");
    int base;
    scanf("%d",&base);

    printf("Number of digits:");
    int number_of_digits;
    scanf("%d",&number_of_digits);
    digits = (int*) malloc(sizeof(int) * (number_of_digits+1));
    digits[0] = number_of_digits;
    digits = (digits + 1);

    for(int i = number_of_digits;i>0;i--){
	int digit = base;
	while (digit >= base){
	    printf("Enter the (%d-th) digit:",i);
	    scanf("%d",&digit);
	}
        digits[number_of_digits - i] = digit;	
    }
    printf("output base:");
    int outbase;
    scanf("%d",&outbase);

    result = base_to_base(digits,base,outbase);
    int size = result[-1];
    for(int i = 0; i <= size; i++){
       printf("%d%c",result[i],(i!=size && outbase!=10)*'-');
    }
}
/* 
 * slower and stupid way to do this 
 * but it's my way so psst :D
 */
void change(int input,int base,int* output){
    int power = 20;
    while(power != 0){
	output[power] = input / pow_int(base,power);
	input %= (pow_int(base,power));
	power --;
    }
}
/*
 * returns power of the base to the exponent
 */
int pow_int(int base,int exponent){
    int tmp = base;
    for(int i = 0; i < exponent; i++){
	tmp *= base;
    }
    return tmp;
}
/*
 * Converts base-10 number to base X
 *
 * returns pointer to int array where the -1-th element is the size
 */
int* dec_to_base(int input,int base){
    int *save = (int*) malloc(sizeof(int) * 20);
    int tmp = 0;
    while(input != 0){
       int zb = input % base;
       save[tmp++] = zb;
       input = input / base;
    }
    tmp--;
    int *output = (int*) malloc(sizeof(int) * (tmp+1));
    for(int i = tmp; i >= 0; i--){
	output[1+tmp-i] = save[i];
    }
    free(save);
    output[0] = tmp;
    return (output+1);
}
/*
 * Converts any base-X number to base-Y
 * 
 * this method is using dec_to_base
 *
 * returns pointer to int array where the -1-th element is the size
 */
int* base_to_base(int *input,int sourcebase,int outbase){
    int tmp = 0;
    int size = input[-1];
    int power = 1;
    for(int i = 1; i <= size; i++){
	tmp += input[size-i] * power;
	power *= sourcebase;
    }
    return dec_to_base(tmp,outbase);
}
