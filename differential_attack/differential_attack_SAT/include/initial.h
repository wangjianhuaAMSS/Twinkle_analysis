#ifndef _INITIAL_H_
#define _INITIAL_H_
#include "../SAT/main.h"
#include "../SAT/mat.h"
#include "../SAT/resizestate.h"
#include "../SAT/generateindex.h"
#include "../SAT/bound.h"
#include "../SAT/satxordiff.h"
#include "../SAT/matsuiconstraint.h"
#include "../SAT/satlinearlayerdiff.h"
#include "../SAT/satsboxlayerdiff.h"
#include "../SAT/sequentialencoding2.h"





#define STATELEN 1280
#define IVLEN 128
#define ROUND 19

#define MSIZE 16


//Sbox
int SboxDiffClauseIncrement;
Mat<int> SboxClauseMat;

//matrix 
int array_5_12[] = {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
                    0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
                    0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0,
                    0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0,
                    0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,
                    1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                    0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0,
                    0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0,
                    0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,
                    0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0,
                    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0,
                    0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1,
                    1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0,
                    0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
                    0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0,
                    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1
};
Mat<int> Matrix(array_5_12, 16, 16);


//permutation
vector<int> Perm1, Perm2;

vector<vector<int>> IVPerm;

//lane rotation parameters
vector<int> LaneRotationParam1 = {20,24,38,77,49,66,30,40,76,15,46,50,17,18,61,62};
vector<int> LaneRotationParam2 = {63,45,34,39,32,43,60,66,54,26,55,36,61,12,15,35};

//IV Permutation parameters
//1280
vector<vector<int>> IVPermParam = {vector<int> {1, 0},  vector<int> {3, 1}, vector<int> {5, 2}, vector<int> {7, 3}, vector<int> {11, 4}, vector<int> {13, 5},
                                vector<int> {17, 6}, vector<int> {19, 7}, vector<int> {23, 8}, vector<int> {29, 9}};



bool IVFlag = true; //true: 128-bit iv padding, false: input no restrictions

string ProblemType = "Active";
string Solver = "D:/cadical-master/build/cadical ";
//
bool WriteVarsFileFlag = true; 
ofstream ResFile;


vector<int> lane_rotation_perm(int len, vector<int> param) {
    vector<int> perm;
    assert(param.size() == 16);
    vector<int> tmp;
    vector<vector<vector<int>>> ctmp;
    vector<vector<int>> lanestmp;
    for (int i = 0; i < len; i++) tmp.push_back(i);
    ctmp = vector_2_cube<int>(tmp,  len / 16, 4, 4);
    lanestmp = xs_of_cube<int>(ctmp);
    for (int i = 0; i < 16; i++) {
        lanestmp[i].insert(lanestmp[i].begin(), lanestmp[i].begin() + (len/16 -param[i] % (len/16)), lanestmp[i].end());
        lanestmp[i].erase(lanestmp[i].begin() + len/16, lanestmp[i].end());
    }
    ctmp = cube_xs_2_cube<int>(lanestmp,  4, 4);
    perm = cube_2_vector<int>(ctmp);
    return(perm);
}

vector<int> mul_add_perm(int len, vector<int> param) {
    vector<int> perm;
    assert(param.size() == 2);

    for (int i = 0; i < len; i++) {
        perm.push_back((i * param[0] + param[1]) % len);
    }
    return(perm);
}
void Truth_Table_2_Matrix(ifstream &TruthTableFile, int NumVars, int NumRow, Mat<int> & matrix) {
    string str;
    getline(TruthTableFile, str);
    for (int i = 0; i < NumRow; i++) {
        getline(TruthTableFile, str);
        stringstream ss;
        ss << str;
        char c;
        for (int j = 0; j < NumVars; j++) {
            ss >> c;
            if (c == '0') {
                matrix(i, j) = 0;
            }
            else if (c == '1') {
                matrix(i, j) = 1;
            }
            else {
                matrix(i, j) = 9;
            }
            ss >> c;
        }
    }
}

void generate_SboxClauseMat(string min_truth_table_name, int ClauseIncrement, Mat<int> &SboxClauseMat) {
    ifstream fmtb;
    fmtb.open((min_truth_table_name + ".csv").c_str());
    Truth_Table_2_Matrix(fmtb, SboxClauseMat.cols(), ClauseIncrement, SboxClauseMat); 
}
void initial_vsbox_Diff() {
    SboxDiffClauseIncrement = 42;
    string Sbox_B9_min_truth_table = "../SAT/sbox_B9_min_truth_table_diff_active";
    Mat<int> tmp(SboxDiffClauseIncrement, 9);
    generate_SboxClauseMat(Sbox_B9_min_truth_table, SboxDiffClauseIncrement, tmp);
    SboxClauseMat = tmp;
}

void initial_paramters() {
    initial_vsbox_Diff();
    Perm1 = lane_rotation_perm(STATELEN, LaneRotationParam1);
    Perm2 = lane_rotation_perm(STATELEN, LaneRotationParam2);
}


void initial_IVPerm() {
    for (int i = 0; i < 10; i++) {
        IVPerm.push_back(mul_add_perm(IVLEN, IVPermParam[i]));
    }
}



void initial() {
    ResFile.open("res.txt", ios::app);
    
    time_t now = time(0);
    char* dt = ctime(&now);
    ResFile << endl;
    ResFile << "**************" << dt << "**************" << endl;

    initial_paramters();

    if (IVFlag)
        initial_IVPerm();
}






#endif









