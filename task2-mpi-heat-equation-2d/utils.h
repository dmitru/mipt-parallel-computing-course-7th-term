
/**
* Macro for measuring execution time of arbitrary statements
* Link: http://www.dborodytechblog.com/c-macro-measuring-time-profiling/
*/

#ifndef UTILS_H_
#define UTILS_H_

#include <cstdlib>

#define MEASURE_TIME_MS( timerVar, ...) \
  do { \
    struct timeval timerVar##_start, timerVar##_end; \
    long timerVar##_seconds, timerVar##_useconds; \
    gettimeofday( &timerVar##_start, NULL); \
    __VA_ARGS__  \
    gettimeofday( &timerVar##_end, NULL); \
    timerVar##_seconds  = timerVar##_end.tv_sec  \
                        - timerVar##_start.tv_sec; \
    timerVar##_useconds = timerVar##_end.tv_usec \
                        - timerVar##_start.tv_usec; \
    timerVar = ( timerVar##_seconds * 1000 \
               + timerVar##_useconds / 1000.0); \
  } while (0) 

#define DATA_INDEX(N, i, j, k) ( ((k) * (N) * (N)) + j * ()

struct Args {
  size_t N;
  double T;
  double alpha;
  double beta;
  const char *fileOut;
};

class Data {
public:
  Data(size_t size) 
  : a(new double[size * size]), 
    b(new double[size * size]),
    current(a),
    size(size) { }

  void flip() {
    current = (current == a)? b : a;
  }

  double *getCurrentBuffer() {
    return current;
  }

  size_t getSize() {
    return size;
  }
  
private:
  double *a;
  double *b;
  double *current;
  size_t size;
};

#endif // UTILS_H_