
#include "initial.h"


int compute_num_of_vars(int roundnum);
int compute_num_of_clauses(int roundnum);

void num_of_vars_and_clauses_fclause(int roundnum, ofstream &fclause) {
    fclause << "p cnf " << compute_num_of_vars(roundnum) <<  " " << compute_num_of_clauses(roundnum) << endl;
}


int compute_increment_of_vars_R_function(int len, Mat<int> matrix) { //s M P 
    int num = 0;    
    num += compute_increment_of_vars_Sbox_layer_dvp(len / 4, 4) +  compute_increment_of_vars_Linear_layer_M_dvp(len / int(MSIZE), matrix) + 2 * compute_increment_of_vars_Linear_layer_P_dvp();
    return(num);
}

int compute_num_of_vars(int roundnum) { 
    int num = 0;
    int inputvarnum;
    if (IVFlag)
        inputvarnum = (int)IVLEN + (int)STATELEN;
    else 
        inputvarnum = (int)STATELEN;
    
    for (int i = 0; i <= roundnum - 1; i++)
        num += compute_increment_of_vars_R_function((int)STATELEN, Matrix);
    num += inputvarnum;
    return(num);       
}




int compute_increment_of_clauses_R_function(int len, int sbox_dvp_clause_increment, Mat<int> matrix) { //s M P
    int num = 0;
    num += compute_increment_of_clauses_Sbox_layer_dvp(len / 4, sbox_dvp_clause_increment) + compute_increment_of_clauses_Linear_layer_M_dvp(len / int(MSIZE), matrix) + 2 * compute_increment_of_clauses_Linear_layer_P_dvp();
    return(num);
}

int compute_num_of_clauses(int roundnum) {
    int ClausesNum = 0;
    int inputconstrclausesnum;
    if (IVFlag)
        inputconstrclausesnum = (int)IVLEN + Count_Branch_Clauses_DVP((int)STATELEN / IVLEN) * IVLEN;
    else {
        inputconstrclausesnum = (int)STATELEN;
    }
        
    ClausesNum += inputconstrclausesnum + (int)STATELEN;
    for (int i = 0; i <= roundnum - 1; i++)
        ClausesNum += compute_increment_of_clauses_R_function((int)STATELEN, SboxDVPClauseIncrement, Matrix);
    
    return(ClausesNum);
}




