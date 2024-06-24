
#include "initial.h"
#include "generate_cnf_header.h"





void R_function(vector<int> input, vector<int> &output, int len, Mat<int>sboxclausemat, Mat<int> mat, vector<int> lr1perm, vector<int> lr2perm, vector<int> & ActiveIndex, int & BeginIndex, ofstream &fclause, vector<vector<int>> &path) { 
    
    vector<vector<vector<int>>> bcsboxinput, bcsboxoutput, bclr1output, bcmoutput;
    vector<vector<int>> sboxinput, sboxoutput(len /4), minput, moutput(len / 16), lr1input(1), lr1output(1), lr2input(1), lr2output(1), ActiveVar(len /4);
    
    //Sbox layer
    bcsboxinput = vector_2_cube<int>(input,  len / 16, 4, 4);
    sboxinput = zs_of_cube<int>(bcsboxinput);
    Sbox_layer_clauses_diff(sboxinput, 4, sboxoutput, sboxclausemat, 3, ActiveVar, BeginIndex, fclause); //1->3
    bcsboxoutput = cube_zs_2_cube<int>(sboxoutput, len / 16,  4);
    vector<int> actvar = plane_2_vector<int>(ActiveVar);
    for (int i = 0; i < (int)ActiveVar.size(); i++) {
        for (int j = 0; j < W_N[0]; j++)
            ActiveIndex.push_back(ActiveVar[i][0]);
        for (int j = 0; j < W_N[1]; j++)
            ActiveIndex.push_back(ActiveVar[i][1]); 
        for (int j = 0; j < W_N[2]; j++)
            ActiveIndex.push_back(ActiveVar[i][2]); 
    }
    
    //LR1   
    lr1input[0] = cube_2_vector<int>(bcsboxoutput);
    Linear_layer_clauses_P_diff(lr1input, lr1output, lr1perm);
    bclr1output = vector_2_cube<int>(lr1output[0], len / 16, 4, 4);

    //M
    minput = yz_planes_of_cube<int>(bclr1output);
    Linear_layer_clauses_M_diff(minput, moutput, mat, BeginIndex, fclause);
    bcmoutput = cube_yz_planes_2_cube<int>(moutput, 4, 4);
            
    //LR2
    lr2input[0] =cube_2_vector<int>(bcmoutput);
    Linear_layer_clauses_P_diff(lr2input, lr2output, lr2perm);
    output.insert(output.end(), lr2output[0].begin(), lr2output[0].end());
            
    //path
    path.push_back(input);
    path.push_back(lr1input[0]);
    path.push_back(actvar);
    path.push_back(lr1output[0]);
    path.push_back(lr2input[0]);
    path.push_back(lr2output[0]);

}

vector<int> initial_state(int &BeginIndex, ofstream &fclause) {
    vector<int> vec;
    if (IVFlag) {
        vector<int> input;
        vector<int> tmpinput(STATELEN);
        vector<vector<vector<int>>> cinput;

        cinput = vector_2_cube<int>(tmpinput, STATELEN/16,  4, 4);

        Generate_Consecutive_Index(input, BeginIndex, (int)IVLEN);

        for (int i = 0; i < (int)STATELEN/IVLEN; i++) {
            for (int j = 0; j < (int)IVLEN; j++)
                cinput[i * (int)IVLEN / 16 + IVPerm[i][j] / 16][IVPerm[i][j] % 16 / 4][IVPerm[i][j] % 16 % 4] = input[j];
        
        }
        for (int i = 0; i < (int)IVLEN; i++) {
            fclause << input[i] << " ";
        }
        fclause << "0" << endl;
        

        vec = cube_2_vector<int>(cinput);
    }
    else {
        Generate_Consecutive_Index(vec, BeginIndex, STATELEN);
        for (int i = 0; i < STATELEN; i++) {
            fclause << vec[i] << " ";
        }
        fclause << "0" << endl;
    }
    return(vec);
}




void open_problem_file(int roundnum, int ActiveNum, ofstream &fclause) {
    string str = "../Problem-state" + to_string(STATELEN) + "/Problem-Round" +  to_string(roundnum)  + "-" + ProblemType + to_string(ActiveNum) + ".cnf"; 
    fclause.open(str.c_str());
}

void open_vars_file(int roundnum, ofstream &fvars) {
    string str;
    str = "../Vars-state" + to_string(STATELEN) + "/Vars-Round" +  to_string(roundnum)  + "-" + ProblemType + ".txt"; 
    fvars.open(str.c_str());
}


void write_vars_file(vector<vector<int>> path, ofstream &fvars) {
    for (size_t i = 0; i < path.size(); i++) {
        for (size_t j = 0; j < path[i].size(); j++) {
            fvars << path[i][j] << " ";
        }
        fvars << endl;
    }  
}

void all_rounds_fclause_phase(int roundnum, vector<int> &ActiveIndex,  int &BeginIndex, ofstream &fclause) {
    ofstream fvars;
    vector<vector<int>> InputIndex(roundnum);
    vector<vector<int>> OutputIndex(roundnum);
    

    InputIndex[0] = initial_state(BeginIndex, fclause);

    if (WriteVarsFileFlag) {
        open_vars_file(roundnum, fvars);
    }    

    for (int r = 1; r <= roundnum; r++) {
        if (r != 1) 
            InputIndex[r - 1].insert(InputIndex[r - 1].end(), OutputIndex[r - 2].begin(), OutputIndex[r - 2].end());

        vector<vector<int>> path;
        
        R_function(InputIndex[r - 1], OutputIndex[r - 1], (int)STATELEN, SboxClauseMat, Matrix, Perm1, Perm2, ActiveIndex, BeginIndex, fclause, path);
        if (WriteVarsFileFlag == 1) 
            write_vars_file(path, fvars);
    }        

    if (WriteVarsFileFlag) {
        fvars.close();
    }
}



void sequentialencoding_and_matsui_fclause(int roundnum, vector<vector<int>> MatsuiRoundIndex, vector<int> ActiveIndex, int ActiveNum, vector<int> MinActiveNum, int &BeginIndex, ofstream &fclause) {
    vector<vector<int>> s;

    Generate_SequentialEncoding_Clauses2(ActiveIndex, ActiveNum, s, BeginIndex, fclause);

    
    for (int i = 0; i < (int)MatsuiRoundIndex.size(); i++) { 
        Generate_MatsuiConstraint_ge_Clauses(ActiveIndex, s, MinActiveNum[MatsuiRoundIndex[i][1]], MatsuiRoundIndex[i][0] * ((int)STATELEN / 4) * (W_N[0] + W_N[1] + W_N[2]), MatsuiRoundIndex[i][1] * ((int)STATELEN / 4) * (W_N[0] + W_N[1] + W_N[2]) - 1, fclause);
    } 
}



void write_clause_file(int roundnum, vector<vector<int>> MatsuiRoundIndex, int ActiveNum, vector<int> MinActiveNum) {
    ofstream fclause;
    vector<int> ActiveIndex; 
    bool flag = false;
    int BeginIndex = 1;
    open_problem_file(roundnum, ActiveNum, fclause);
    num_of_vars_and_clauses_fclause_phase(roundnum, ActiveNum, MatsuiRoundIndex, MinActiveNum, fclause);

    all_rounds_fclause_phase(roundnum, ActiveIndex, BeginIndex, fclause);

    sequentialencoding_and_matsui_fclause(roundnum, MatsuiRoundIndex, ActiveIndex, ActiveNum, MinActiveNum, BeginIndex, fclause); 
    fclause.close();
    cout << "index: " << BeginIndex << endl;
}