#include <stdio.h>

#define MAX_ID 99999
#define MIN_ID 0
#define MAX_ACCESS_COUNT 1000000

int rangeCheck(long number,long max,long min){
    return number>=min&&number<=max;
}

long arrayUniqueCount(int array[],long from,long to){
    long count=0;
    int unique[MAX_ID+1]={0};
    for(;from<=to;from++){
        if(unique[array[from]]==0){
            unique[array[from]]=1;
            count++;
        }
    }
    return count;
}

int addLogRecord(int index,int log[],int counter[]){
    long id;
    int result;
    if((result=scanf("%ld",&id))==1){
        if(rangeCheck(id,MAX_ID,MIN_ID)){
            log[index]=id;
            counter[id]+=1;
            if(counter[id]==1){
                printf("> prvni navsteva\n");
            } else {
                printf("> navsteva #%d\n",counter[id]);
            }
        } else {
            return 0;
        }
    }
    return result;
}

int countLogRecord(int index,int log[]){
    long from,to;
    int result;
    if((result=scanf("%ld %ld",&from,&to))==2){
        if (rangeCheck(from,index-1,0)&&
            rangeCheck(to,index-1,from)){
            long sum=arrayUniqueCount(log,from,to);
            printf("> %ld / %ld\n",sum,to-from+1);
        } else {
            return 0;
        }
    }
    return result;
}

int executeOperation(char operation,int log[],int counter[],int *index){
    int result;
    int wantedResult;
    switch (operation) {
            case '+':
                result = addLogRecord(*index, log,counter);
                *index+=1;
                if(*index>MAX_ACCESS_COUNT) return 0;
                wantedResult=1;
                break;
            case '?':
                result = countLogRecord(*index,log);
                wantedResult=2;
                break;
            default:
                return 0;
    }
    if(result>0) result=result==wantedResult;
    return result;
}

int parseLog(){
    int log[MAX_ACCESS_COUNT+1];
    int counter[MAX_ID+1];
    printf("Pozadavky:\n");
    int index=0;
    int result;
    char operation;
    while((result=scanf(" %c",&operation))==1){
        if((result=executeOperation(operation,log,counter,&index))!=1){
            break;
        }
    }
    if(result == EOF){
        return 1;
    }
    return 0;
}

int main(void){
    if(!parseLog()){
        printf("Nespravny vstup.\n");
        return 1;
    }
    return 0;
}
