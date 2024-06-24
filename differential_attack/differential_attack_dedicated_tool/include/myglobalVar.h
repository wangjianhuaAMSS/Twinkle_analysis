#ifndef _MYGLOBALVAL_H_
#define _MYGLOBALVAL_H_


#include <iostream>
#include <stdio.h>
#include <fstream>
#include <algorithm> 
#include <chrono> 
#include <stdlib.h>
#include <malloc.h>
#include <functional>
#include <future>
#include <queue>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <utility>
#include <vector>
using namespace std;

extern std::mutex mtx;

extern vector<int> MHis2R;
extern vector<int> MHis3RBack;
extern vector<int> MHis3RFor;
extern vector<int> MHis4RBack;
extern vector<int> MHis4RFor;
extern vector<vector<int>> MHis;

extern double MINR1, MINR2, MINR3BACK, MINR3FOR, MINR4BACK, MINR4FOR;








extern ofstream Mf2R, Mf3RBack, Mf3RFor, Mf4RBack, Mf4RFor;
extern ofstream foutHisto4;
extern ofstream foutHisto3;
extern ofstream foutHisto2;
extern ofstream foutHisto2Det;


#endif