//#pragma once
#ifndef _TWINKLE_H_
#define _TWINKLE_H_
#include <iostream>
#include <vector>
#include <bitset> 
#include <map>
#include <cmath>

using namespace std;
#define TWINKLESIZE 1280
#define PLANESIZE 16
#define LANESIZE 80

typedef double WEIGHTTYPE;

using namespace std;
typedef struct twoDimCd {
    uint8_t x;
    uint8_t z;
} twoDimCd;

typedef struct planePoints {
    vector<uint8_t> xs;
    uint8_t z;
} planePoints;

class Twinkle {
public:
	static const unsigned int Size = TWINKLESIZE;

    //Sbox information LAT
    
                                          //input/output           0|   1|   2|   4|   8|   3|   5|   6|   9|   a|   c|   7|   b|   d|   e|   f| 
    map<uint8_t, vector<uint8_t>> SboxDDTI = {{0,   vector<uint8_t>{16,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
                                            {1,   vector<uint8_t>{0,   0,   4,   0,   0,   4,   0,   4,   0,   4,   8,   4,   4,   8,   4,   4} },
                                            {2,   vector<uint8_t>{0,   0,   0,   0,   0,   8,   8,   0,   8,   0,   0,   0,   0,   0,   0,   8} },
                                            {4,   vector<uint8_t>{0,   0,   0,   0,   4,   0,   0,   8,   4,   4,   4,   8,   4,   4,   4,   4} },
                                            {8,   vector<uint8_t>{0,   0,   0,   0,   0,   0,   8,   0,   0,   0,   0,   8,   0,   8,   0,   8} },
                                            {3,   vector<uint8_t>{0,   0,   4,   8,   0,   4,   0,   4,   8,   4,   0,   4,   4,   0,   4,   4} },
                                            {5,   vector<uint8_t>{0,   0,   4,   0,   4,   4,   0,   4,   4,   8,   4,   4,   8,   4,   0,   0} },
                                            {6,   vector<uint8_t>{0,   0,   0,   8,   4,   8,   0,   0,   4,   4,   4,   0,   4,   4,   4,   4} },
                                            {9,   vector<uint8_t>{0,   8,   4,   0,   8,   4,   0,   4,   0,   4,   0,   4,   4,   0,   4,   4} },
                                            {0xa,   vector<uint8_t>{0,   0,   8,   0,   8,   0,   0,   8,   0,   0,   8,   0,   0,   0,   0,   0} },
                                            {0xc,   vector<uint8_t>{0,   8,   8,   0,   4,   0,   0,   0,   4,   4,   4,   0,   4,   4,   4,   4} },
                                            {0x7,   vector<uint8_t>{0,   0,   4,   0,   4,   4,   8,   4,   4,   0,   4,   4,   0,   4,   8,   0} },
                                            {0xb,   vector<uint8_t>{0,   8,   4,   8,   0,   4,   0,   4,   0,   4,   0,   4,   4,   0,   4,   4} },
                                            {0xd,   vector<uint8_t>{0,   0,   4,   0,   4,   4,   8,   4,   4,   0,   4,   4,   0,   4,   8,   0} },
                                            {0xe,   vector<uint8_t>{0,   8,   0,   8,   4,   0,   0,   0,   4,   4,   4,   0,   4,   4,   4,   4} },
                                            {0xf,   vector<uint8_t>{0,   0,   4,   0,   4,   4,   0,   4,   4,   8,   4,   4,   8,   4,   0,   0} }}; //B9
    map<uint8_t, vector<uint8_t>> SboxDDT;
    vector<uint8_t> outputIndex = { 0x0, 0x1, 0x2, 0x4, 0x8, 0x3, 0x5, 0x6, 0x9, 0xa, 0xc, 0x7, 0xb, 0xd, 0xe, 0xf};
    map<uint8_t, WEIGHTTYPE> SboxDiffIMin;
    map<uint8_t, WEIGHTTYPE> SboxDiffOMin;
    map<uint8_t, WEIGHTTYPE> SboxDiffILowerBound;
    map<uint8_t, WEIGHTTYPE> SboxDiffOLowerBound;
    map<uint8_t, map<uint8_t, WEIGHTTYPE>> SboxDiffIT;
    map<uint8_t, map<uint8_t, WEIGHTTYPE>> SboxDiffOT;

    map<uint8_t, map<uint8_t, WEIGHTTYPE>> SboxDiffIfOvCLowerBound;

    map<uint8_t, map<uint8_t, WEIGHTTYPE>> SboxDiffOfIvCLowerBound;

    WEIGHTTYPE MinDiff;




    vector<int> LROffset1I{20, 24, 38, 77, 49, 66, 30, 40, 76, 15, 46, 50, 17, 18, 61, 62};
    vector<int> LROffset1;


    vector<int> LROffset2I{63, 45, 34, 39, 32, 43, 60, 66, 54, 26, 55, 36, 61, 12, 15, 35};
	vector<int> LROffset2;

    vector<unsigned int> NewMatOffsetI{0, 3, 4};
    vector<unsigned int> NewMatInverseOffsetI{0, 4, 7, 8, 10, 12, 13, 14, 15};
    vector<unsigned int> NewMatOffset;
    vector<unsigned int> NewMatInverseOffset;
    int Pi = 7;
    int PiInverse = 7;
    int NewMatFixedL = 0;
    int NewMatFixedHd = 4; //
    int NewMatInverseFixedL = 12;
    int NewMatInverseFixedHd = 0; //
public:
    void InitParam();
    void InitSboxDiffParam();
    

    void NewMat(bitset<Size> &arr, twoDimCd aP);

    void NewMatPiInverseLR2(bitset<Size> &arr, twoDimCd aP);

    void NewMatPiInverseLR2Fixed(bitset<Size> &arr, bitset<Size> fixed, twoDimCd aP, bool op);

    void PiInverseLR1Inverse(bitset<Size> &arr, twoDimCd aP);
    void PiInverseLR1InverseFixed(bitset<Size>& arr, twoDimCd aP, bool op);


    void NewMatInversePiInverseLR1Inverse(bitset<Size> &arr, twoDimCd aP);
    void PiInverseLR2(bitset<Size>& arr, twoDimCd aP);
    void NewMatInversePiInverseLR1InverseFixed(bitset<Size> &arr, bitset<Size> fixed, twoDimCd aP, bool op);
    void PiInverseLR2Fixed(bitset<Size>& arr, twoDimCd aP, bool op);

    void SboxInverseFixedBack(bitset<Size> &arr, bitset<Size> outputDiff, bitset<Size/4> &incomp, int &incomn);
    void LR2InversePiFixedBack(bitset<Size> &arr, bitset<Size> arrInput);
    void NewMatInverseFixedBack(bitset<Size> &arr, bitset<Size> arrInput);
    void PiInverseLR1InverseFixedBack(bitset<Size> &arr, bitset<Size> arrInput);

    void SboxFixedFor(bitset<Size> &arr, bitset<Size> inputDiff, bitset<Size/4> &incomp, int &incomn);
    void LR1PiFixedFor(bitset<Size> &arr, bitset<Size> arrInput);
    void NewMatFixedFor(bitset<Size> &arr, bitset<Size> arrInput);
    void PiInverseLR2FixedFor(bitset<Size> &arr, bitset<Size> arrInput);


    void PushPoints(vector<planePoints> &vP, twoDimCd aP);
    void PopPoints(vector<planePoints> &vP); 
};

#endif