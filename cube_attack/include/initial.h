#ifndef _INITIAL_H_
#define _INITIAL_H_

#include "../SAT/main.h"
#include "../SAT/mat.h"
#include "../SAT/resizestate.h"
#include "../SAT/generateindex.h"
#include "../SAT/sequentialencoding2.h"
#include "../SAT/bound.h"
#include "../SAT/satxordvp.h"
#include "../SAT/satbranchdvp.h"
#include "../SAT/matsuiconstraint.h"
#include "../SAT/satlinearlayerdvp.h"
#include "../SAT/satsboxlayerdvp.h"



#define STATELEN 1280
#define IVLEN 128//
#define ROUND 9 //

#define MSIZE 16



//Sbox
int SboxDVPClauseIncrement;
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


vector<int> LaneRotationParam1 = {20,24,38,77,49,66,30,40,76,15,46,50,17,18,61,62};
vector<int> LaneRotationParam2 = {63,45,34,39,32,43,60,66,54,26,55,36,61,12,15,35};
                                          

//IV Permutation parameters
vector<vector<int>> IVPermParam = {vector<int> {1, 0},  vector<int> {3, 1}, vector<int> {5, 2}, vector<int> {7, 3}, vector<int> {11, 4}, vector<int> {13, 5},
                                vector<int> {17, 6}, vector<int> {19, 7}, vector<int> {23, 8}, vector<int> {29, 9}};



bool IVFlag = true; //true: 128-bit iv padding, false: input no restrictions



string ProblemType = "Active";
string Solver = "D:/cadical-master/build/cadical ";
//
bool WriteVarsFileFlag = true; 
ofstream ResFile;

string input_bitvar, input_hexvar;
string binary_2_hex(string binstr);



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

void initial_vsbox_DVP() {
    SboxDVPClauseIncrement = 38;
    string Sbox_B9_min_truth_table = "../SAT/sbox_B9_min_truth_table_dvp";
    Mat<int> tmp(SboxDVPClauseIncrement, 8);
    generate_SboxClauseMat(Sbox_B9_min_truth_table, SboxDVPClauseIncrement, tmp);
    SboxClauseMat = tmp;
}

void initial_paramters() {
    initial_vsbox_DVP();
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
    
    time_t now = time(0);// 基于当前系统的当前日期/时间
    char* dt = ctime(&now);// 把 now 转换为字符串形式
    ResFile << endl;
    ResFile << "**************" << dt << "**************" << endl;

    if (!IVFlag) {
        for (int i = 0; i < STATELEN; i++) {
            input_bitvar+='1';
        }
        
    } else {
        for (int i = 0; i < IVLEN; i++) {
            input_bitvar+='1';
        }
            
    }
    input_hexvar = binary_2_hex(input_bitvar);
    initial_paramters();
    if (IVFlag)
        initial_IVPerm();
}


string binary_2_hex(string binstr) {
    string hexstr;
    for (int i = 0; i < (int)binstr.size() / 16; i++) {
        stringstream ss;
        string tmps;
        ss << hex << setw(4) << setfill('0') << stoi(binstr.substr(i * 16, 16), nullptr, 2);
        ss >> tmps;
        hexstr += tmps;
    }
    if (((int)binstr.size() % 16) != 0) {
        stringstream ss;
        string tmps;
        ss << hex << setw(4) << setfill('0') << stoi(binstr.substr(((int)binstr.size() / 16 - 1) * 16, (int)binstr.size() % 16), nullptr, 2);
        ss >> tmps;
        hexstr += tmps;
    }
    
    return(hexstr);
}



#endif









