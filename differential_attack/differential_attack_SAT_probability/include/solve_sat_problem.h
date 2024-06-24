
#include "initial.h"

void call_sat_solver(int roundnum, int ActiveNum) {
    string str = "../Problem-state" + to_string(STATELEN) + "/Problem-Round" +  to_string(roundnum) + "-" + ProblemType + to_string(ActiveNum) + ".cnf"; 
    string order = Solver + str + " > ../solution-state" + to_string(STATELEN) + "/Round" + to_string(roundnum) +  "-" + ProblemType + to_string(ActiveNum) + "-solution.out";
    system(order.c_str());
}

bool Check_If_Satisfiable(int roundnum, int ActiveNum) {
    ifstream file;
    string Solution_File_Name = "../solution-state" + to_string(STATELEN) + "/Round" + to_string(roundnum) + "-" + ProblemType + to_string(ActiveNum) + "-solution.out";
    
    file.open(Solution_File_Name.c_str());

    string str, strS, strUS;
    strS = "s SATISFIABLE";
    strUS = "s UNSATISFIABLE";
    while(getline(file, str)) {
        if (str.compare(strS) == 0) {
            return(true);
        }
        if (str.compare(strUS) == 0) {
            return(false);
        }
    }
    cout << "Could not solve the problem!" << endl;
    exit(-1);
}

bool slove_sat_problem(int roundnum, int ActiveNum) {
    clock_t time_start = clock();
    
    //solve
    call_sat_solver(roundnum, ActiveNum);
    //analyse solution
    bool flag = false;
    flag = Check_If_Satisfiable(roundnum, ActiveNum);
    
    clock_t time_end = clock();

    if (flag == true) {
        cout << "Round:" << to_string(roundnum) << "; " + ProblemType + ": " << ActiveNum << "; Sat; TotalCost: " << (double)((time_end - time_start)/CLOCKS_PER_SEC) << "s" << endl;
    }
    else
        cout << "Round:" << to_string(roundnum) << "; " + ProblemType + ": " << ActiveNum << "; Unsat; TotalCost: " << (double)((time_end - time_start)/CLOCKS_PER_SEC) << "s" << endl;
    
    return(flag);
}