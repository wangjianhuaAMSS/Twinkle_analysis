
#include "initial.h"
#include "generate_cnf_clause.h"
#include "solve_sat_problem.h"




void Search_All_Rounds_DVP_Trails() {
    initial();
    
    int unsol_n1, sol_n2;
    int maxround;   

    ResFile << "test var string: " << endl;
    ResFile << input_hexvar << endl;
    
    for (int i = 0; i < (int)STATELEN; i++) {
        int output_index = i;
        ResFile << "output index: " << i << endl;
        cout << "output index: " << i << endl;
        for (int r = 5; r <= (int)5; r++) {
            cout << "Round" << r << ":" << endl;
            WriteVarsFileFlag = true;

            write_clause_file(r, input_bitvar, output_index);
            bool sol = slove_sat_problem(r, input_bitvar, output_index);
            if (sol) {
                ResFile << "Round " << r << ": sat;" << endl;  
            }
            else {
                ResFile << "Round " << r << ": unsat;" << endl;
                cout << i << " !!!!" << endl;
                exit(2);
            }
        }
    }
}
