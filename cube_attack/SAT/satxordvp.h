#ifndef _SATXORDVP_H_
#define _SATXORDVP_H_



void Generate_XOR_Clauses_DVP(vector<int> InputVar, int OutputVar, int &BeginIndex, ofstream &fClauses) {
    vector<vector<int>> s;
    Generate_SequentialEncoding_Clauses2(InputVar, 1, s, BeginIndex, fClauses);
    fClauses << "-" << OutputVar << " " << s[InputVar.size() - 2][0] << " " << InputVar[InputVar.size() - 1] << " 0" << endl;
    fClauses << OutputVar << " -" << s[InputVar.size() - 2][0] << " 0" << endl;
    fClauses << OutputVar << " -" << InputVar[InputVar.size() - 1] << " 0" << endl;
}

void Generate_XOR_Clauses_DVP_Output_Known(vector<int> InputVar, int OutputVal, int &BeginIndex, ofstream &fClauses) {

    if (OutputVal == 0) {
        for (size_t i = 0; i < InputVar.size(); i++) {
            fClauses << "-" << InputVar[i] << " 0" << endl;
        }
    } else {
        vector<vector<int>> s;
        Generate_SequentialEncoding_Clauses2(InputVar, 1, s, BeginIndex, fClauses);
        fClauses << s[InputVar.size() - 2][0] << " " << InputVar[InputVar.size() - 1] << " 0" << endl;
    }
}

int Count_XOR_Clauses_DVP(int NumInputVars) {
    return(4 * NumInputVars - 2);
}

int Count_XOR_Clauses_DVP_Output_Known(int NumInputVars, int OutputVal) {
    if (OutputVal == 0) {
        return(NumInputVars);
    } else {
        return(4 * NumInputVars - 4);
    }   
}

int Count_XOR_Add_Dummy_Vars_DVP(int NumInputVars) {
    return(NumInputVars - 1);
}

int Count_XOR_Add_Dummy_Vars_DVP_Output_Known(int NumInputVars, int OutputVal) {
    if (OutputVal == 0) {
        return(0);
    } else {
        return(NumInputVars - 1);
    }  
}


#endif