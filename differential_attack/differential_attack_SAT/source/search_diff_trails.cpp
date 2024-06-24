#include "../include/search_diff_trails.h"

int main() {
    clock_t TotalTimeStart = clock();
 
    //Search all rounds diff trails
    Search_All_Rounds_Diff_Trails();



    clock_t TotalTimeEnd = clock();
    cout << "Total Runtime: "<< (double)((TotalTimeEnd - TotalTimeStart)/CLOCKS_PER_SEC) << "s" << endl;
    return 0;
}
