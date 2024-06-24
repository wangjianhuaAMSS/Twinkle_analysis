#ifndef _SATLINEARLAYERDIFF_H_
#define _SATLINEARLAYERDIFF_H_



int compute_increment_of_vars_Linear_layer_M_diff(int Mtimes, Mat<int> mat) {
    return(Mtimes * mat.rows()); 
}


int compute_increment_of_vars_Linear_layer_P_diff() {
    return(0); //   
}


int compute_increment_of_clauses_Linear_layer_M_diff(int Mtimes, Mat<int> mat) {//
    int num = 0;
    int rowsuml[mat.rows()];
    mat_rowsum(mat, rowsuml);
    for (int i = 0; i < mat.rows(); i++) {
        if (rowsuml[i] > 1)
            num += Count_Xor_Clauses_MinVars_Diff(rowsuml[i]);
        else if (rowsuml[i] == 1)
            num +=2;
        else
            exit(1);
    }
    return(num * Mtimes);
}

int compute_increment_of_clauses_Linear_layer_P_diff() {
    return(0);
}

void M_clauses_diff(vector<int> input, vector<int> output, Mat<int> mat, int &BeginIndex, ofstream &fclause) {
    for (size_t k0 = 0; k0 < mat.rows(); k0++) {
        vector<int> vt;
        for (size_t k1 = 0; k1 < mat.cols(); k1++) {
            if (mat(k0, k1) == 1) {
                vt.push_back(input[k1]);
            }
        }
        if (vt.size() > 1) {
            Generate_XOR_Clauses_MinVars_Diff(vt, output[k0], fclause);
        }    
        else if(vt.size() == 1) {
            fclause << vt[0] << " -" << output[k0] << " 0" << endl;
            fclause << "-" << vt[0] << " " << output[k0] << " 0" << endl;
        }
        else {
            exit(0);
        }
    }
}


void Linear_layer_clauses_M_diff(vector<vector<int>> input, vector<vector<int>> &output, Mat<int> mat, int &BeginIndex, ofstream &fclause) {
    if ((input.size() != output.size()) || (mat.rows() != input[0].size()) || (mat.rows() != mat.cols())) {
       exit(1); 
    }
    for (int i = 0; i < input.size(); i++) {
        Generate_Consecutive_Index(output[i], BeginIndex, input[i].size());
        M_clauses_diff(input[i], output[i], mat, BeginIndex, fclause);   
    }   
}


void Linear_layer_clauses_P_diff(vector<vector<int>> input, vector<vector<int>> &output, vector<int> perm) {
    if ((input.size() != output.size()) || (perm.size() != input[0].size())) {
       exit(1); 
    }
    for (int i = 0; i < input.size(); i++) {
        vector<int> tmp(input[i].size());
        for (int j = 0; j < perm.size(); j++) {
            tmp[perm[j]] = input[i][j];
        }
        output[i] = tmp;
    }  
}







#endif