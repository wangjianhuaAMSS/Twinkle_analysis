
#include "initial.h"
#include "generate_cnf_header.h"

void R_function(vector<int> input, vector<int> &output, int len, Mat<int>sboxclausemat, Mat<int> mat, vector<int> lr1perm, vector<int> lr2perm,  int & BeginIndex, ofstream &fclause, vector<vector<int>> &path) { 
    
    vector<vector<vector<int>>> bcsboxinput, bcsboxoutput, bclr1output, bcmoutput;
    vector<vector<int>> sboxinput, sboxoutput(len /4), minput, moutput(len / 16), lr1input(1), lr1output(1), lr2input(1), lr2output(1);
    
    //Sbox layer
    bcsboxinput = vector_2_cube<int>(input, len / 16, 4, 4);
    sboxinput = zs_of_cube<int>(bcsboxinput);
    Sbox_layer_clauses_dvp(sboxinput, 4, sboxoutput, sboxclausemat, BeginIndex, fclause);
    bcsboxoutput = cube_zs_2_cube<int>(sboxoutput, len / 16, 4);
    //Linear layer   
    //LR1
    lr1input[0] = cube_2_vector<int>(bcsboxoutput);
    Linear_layer_clauses_P_dvp(lr1input, lr1output, lr1perm);
    bclr1output = vector_2_cube<int>(lr1output[0], len / 16, 4, 4);

    //M
    minput = yz_planes_of_cube<int>(bclr1output);
    Linear_layer_clauses_M_dvp(minput, moutput, mat, BeginIndex, fclause);
    bcmoutput = cube_yz_planes_2_cube<int>(moutput, 4, 4);
            
    //LR2
    lr2input[0] = cube_2_vector<int>(bcmoutput);
    Linear_layer_clauses_P_dvp(lr2input, lr2output, lr2perm);
    output.insert(output.end(), lr2output[0].begin(), lr2output[0].end());
            
    //path
    vector<int> tmp;
    path.push_back(input);
    tmp = cube_2_vector<int>(bcsboxoutput);
    path.push_back(tmp);
    path.push_back(lr1output[0]);
    path.push_back(lr2input[0]);
    path.push_back(lr2output[0]);
}



vector<int> initial_state(int &BeginIndex, string input_bitval, ofstream &fclause) {
    vector<int> vec;
    if (IVFlag) {
        vector<int> input;
        vector<int> tmpinput;
        vector<vector<vector<int>>> cinput;

        Generate_Consecutive_Index(input, BeginIndex, (int)IVLEN);
        Generate_Consecutive_Index(tmpinput, BeginIndex, (int)STATELEN);
        
        cinput = vector_2_cube<int>(tmpinput, STATELEN/16, 4, 4);

        vector<vector<int>> vbranchout(IVLEN);
        for (int i = 0; i < (int)STATELEN/IVLEN; i++) {
            for (int j = 0; j < (int)IVLEN; j++)
                vbranchout[j].push_back(cinput[i * (int)IVLEN / 16 + IVPerm[i][j] / 16][IVPerm[i][j] % 16 / 4][IVPerm[i][j] % 16 %4]);
        }
        for (int i = 0; i < (int)IVLEN; i++) {
            if (input_bitval[i] == '1') {
                fclause << input[i] << " 0" << endl;
            } else {
                fclause << "-" << input[i] << " 0" << endl;
            }  
        }
        for (int i = 0; i < (int)IVLEN; i++) {
            Generate_Branch_Clauses_DVP(input[i], vbranchout[i], fclause); 
        }
        vec = tmpinput;
    }
    else {
        Generate_Consecutive_Index(vec, BeginIndex, STATELEN);
        for (int i = 0; i < STATELEN; i++) {
            if (input_bitval[i] == '1') {
                fclause << vec[i] << " 0" << endl;
            } else {
                fclause << "-" << vec[i] << " 0" << endl;
            } 
        }
    }
    return(vec);
}





void open_problem_file(int roundnum, string input_bitval, int output_index, ofstream &fclause) {
    string str;
    string input_hexval = binary_2_hex(input_bitval);
    if (IVFlag)
        str = "../Problem-state" + to_string(STATELEN) + "/Problem-Round" +  to_string(roundnum)  + "-IV-" + input_hexval + "-" + to_string(output_index) + ".cnf"; 
    else 
        str = "../Problem-state" + to_string(STATELEN) + "/Problem-Round" +  to_string(roundnum)  + "-FULLSTATE" + "-" + to_string(output_index) + ".cnf"; 
    fclause.open(str.c_str());
}

void open_vars_file(int roundnum, string input_bitval, int output_index, ofstream &fvars) {
    string str;
    string input_hexval = binary_2_hex(input_bitval);
    if (IVFlag)
        str = "../Vars-state" + to_string(STATELEN) + "/Vars-Round" +  to_string(roundnum) + "-IV-" + input_hexval + "-" + to_string(output_index) + ".txt"; 
    else 
        str = "../Vars-state" + to_string(STATELEN) + "/Vars-Round" +  to_string(roundnum)  + "-FULLSTATE" + "-" + to_string(output_index) + ".txt"; 
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

void all_rounds_fclause(int roundnum, string input_bitval, int output_index, int &BeginIndex, ofstream &fclause) {
    ofstream fvars;
    vector<vector<int>> InputIndex(roundnum);
    vector<vector<int>> OutputIndex(roundnum);
    

    InputIndex[0] = initial_state(BeginIndex, input_bitval,  fclause);

    if (WriteVarsFileFlag) {
        open_vars_file(roundnum, input_bitval, output_index, fvars);
    }    

    for (int r = 1; r <= roundnum; r++) {
        if (r != 1) 
            InputIndex[r - 1].insert(InputIndex[r - 1].end(), OutputIndex[r - 2].begin(), OutputIndex[r - 2].end());

        vector<vector<int>> path;

        R_function(InputIndex[r - 1], OutputIndex[r - 1], (int)STATELEN, SboxClauseMat, Matrix, Perm1, Perm2, BeginIndex, fclause, path);

        
        if (WriteVarsFileFlag == 1) 
            write_vars_file(path, fvars);
    }   
    for (int i = 0; i < (int)STATELEN; i++) {
        if (i == output_index) {
            fclause << OutputIndex[roundnum - 1][i] << " 0" << endl;
        } else {
            fclause << "-" << OutputIndex[roundnum - 1][i] << " 0" << endl;
        }
    }     

    if (WriteVarsFileFlag) {
        fvars.close();
    }
}




void write_clause_file(int roundnum, string input_bitval, int output_index) {
    ofstream fclause;
    vector<int> ActiveIndex; 
    bool flag = false;
    int BeginIndex = 1;
    open_problem_file(roundnum, input_bitval, output_index, fclause);

    num_of_vars_and_clauses_fclause(roundnum, fclause);

    all_rounds_fclause(roundnum, input_bitval, output_index, BeginIndex, fclause);
    

    fclause.close();
    cout << "index: " << BeginIndex << endl;
}