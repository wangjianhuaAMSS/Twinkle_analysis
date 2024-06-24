#ifndef _SATSBOXLAYERDIFF_H_
#define _SATSBOXLAYERDIFF_H_


int compute_increment_of_vars_Sbox_layer_diff(int sboxnum, int persboxincrement) {
    return(sboxnum * persboxincrement); 
}

int compute_increment_of_clauses_Sbox_layer_diff(int sboxnum, int persboxclauses) {
    return(sboxnum * persboxclauses); 
}


void Generate_Clauses_Based_on_Truth_Table_Matrix(vector<int> Var, Mat<int> SboxMatrix, ofstream &fClauses) { // Var = [InputVar, OutputVar, W]
    assert(SboxMatrix.cols() == Var.size());
    for (size_t i = 0; i < SboxMatrix.rows(); i++) {
        
        for (size_t j = 0; j < Var.size(); j++) {
            if (SboxMatrix(i, j) == 0) {
                fClauses << Var[j] << " ";
            }
            else if (SboxMatrix(i, j) == 1) {
                fClauses << "-" << Var[j] << " ";
            }
            
        }
        fClauses << "0" << endl;
    }
}

void Sbox_layer_clauses_diff(vector<vector<int>> input, int lenoutput, vector<vector<int>> &output, Mat<int> SboxClauseMat, int lenactvar, vector<vector<int>> &ActiveVar, int &BeginIndex, ofstream &fclause) {
    assert ((input.size() == output.size()) && (input.size() == ActiveVar.size()) && (SboxClauseMat.cols() == input[0].size() + lenoutput + lenactvar));
    for (int i = 0; i < input.size(); i++) {
        Generate_Consecutive_Index(output[i], BeginIndex, lenoutput);
        Generate_Consecutive_Index(ActiveVar[i], BeginIndex, lenactvar);
        vector<int> vt;
        vt.insert(vt.end(), input[i].begin(), input[i].end());
        vt.insert(vt.end(), output[i].begin(), output[i].end());
        vt.insert(vt.end(), ActiveVar[i].begin(), ActiveVar[i].end());
        Generate_Clauses_Based_on_Truth_Table_Matrix(vt, SboxClauseMat, fclause);
    }
}

#endif