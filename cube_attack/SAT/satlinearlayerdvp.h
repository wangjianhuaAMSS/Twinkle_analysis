#ifndef _SATLINEARLAYERDVP_H_
#define _SATLINEARLAYERDVP_H_


int compute_increment_of_vars_Linear_layer_M_dvp(int Mtimes, Mat<int> mat) {
        int num = 0;
        int rowsuml[mat.rows()];
        mat_rowsum(mat, rowsuml);
        int colsuml[mat.cols()];
        mat_colsum(mat, colsuml);
        //xor
        for (int i = 0; i < mat.rows(); i++) {
            if (rowsuml[i] > 1)
                num += Count_XOR_Add_Dummy_Vars_DVP(rowsuml[i]) + 1;
            else {
                num += 1;
            }
        }
        //branch
        for (int i = 0; i < mat.cols(); i++) {
            num += colsuml[i];
        }
        return (Mtimes * num);
    
}



int compute_increment_of_vars_Linear_layer_P_dvp() {
    return(0); //   
}


int compute_increment_of_clauses_Linear_layer_M_dvp(int Mtimes, Mat<int> mat) {
        int num = 0;
        int colsuml[mat.cols()];
        mat_colsum(mat, colsuml);
        int rowsuml[mat.rows()];
        mat_rowsum(mat, rowsuml);
        // branch
        for (int i = 0; i < mat.cols(); i++) {
            if (colsuml[i] > 1)
                num += Count_Branch_Clauses_DVP(colsuml[i]);
            else if (colsuml[i] == 1) {
                num += 2;
            } else
                exit(1);
        }
        // xor
        for (int i = 0; i < mat.rows(); i++) {
            if (rowsuml[i] > 1)
                num += Count_XOR_Clauses_DVP(rowsuml[i]);
            else if (rowsuml[i] == 1) {
                num += 2;
            } else
                exit(1);
        }
        return (num * Mtimes);
}

int compute_increment_of_clauses_Linear_layer_P_dvp() {
    return(0);
}

void M_clauses_dvp(vector<int> input, vector<int> output, Mat<int> mat, int &BeginIndex, ofstream &fclause) {
    // branch
    Mat<int> bvar(mat.rows(), mat.cols());
    for (size_t k0 = 0; k0 < mat.cols(); k0++) {
        vector<int> vt;
        for (size_t k1 = 0; k1 < mat.rows(); k1++) {
            if (mat(k1, k0) == 1) {
                bvar(k1, k0) = BeginIndex;
                BeginIndex++;
                vt.push_back(bvar(k1, k0));
            }
        }
        if (vt.size() > 1) {
            Generate_Branch_Clauses_DVP(input[k0], vt, fclause);
        } else if (vt.size() == 1) {
            fclause << vt[0] << " -" << input[k0] << " 0" << endl;
            fclause << "-" << vt[0] << " " << input[k0] << " 0" << endl;
        }
    }
    // xor
    for (size_t k0 = 0; k0 < mat.rows(); k0++) {
        vector<int> vt;
        for (size_t k1 = 0; k1 < mat.cols(); k1++) {
            if (mat(k0, k1) == 1) {
                vt.push_back(bvar(k0, k1));
            }
        }
        if (vt.size() > 1) {
            Generate_XOR_Clauses_DVP(vt, output[k0], BeginIndex, fclause);
        } else if (vt.size() == 1) {
            fclause << vt[0] << " -" << output[k0] << " 0" << endl;
            fclause << "-" << vt[0] << " " << output[k0] << " 0" << endl;
        }
    }
}

void Linear_layer_clauses_M_dvp(vector<vector<int>> input, vector<vector<int>> &output, Mat<int> mat, int &BeginIndex, ofstream &fclause) {
    assert ((input.size() == output.size()) && (mat.rows() == input[0].size()) && (mat.rows() == mat.cols()));
    for (size_t i = 0; i < input.size(); i++) {
        Generate_Consecutive_Index(output[i], BeginIndex, input[i].size());
        M_clauses_dvp(input[i], output[i], mat, BeginIndex, fclause);
    }   
}


void Linear_layer_clauses_P_dvp(vector<vector<int>> input, vector<vector<int>> &output, vector<int> perm) {
    assert ((input.size() == output.size()) && (perm.size() == input[0].size()));
    for (int i = 0; i < input.size(); i++) {
        vector<int> tmp(input[i].size());
        for (int j = 0; j < perm.size(); j++) {
            tmp[perm[j]] = input[i][j];
        }
        output[i] = tmp;
    }  
}

#endif