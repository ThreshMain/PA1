#include <stdio.h>

int main(int argc, char **argv){
    int x,y;
    if(scanf("%d %d",&x,&y)==2){
	printf("%d + %d = %d\n",x,y,x+y);
	printf("%d - %d = %d\n",x,y,x-y);
	printf("%d * %d = %d\n",x,y,x*y);
	if(y){
		printf("%d / %d = %d\n",x,y,x/y);
		printf("%d %% %d = %d\n",x,y,x%y);
	}
	return 0;
    }
    return 1;
}
