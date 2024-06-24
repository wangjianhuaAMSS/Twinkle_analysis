
#include "initial.h"
#include "generate_cnf_clause.h"
#include "solve_sat_problem.h"



void generate_Matsui_bound(vector<vector<int>> &MatsuiRoundIndex, int r){

    vector<int> mv(2);
    mv[0] = 0;
    for (int j = 1; j <= r; j++) {
        mv[1] = j;
        MatsuiRoundIndex.push_back(mv);
    }
}


void Search_All_Rounds_Diff_Trails() {
    initial();
    int ActiveNum;

    vector<int> MinActiveNum(20); 
    MinActiveNum[0] = 0; 


    int unsol_n1, sol_n2;


    for (int r = 1; r <= (int)ROUND; r++) {
        vector<vector<int>> MatsuiRoundIndex;
        cout << "Round" << r << ":" << endl;
        generate_Matsui_bound(MatsuiRoundIndex, r);

        unsol_n1 = MinActiveNum[r - 1] - 1;
        
        sol_n2 = 1000000; //choose a max number
        ActiveNum = unsol_n1;
        WriteVarsFileFlag = true;
        while (if_compact_bounds(unsol_n1, sol_n2) == false) {
            ActiveNum = checkbound_update(unsol_n1, sol_n2, 25);
            //ActiveNum++;
            MinActiveNum[r] = unsol_n1 + 1;
            write_clause_file(r, MatsuiRoundIndex, ActiveNum, MinActiveNum);
            bool sol = slove_sat_problem(r, ActiveNum);
            WriteVarsFileFlag = false;

            if (sol == false) {
                unsol_n1 = ActiveNum;
            }
            else {
                sol_n2 = ActiveNum;
            }
            if (unsol_n1 > STATELEN)
                break;
        }
        MinActiveNum[r] = sol_n2;
        ResFile << "Round " << r << ": minimum probability weight: " << sol_n2/5 << endl;
        
    }
    for (int i = 1; i <= (int)ROUND; i++) 
        cout << MinActiveNum[i]/5 << " ";

    cout << endl;
}
