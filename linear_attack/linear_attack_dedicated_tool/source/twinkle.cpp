#include "../include/twinkle.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include "../include/twinkle.h"


using namespace std;

void Twinkle::InitParam() {
    for (uint8_t i = 0; i < 16; i++) {
        SboxDDT[i] = vector<uint8_t>();
    }
    for (uint8_t i = 0; i < 16; i++) {
        for (uint8_t j = 0; j < 16; j++) {
            SboxDDT[outputIndex[j]].push_back(SboxDDTI[outputIndex[i]][j]);
        }
    }
    for (int i = 0; i < 16; i++) {
        LROffset2.push_back((LANESIZE - LROffset1I[i] % LANESIZE)%LANESIZE);
        LROffset1.push_back((LANESIZE - LROffset2I[i] % LANESIZE)%LANESIZE);
    }
    for (size_t i = 0; i < NewMatOffsetI.size(); i++) 
        NewMatOffset.push_back((PLANESIZE - NewMatOffsetI[i]) % PLANESIZE);
    for (size_t i = 0; i < NewMatInverseOffsetI.size(); i++) 
        NewMatInverseOffset.push_back((PLANESIZE - NewMatInverseOffsetI[i]) % PLANESIZE);
}

void Twinkle::InitSboxDiffParam() {
    

    MinDiff = 255;
    for (uint8_t i = 0; i < 16; i++) {
        SboxDiffIMin[i] = 255; //set a maximum weight
        SboxDiffOMin[i] = 255; //set a maximum weight
        SboxDiffIfOvCLowerBound[i] = map<uint8_t, WEIGHTTYPE> {};
        SboxDiffIT[i] = map<uint8_t, WEIGHTTYPE> {};
        SboxDiffOT[i] = map<uint8_t, WEIGHTTYPE> {};
        SboxDiffOfIvCLowerBound[i] = map<uint8_t, WEIGHTTYPE> {};
        for (int j = 0; j < 256; j++) {
            SboxDiffIfOvCLowerBound[i][(uint8_t)j] = 255; //set a maximum weight
            SboxDiffOfIvCLowerBound[i][(uint8_t)j] = 255; //set a maximum weight
        }
    }
    for (int i = 0; i < 256; i++) {
        SboxDiffILowerBound[(uint8_t)i] = 255; //set a maximum weight
        SboxDiffOLowerBound[(uint8_t)i] = 255; //set a maximum weight
    }
    for (uint8_t i = 0; i < 16; i++) {
        for (uint8_t j = 0; j < 16; j++) {
            if (SboxDDT[i][j] > 0) {
                WEIGHTTYPE wt = abs(log((WEIGHTTYPE)SboxDDT[i][j] / 16) / log(2));
                if (wt > 0 && MinDiff > wt) {
                    MinDiff = wt;
                }
                SboxDiffIMin[i] = min(wt, SboxDiffIMin[i]);
                SboxDiffOMin[outputIndex[j]] = min(wt, SboxDiffOMin[outputIndex[j]]);
                SboxDiffIT[i][outputIndex[j]] = wt;
                SboxDiffOT[outputIndex[j]][i] = wt;
                for (uint8_t k = 0; k < 16; k++) {
                    if ((outputIndex[j] & k) == outputIndex[j]){
                        SboxDiffIfOvCLowerBound[i][(outputIndex[j] << 4) | k] = wt;
                    }
                    if ((i  & k) == i) {
                        SboxDiffOfIvCLowerBound[outputIndex[j]][(i << 4)| k] = wt;
                    }
                    
                }   
            }
        }
    }
    for (uint8_t i = 0; i < 16; i++) {
        for (uint8_t j = 0; j < 16; j++) {
            if ((i & j) == i) {
                for (uint8_t l = 0; l < 16; l++) {
                    if ((i & l) == i && ((i ^j) & l) == 0) {
                        SboxDiffILowerBound[(i<<4)|l] = min(SboxDiffILowerBound[(i<<4)|l], SboxDiffIMin[j]);
                        SboxDiffOLowerBound[(i<<4)|l] = min(SboxDiffOLowerBound[(i<<4)|l], SboxDiffOMin[j]);
                    }
                    
                }        
            }
            for (uint8_t k = 0; k < 16; k++) {
                if ((j & k) == j && SboxDiffIfOvCLowerBound[i][(k<<4) | 0xf] < SboxDiffIfOvCLowerBound[i][(j<<4) | 0xf]) {
                    for (uint8_t l = 0; l < 16; l++) {
                        if (((j & l) == j) && (((k ^ j) & l) == 0)  &&  SboxDiffIfOvCLowerBound[i][(k<<4) | 0xf] < SboxDiffIfOvCLowerBound[i][(j<<4) | l]) 
                            SboxDiffIfOvCLowerBound[i][(j<<4) | l] = SboxDiffIfOvCLowerBound[i][(k<<4) | 0xf];
                    }
                    
                }
                if ((j & k) == j && SboxDiffOfIvCLowerBound[i][(k<<4) | 0xf] < SboxDiffOfIvCLowerBound[i][(j<<4) | 0xf]) {
                    for (uint8_t l = 0; l < 16; l++) {
                        if (((j & l) == j) && (((k ^ j) & l) == 0)  &&  SboxDiffOfIvCLowerBound[i][(k<<4) | 0xf] < SboxDiffOfIvCLowerBound[i][(j<<4) | l]) 
                            SboxDiffOfIvCLowerBound[i][(j<<4) | l] = SboxDiffOfIvCLowerBound[i][(k<<4) | 0xf];
                    }
                }
            }
        }
    }
}


void Twinkle::NewMat(bitset<Size> &arr, twoDimCd aP) {
	for (unsigned int i = 0; i < NewMatOffset.size(); i++) {
		arr.flip(((int)aP.x + NewMatOffset[i]) % PLANESIZE + PLANESIZE * (int)aP.z);
	}
}

void Twinkle::PiInverseLR1Inverse(bitset<Size>& arr, twoDimCd aP) {
	int newx = (int)aP.x * PiInverse % PLANESIZE;
	arr.flip( newx + PLANESIZE * (((int)aP.z + LROffset1[newx]) % LANESIZE)); 
}

void Twinkle::PiInverseLR1InverseFixed(bitset<Size>& arr, twoDimCd aP, bool op) {
    int newx = (int)aP.x * PiInverse % PLANESIZE;
	arr[newx + PLANESIZE * (((int)aP.z + LROffset1[newx]) % LANESIZE)] = op; 	
}

void Twinkle::NewMatPiInverseLR2(bitset<Size>& arr, twoDimCd aP) {
	for (unsigned int i = 0; i < NewMatOffset.size(); i++) {
        int newx = ((int)aP.x + NewMatOffset[i]) * PiInverse % PLANESIZE;
		arr.flip(newx + PLANESIZE * (((int)aP.z + (LANESIZE - LROffset2[newx])) % LANESIZE)); 
	}
}

void Twinkle::NewMatPiInverseLR2Fixed(bitset<Size> &arr, bitset<Size> fixed, twoDimCd aP, bool op) {
    int maxFixedIn = PLANESIZE - 1;
    for (int i = (int)aP.x + 1; i < (int)PLANESIZE; i++) {
        if (fixed[i + (int)aP.z * PLANESIZE]  == 0) {
            maxFixedIn = i - 1;
            break;
        }
    }
    if (maxFixedIn == PLANESIZE - 1) {
        for (int i = maxFixedIn - NewMatFixedHd + 1; i < PLANESIZE + NewMatFixedL; i++) {
            int newx = (i + PLANESIZE) * PiInverse % PLANESIZE;
		    arr[newx + PLANESIZE * (((int)aP.z + (LANESIZE - LROffset2[newx])) % LANESIZE)] = op; 
        }
    } 
    for (int i = max(NewMatFixedL, (int)aP.x - NewMatFixedHd); i <= maxFixedIn - NewMatFixedHd; i++) {
        int newx = i * PiInverse % PLANESIZE;
		arr[newx + PLANESIZE * (((int)aP.z + (LANESIZE - LROffset2[newx])) % LANESIZE)] = op;
    }
}


void Twinkle::NewMatInversePiInverseLR1Inverse(bitset<Size> &arr, twoDimCd aP) {
	for (unsigned int i = 0; i < NewMatInverseOffset.size(); i++) {
        int newx = ((int)aP.x + NewMatInverseOffset[i]) * PiInverse % PLANESIZE;
		arr.flip(newx + PLANESIZE * (((int)aP.z + LROffset1[newx]) % LANESIZE));
	}
}


void Twinkle::NewMatInversePiInverseLR1InverseFixed(bitset<Size> &arr, bitset<Size> fixed, twoDimCd aP, bool op) { //op: add 1, op: del: 0
    int maxFixedIn = PLANESIZE - 1;
    for (int i = (int)aP.x + 1; i < (int)PLANESIZE; i++) {
        if (fixed[i + (int)aP.z * PLANESIZE]  == 0) {
            maxFixedIn = i - 1;
            break;
        }
    }
    if (maxFixedIn == (int)PLANESIZE - 1) {
        for (int i = maxFixedIn - NewMatInverseFixedHd + 1; i < PLANESIZE + NewMatInverseFixedL; i++) {
            int newx = i * PiInverse % PLANESIZE;
            int newz = ((int)aP.z + LROffset1[newx]) % LANESIZE;
            uint8_t output, outputFixed;
            if (op) {
                arr[newx + PLANESIZE * newz] = 1;
            } else {
                arr[newx + PLANESIZE * newz] = 0;
            }
        }
    }
    for (int i = max(NewMatInverseFixedL, (int)aP.x - NewMatInverseFixedHd); i <= maxFixedIn - NewMatInverseFixedHd; i++) {   
        int newx = i * PiInverse % PLANESIZE;
        int newz = ((int)aP.z + LROffset1[newx]) % LANESIZE;
        if (op) {
            arr[newx + PLANESIZE * newz] = 1;
        } else {
            arr[newx + PLANESIZE * newz] = 0;
        }
    }
}

void Twinkle::PiInverseLR2(bitset<Size>& arr, twoDimCd aP) {
    int newx = (int)aP.x  * PiInverse % PLANESIZE;
	arr.flip(newx + PLANESIZE * (((int)aP.z + (LANESIZE - LROffset2[newx])) % LANESIZE)); 
}

void Twinkle::PiInverseLR2Fixed(bitset<Size>& arr, twoDimCd aP, bool op) {
    if (op) {
        int newx = (int)aP.x * PiInverse % PLANESIZE;
	    arr[newx + PLANESIZE * (((int)aP.z + (LANESIZE - LROffset2[newx])) % LANESIZE)] = 1; 
    } else {
        int newx = (int)aP.x * PiInverse % PLANESIZE;
	    arr[newx + PLANESIZE * (((int)aP.z + (LANESIZE - LROffset2[newx])) % LANESIZE)] = 0; 
    }
}


void Twinkle::SboxInverseFixedBack(bitset<Size> &arr, bitset<Size> outputDiff, bitset<Size/4> &incomp, int &incomn) {
    arr.reset();
    incomp.reset();
    incomn = 0;
    for (int i = 0; i < (int) LANESIZE; i++) {
        for (int j = 0; j < (int) 4; j++) {
            int in = 4 * j + i * PLANESIZE;
            if ((outputDiff[in] | outputDiff[in + 1] | outputDiff[in + 2] | outputDiff[in + 3]) == 0) {
                arr[in] = 1; arr[in + 1] = 1; arr[in + 2] = 1; arr[in + 3] = 1;
            }
            else {
                incomp[in/4] = 1;
                incomn++;
            }
            
        }
    }
}

void Twinkle::LR2InversePiFixedBack(bitset<Size> &arr, bitset<Size> arrInput) {
    arr.reset();
    for (int i = 0; i < (int) LANESIZE; i++) {
        for (int j = 0; j < (int) PLANESIZE; j++) {
            if (arrInput[j+ i * PLANESIZE]) {
                int newz = (LANESIZE + i + LROffset2[j]) % LANESIZE;
	            arr[j * Pi % PLANESIZE + PLANESIZE * newz] = 1; 
            }
        }
    }
}

void Twinkle::NewMatInverseFixedBack(bitset<Size> &arr, bitset<Size> arrInput) {
    arr.reset();
    arr.flip();
    for (int i = 0; i < (int) LANESIZE; i++) {
        for (int j = 0; j < (int) PLANESIZE; j++) {
            if (arrInput[j+ i * PLANESIZE] == 0) {
                for (int k = 0; k < NewMatInverseOffset.size(); k++) {
                    arr[(j + NewMatInverseOffset[k]) % PLANESIZE + PLANESIZE * i] = 0;
                }   
            }
        }
    }
}
void Twinkle::PiInverseLR1InverseFixedBack(bitset<Size> &arr, bitset<Size> arrInput) {
    arr.reset();
    for (int i = 0; i < (int) LANESIZE; i++) {
        for (int j = 0; j < (int) PLANESIZE; j++) {
            if (arrInput[j+ i * PLANESIZE]) {
                int newx = j * PiInverse % PLANESIZE;
	            arr[newx + PLANESIZE * ((i + LROffset1[newx]) % LANESIZE)] = 1; 
            }
        }
    }
}

void Twinkle::SboxFixedFor(bitset<Size> &arr, bitset<Size> inputDiff, bitset<Size/4> &incomp, int & incomn) {
    arr.reset();
    incomp.reset();
    incomn = 0;
    for (int i = 0; i < (int) LANESIZE; i++) {
        for (int j = 0; j < (int) 4; j++) {
            int in = 4*j+ i * PLANESIZE;
            if ((inputDiff[in] | inputDiff[in + 1] | inputDiff[in + 2] | inputDiff[in + 3]) == 0) {
                arr[in] = 1; arr[in + 1] = 1; arr[in + 2] = 1; arr[in + 3] = 1;
            }
            else {
                incomp[in/4] = 1;
                incomn ++;
            }
            
        }
    }
}

void Twinkle::LR1PiFixedFor(bitset<Size> &arr, bitset<Size> arrInput) {
    arr.reset();
    for (int i = 0; i < (int) LANESIZE; i++) {
        for (int j = 0; j < (int) PLANESIZE; j++) {
            if (arrInput[j+ i * PLANESIZE]) {
                int newz = ((int)LANESIZE + i - LROffset1[j]) % LANESIZE;
	            arr[j * Pi % PLANESIZE + PLANESIZE * newz] = 1; 
            }
        }
    }
}

void Twinkle::NewMatFixedFor(bitset<Size> &arr, bitset<Size> arrInput) {
    arr.reset();
    arr.flip();
    for (int i = 0; i < (int) LANESIZE; i++) {
        for (int j = 0; j < (int) PLANESIZE; j++) {
            if (arrInput[j+ i * PLANESIZE] == 0) {
                for (int k = 0; k < NewMatOffset.size(); k++) {
                    arr[(j + NewMatOffset[k]) % PLANESIZE + PLANESIZE * i] = 0;
                }   
            }
        }
    }
}

void Twinkle::PiInverseLR2FixedFor(bitset<Size> &arr, bitset<Size> arrInput) {
    arr.reset();
    for (int i = 0; i < (int) LANESIZE; i++) {
        for (int j = 0; j < (int) PLANESIZE; j++) {
            if (arrInput[j+ i * PLANESIZE]) {
                int newx = j * PiInverse % PLANESIZE;
	            arr[newx + PLANESIZE * ((i + (int) LANESIZE - LROffset2[newx]) % LANESIZE)] = 1; 
            }
        }
    }
}

void Twinkle:: PushPoints(vector<planePoints> &vP, twoDimCd aP) {
    if (vP.size() > 0) {
        if (vP[vP.size() - 1].z == aP.z) 
            vP[vP.size() - 1].xs.push_back(aP.x);
        else {
            vP.push_back(planePoints{vector<uint8_t>{aP.x}, aP.z});
        }
    }
    else {
        vP.push_back(planePoints{vector<uint8_t>{aP.x}, aP.z});
    }
}

void Twinkle:: PopPoints(vector<planePoints> &vP) {
    vP[vP.size() - 1].xs.pop_back();
    if (vP[vP.size() - 1].xs.size() == 0) {
        vP.pop_back();
    }
}


