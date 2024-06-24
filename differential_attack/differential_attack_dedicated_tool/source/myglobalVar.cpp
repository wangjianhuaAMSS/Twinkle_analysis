#include "../include/myglobalVar.h"

vector<int> MHis2R(100);
vector<int> MHis3RBack(100);
vector<int> MHis3RFor(100);
vector<int> MHis4RBack(100);
vector<int> MHis4RFor(100);
vector<vector<int>> MHis(40,vector<int>(40));

double MINR1 =255;
double MINR2 = 255;
double MINR3BACK = 255;
double MINR3FOR =255;
double MINR4BACK = 255;
double MINR4FOR = 255;









ofstream Mf2R, Mf3RBack, Mf3RFor, Mf4RBack, Mf4RFor;
 /* A file of the histogram of required 4-round trail cores. */
ofstream foutHisto4;
/* A file of the histogram of required 3-round trail cores. */
ofstream foutHisto3;
/* A file of the histogram of all 2-round trail cores up to the given certain weight. */
ofstream foutHisto2;
/* A file of the histogram of all 2-round trail cores up to the given certain weight in details. */
ofstream foutHisto2Det;

std::mutex mtx;