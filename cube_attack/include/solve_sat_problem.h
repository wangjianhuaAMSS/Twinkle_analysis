
#include "initial.h"

void call_sat_solver(int roundnum, string input_bitval, int output_index) {
    string str, solstr;
    string input_hexval = binary_2_hex(input_bitval);
    if (IVFlag) {
        str = "../Problem-state" + to_string(STATELEN) + "/Problem-Round" +  to_string(roundnum)  + "-IV-" + input_hexval + "-" + to_string(output_index) + ".cnf"; 
        solstr ="../solution-state" + to_string(STATELEN) + "/Round" + to_string(roundnum) + "-IV-" + input_hexval + "-" + to_string(output_index) + "-solution.out";
    }   
    else {
        str = "../Problem-state" + to_string(STATELEN) + "/Problem-Round" +  to_string(roundnum)  + "-FULLSTATE" + "-" + to_string(output_index) + ".cnf"; 
        solstr ="../solution-state" + to_string(STATELEN) + "/Round" + to_string(roundnum) + "-FULLSTATE" + "-" + to_string(output_index) + "-solution.out";
    } 
    string order = Solver + str + " > " + solstr;
    system(order.c_str());
}

bool Check_If_Satisfiable(int roundnum, string input_bitval, int output_index) {
    ifstream file;
    string Solution_File_Name;
    string input_hexval = binary_2_hex(input_bitval);
    if (IVFlag) {
        Solution_File_Name ="../solution-state" + to_string(STATELEN) + "/Round" + to_string(roundnum) + "-IV-" + input_hexval + "-" + to_string(output_index) + "-solution.out";
    }   
    else {
        Solution_File_Name ="../solution-state" + to_string(STATELEN) + "/Round" + to_string(roundnum)  + "-FULLSTATE" + "-" + to_string(output_index) + "-solution.out";
    }
    
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

bool slove_sat_problem(int roundnum, string input_bitval, int output_index) {
    clock_t time_start = clock();
    
    //solve
    call_sat_solver(roundnum, input_bitval, output_index);
    //analyse solution
    bool flag = false;
    flag = Check_If_Satisfiable(roundnum, input_bitval, output_index);
    
    string input_hexval = binary_2_hex(input_bitval);
    clock_t time_end = clock();

    if (flag == true) {
        cout << "Round:" << to_string(roundnum) << "; " + input_hexval + ": " << "; Sat; TotalCost: " << (double)((time_end - time_start)/CLOCKS_PER_SEC) << "s" << endl;
    }
    else
        cout << "Round:" << to_string(roundnum) << "; " + input_hexval + ": "  << "; Unsat; TotalCost: " << (double)((time_end - time_start)/CLOCKS_PER_SEC) << "s" << endl;
    
    return(flag);
}