
/**
* Macro for measuring execution time of arbitrary statements
* Link: http://www.dborodytechblog.com/c-macro-measuring-time-profiling/
*/

#ifndef UTILS_H_
#define UTILS_H_

#include <cstdlib>
#include <sys/time.h>

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

#define GET_X(i, N) (1.0 / N * i)
#define GET_Y(i, N) (1.0 / N * i)
#define GET_INDEX(i, j, N) ( ((i) * (N)) + (j) )

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
    next(b),
    size(size) { }

  ~Data() {
    delete a;
    delete b;
  }

  void flip() {
    if (current == a) {
      current = b;
      next = a;
    } else {
      current = a;
      next = b;
    }
  }

  double *getCurrentBuffer() {
    return current;
  }

  double *getNextBuffer() {
    return next;
  }

  size_t getSize() {
    return size;
  }
  
private:
  double *a;
  double *b;
  double *current;
  double *next;
  size_t size;
};

#endif // UTILS_H_