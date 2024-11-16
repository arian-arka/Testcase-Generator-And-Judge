#include<time.h>
#include <sys/time.h>
#include "watch.h"
struct timeval ___TIME___;

float TimeDifference()
{
    struct timeval t;
    gettimeofday(&t, 0);
    return (t.tv_sec - ___TIME___.tv_sec) * 1000.0f + (t.tv_usec - ___TIME___.tv_usec) / 1000.0f;
}
void StartStopWatch(){
    gettimeofday(&___TIME___, 0);
}