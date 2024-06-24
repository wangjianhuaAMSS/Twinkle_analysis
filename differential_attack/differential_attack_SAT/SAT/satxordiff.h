#ifndef _SATXORDIFF_H_
#define _SATXORDIFF_H_


void Generate_XOR_Clauses_MinVars_Diff(vector<int> InputVar, int OutputVar, ofstream &fClauses) {
    if (InputVar.size() > 30) {
        cout << "Error! Please modify the code!" << endl;
        exit(1);
    }
    else if (InputVar.size() < 2) {
        cout << "Error! The number of input vars is less than 2!" << endl;
        exit(1);
    }
    int n;
    for (int i = 0; i < (1 << InputVar.size()); i++) {
        n = 0;
        for (int j = 0; j < InputVar.size(); j++) {
            if (((i >> j) & 1) == 1) {
                fClauses << "-" << InputVar[j] << " ";
                n = n ^ 1;
            }
            else {
                fClauses << InputVar[j] << " ";
            }
        }
        if (n == 1) {
            fClauses << OutputVar << " ";
        }
        else {
            fClauses << "-" << OutputVar << " ";
        }
        fClauses << "0" << endl;
    }
}



int Count_Xor_Clauses_MinVars_Diff(int NumInputVars) {
    return(1 << NumInputVars);
}

#endif