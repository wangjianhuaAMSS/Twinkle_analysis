#ifndef _MYASSIGNTASKS_H_
#define _MYASSIGNTASKS_H_
#include <unistd.h>
#include "myThreadPool.h"
#include "myTrailSeedIterator.h"
#include "myglobalVar.h"

int thread_f(const TrailSeedIterator & T);
void assignTasks(int threadNum, int maxWeightTwoRound, int maxWeightFourRound, WEIGHTTYPE HB, WEIGHTTYPE LB);

#endif