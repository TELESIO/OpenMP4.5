#ifndef OPENCALTIME
#define OPENCALTIME

#include <sys/time.h>
#include <stdio.h>

struct Time{

    struct timeval tmStart;

    struct timeval tmEnd;
};

void startTime(struct Time * time){
    gettimeofday(&time->tmStart, NULL);
}

void endTime(struct Time * time){
    gettimeofday(&time->tmEnd, NULL);
    unsigned long long seconds =(time->tmEnd.tv_sec - time->tmStart.tv_sec) ;
    unsigned long long milliseconds = (time->tmEnd.tv_usec - time->tmStart.tv_usec) / 1000;
    unsigned long long totalMilliseconds =1000*seconds + milliseconds;
    int totalSeconds =(int)totalMilliseconds/1000;
    int totalMinutes =(int)totalSeconds/60;
    totalSeconds =(int)totalSeconds%60;
    int totalMilliseconds2 =(int)totalMilliseconds%1000;
    printf("%d:%d.%d;",totalMinutes,totalSeconds,totalMilliseconds2);
    printf("%llu;",totalMilliseconds);
    printf("%llu\n",seconds);

}


#endif
