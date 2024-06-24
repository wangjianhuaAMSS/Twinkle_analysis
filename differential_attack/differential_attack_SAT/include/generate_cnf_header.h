
#include "initial.h"


int compute_num_of_vars(int roundnum, int ActiveNum);
int compute_num_of_clauses(int roundnum, int ActiveNum, vector<vector<int>> MatsuiRoundIndex, vector<int> MinActiveNum);

void num_of_vars_and_clauses_fclause(int roundnum, int ActiveNum, vector<vector<int>> MatsuiRoundIndex, vector<int> MinActiveNum, ofstream &fclause) {
    fclause << "p cnf " << compute_num_of_vars(roundnum, ActiveNum) <<  " " << compute_num_of_clauses(roundnum,  ActiveNum, MatsuiRoundIndex, MinActiveNum) << endl;
}



int compute_increment_of_vars_R_function(int len, Mat<int> matrix) { //s LR1 M LR2  
    int num = 0;    
    num += compute_increment_of_vars_Sbox_layer_diff(len / 4, 5) +  compute_increment_of_vars_Linear_layer_M_diff(len / int(MSIZE), matrix) + 2 * compute_increment_of_vars_Linear_layer_P_diff();
    return(num);
}

int compute_num_of_vars(int roundnum, int ActiveNum) { 
    int num = 0;
    int sboxnumperround = (int)STATELEN / 4;
    int inputvarnum;
    if (IVFlag)
        inputvarnum = (int)IVLEN;
    else 
        inputvarnum = (int)STATELEN;
    
    for (int i = 0; i < roundnum; i++)
        num += compute_increment_of_vars_R_function((int)STATELEN, Matrix);
        
     
    num += inputvarnum + Count_SequentialEncoding_Add_Dummy_Vars2(sboxnumperround * roundnum, ActiveNum);
    return(num);       
}






int compute_increment_of_clauses_R_function(int len, int sbox_diff_clause_increment, Mat<int> matrix) { //s LR1 M LR2 
    int num = 0;
    num += compute_increment_of_clauses_Sbox_layer_diff(len / 4, sbox_diff_clause_increment) + compute_increment_of_clauses_Linear_layer_M_diff(len / int(MSIZE), matrix) + 2 * compute_increment_of_clauses_Linear_layer_P_diff();
    return(num);
}

int compute_num_of_clauses(int roundnum, int ActiveNum, vector<vector<int>> MatsuiRoundIndex, vector<int> MinActiveNum) {
    int ClausesNum = 0;
    int sboxnumperround = int(STATELEN) / 4;
    int inputconstrclausesnum;
    if (IVFlag)
        inputconstrclausesnum = 1;
    else {
        inputconstrclausesnum = 1;
    }
    
    for (int i = 0; i < (int)MatsuiRoundIndex.size(); i++) {
        ClausesNum += Count_MatsuiConstraint_ge_Clauses(sboxnumperround * roundnum, ActiveNum, MinActiveNum[MatsuiRoundIndex[i][1]], MatsuiRoundIndex[i][0] * sboxnumperround, MatsuiRoundIndex[i][1] * sboxnumperround - 1);
    }    
    ClausesNum += inputconstrclausesnum + Count_SequentialEncoding_Clauses2(sboxnumperround * roundnum, ActiveNum);
    for (int i = 0; i < roundnum; i++)
        ClausesNum += compute_increment_of_clauses_R_function((int)STATELEN, SboxDiffClauseIncrement, Matrix);
    
    
    return(ClausesNum);
}





