#include <stdio.h>

int main(int argc,char **args){
    printf("%s",*(args+1));
    return 0;
}

