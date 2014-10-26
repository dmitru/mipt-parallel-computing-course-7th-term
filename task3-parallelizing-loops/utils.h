
#include <stdlib.h>
#include <sys/time.h>

#define MEASURE_TIME_MS( timerVar, ...) \
    do { \
        struct timeval timerVar##_start, timerVar##_end; \
        long timerVar##_seconds, timerVar##_useconds; \
        gettimeofday( &timerVar##_start, NULL); \
        __VA_ARGS__ \
        gettimeofday( &timerVar##_end, NULL); \
        timerVar##_seconds = timerVar##_end.tv_sec \
        - timerVar##_start.tv_sec; \
        timerVar##_useconds = timerVar##_end.tv_usec \
        - timerVar##_start.tv_usec; \
        timerVar = ( timerVar##_seconds * 1000 \
                     + timerVar##_useconds / 1000.0); \
    } while (0) 
