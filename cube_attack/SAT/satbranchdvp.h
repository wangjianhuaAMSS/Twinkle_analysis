#ifndef _SATBRANCHDVP_H_
#define _SATBRANCHDVP_H_


void Generate_Branch_Clauses_DVP(int InputVar, vector<int> OutputVar, ofstream &fClauses) {
    fClauses << "-" << InputVar << " "; 
    for (size_t i = 0; i < OutputVar.size(); i++) {
        fClauses << OutputVar[i] << " ";
    }
    fClauses << "0" << endl;
    for (size_t i = 0; i < OutputVar.size(); i++) {
        fClauses << InputVar << " ";
        fClauses << "-" << OutputVar[i] << " ";
        fClauses << "0" << endl;
    }
}

int Count_Branch_Clauses_DVP(int NumOutputVars) {
    return(1 + NumOutputVars);
}


#endif