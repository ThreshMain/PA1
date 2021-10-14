#include <stdio.h>

typedef struct {
    int hour;
    int minute;
} time_t;

char validate_time(time_t *time);

int main(int argc, char **arhv){
    time_t x;
    time_t y;
    time_t result;
    int tmp;
    int days = 0;
    if(scanf("%d:%d %d:%d",&y.hour,&y.minute,&x.hour,&x.minute)==4){
	if(validate_time(&x) && validate_time(&y)){
	     tmp = x.minute - y.minute;
	     while(tmp < 0){
		 y.hour++;
		 tmp += 60;
	     }
	     while(tmp > 60){
		 y.hour--;
		 tmp -=60;
	     }
	     result.minute = tmp;
             tmp = x.hour - y.hour;

	     while(tmp<0){
		 tmp += 24;
		 days++;
	     }
	     while(tmp>=24){
		 tmp -= 24;
		 days--;
	     }
	     result.hour = tmp;
	     if(days){
		 printf("%d ",days);
	     }
	     printf("%02d:%02d\n",result.hour,result.minute);
	     return 0;
	}
    }
    return 1;
}

char validate_time(time_t *time){
    return !(time->hour >= 24 || time->minute >= 60 || time->minute < 0 || time->hour <0);
}
