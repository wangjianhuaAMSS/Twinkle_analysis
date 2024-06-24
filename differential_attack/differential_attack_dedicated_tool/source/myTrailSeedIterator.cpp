#include <iostream>
#include <stdio.h>
#include <fstream>
#include <iomanip>
#include<algorithm>
#include <bitset>
#include "../include/myTrailSeedIterator.h"


using namespace std;

TrailSeedIterator::TrailSeedIterator(const WEIGHTTYPE aMaxCost2Round, const WEIGHTTYPE aMaxCost4Round) :
	MaxCost2Round(aMaxCost2Round), MaxCost4Round(aMaxCost4Round) {
	initialization();
}

TrailSeedIterator::TrailSeedIterator(const TrailSeedIterator &T){
	MaxCost2Round = T.MaxCost2Round;
    MaxCost4Round = T.MaxCost4Round;

    SboxDiffIMin = T.SboxDiffIMin;
    SboxDiffOMin = T.SboxDiffOMin;
    SboxDiffILowerBound = T.SboxDiffILowerBound;
    SboxDiffOLowerBound = T.SboxDiffOLowerBound;
    SboxDiffIfOvCLowerBound = T.SboxDiffIfOvCLowerBound;
    SboxDiffOfIvCLowerBound = T.SboxDiffOfIvCLowerBound;

    Min1Round = T.Min1Round;
	BeforeNewMat = T.BeforeNewMat;
	AfterNewMat = T.AfterNewMat;
	BeforeSbox = T.BeforeSbox;
    AfterSbox = T.AfterSbox;

    BeforeSboxTempFixed = T.BeforeSboxTempFixed;
    AfterSboxTempFixed = T.AfterSboxTempFixed;
    minWeight = T.minWeight;
    lowerBoundWeight = T.lowerBoundWeight;
    minAfterSboxWeight = T.minAfterSboxWeight;
    minBeforeSboxWeight = T.minBeforeSboxWeight;
    nx = T.nx;
    nz = T.nz;
    position = T.position;

    InitPos = {T.position[T.nz-1].xs[T.nx-1], T.position[T.nz-1].z};
    
}

void TrailSeedIterator::initialization() {
	InitSboxDiffParam();

    Min1Round = MinDiff;
	BeforeNewMat.reset();
	AfterNewMat.reset();
	BeforeSbox.reset();
    AfterSbox.reset();

    BeforeSboxTempFixed.reset();
    AfterSboxTempFixed.reset();
    minWeight = 0;
    lowerBoundWeight = 0;
    minAfterSboxWeight = 0;
    minBeforeSboxWeight = 0;
    nx = 0;
    nz = 0;
    position.clear();

    Consistant(twoDimCd{0, 0}, 1, 1);
}




bool TrailSeedIterator::toChild() {
    int CurrentIndex = (int)position[nz-1].xs[nx - 1] + PLANESIZE * (int)position[nz - 1].z;
    twoDimCd CurrentPoint{position[nz-1].xs[nx-1], position[nz-1].z};
    for (int i = CurrentIndex + 1; i < Size; i++) {
        twoDimCd aP{i % (int)PLANESIZE, i /(int)PLANESIZE};
        if (Consistant(aP, 1, 1)) {
            return true;
        } else if (Consistant(aP, 2, 0)){
        } else {
            for (int j = i; j >= CurrentIndex + 1; j--) {
                Consistant(twoDimCd{j % (int)PLANESIZE, j /(int)PLANESIZE}, 0, 0);
            }
            return false;
        }   
    }
    for (int i = Size - 1; i >= CurrentIndex + 1 ; i--) {
        twoDimCd aP{i % (int)PLANESIZE, i /(int)PLANESIZE};
        Consistant(aP, 0, 0);  
    }
	return false;
}

bool TrailSeedIterator::toSibling() {
    int CurrentIndex = (int)position[nz-1].xs[nx - 1] + PLANESIZE * (int)position[nz - 1].z;
    twoDimCd CurrentPoint{position[nz-1].xs[nx-1], position[nz-1].z};
    if ((CurrentIndex == Size - 1) || (isCanonical() == false) ) {
        return false;
    }
	if (Consistant(CurrentPoint, 2, 0)) { // check 0
        for (int i = CurrentIndex + 1; i < Size; i++) {
            if (Consistant(twoDimCd{i % (int)PLANESIZE, i /(int)PLANESIZE}, 1, 1)) { //check 1 
                return true;
            } else if (Consistant(twoDimCd{i % (int)PLANESIZE, i /(int)PLANESIZE}, 2, 0)){ //check 0
                    
            } else {
                for (int j = i; j >= CurrentIndex + 1; j--) {
                    Consistant(twoDimCd{j % (int)PLANESIZE, j /(int)PLANESIZE}, 0, 0);
                }
                Consistant(CurrentPoint, 2, 1); //recover to be 1 
                return false;
            }
            
        }
        for (int i = Size - 1; i >= CurrentIndex + 1 ; i--) {
            twoDimCd aP{i % (int)PLANESIZE, i /(int)PLANESIZE};
            Consistant(aP, 0, 0);   
        }  
    } 
	Consistant(CurrentPoint, 2, 1); //recover to be 1  
	return false;
}
/* set multi parents for multi threads */
bool TrailSeedIterator::toPar() {
    if (nz == 1 && nx == 1) {
        return false;
    }
    int CurrentIndex = (int)position[nz-1].xs[nx-1] + PLANESIZE * (int)position[nz-1].z;
    Consistant(twoDimCd{position[nz-1].xs[nx-1], position[nz-1].z}, 0, 1);
    int newIndex = (int)position[nz-1].xs[nx-1] + PLANESIZE * (int)position[nz-1].z;
    for (int i = CurrentIndex - 1; i > newIndex; i--) {
        Consistant(twoDimCd{i % (int)PLANESIZE, i / (int)PLANESIZE}, 0, 0);
    }
	return(true);
}


/* set multi parents for multi threads */
bool TrailSeedIterator::toparForThread() {
    if ((position[nz-1].xs[nx-1] == InitPos.x) && (position[nz-1].z == InitPos.z))  {
        return false;
    }
    int CurrentIndex = (int)position[nz-1].xs[nx-1] + PLANESIZE * (int)position[nz-1].z;
    Consistant(twoDimCd{position[nz-1].xs[nx-1], position[nz-1].z}, 0, 1);
    int newIndex = (int)position[nz-1].xs[nx-1] + PLANESIZE * (int)position[nz-1].z;
    for (int i = CurrentIndex - 1; i > newIndex; i--) {
        Consistant(twoDimCd{i % (int)PLANESIZE, i / (int)PLANESIZE}, 0, 0);
    }
	return(true);
}

bool TrailSeedIterator::toSiblingForThread() {
    if ((position[nz-1].xs[nx-1] == InitPos.x) && (position[nz-1].z == InitPos.z))  {
        return false;
    }
    int CurrentIndex = (int)position[nz-1].xs[nx - 1] + PLANESIZE * (int)position[nz - 1].z;
    twoDimCd CurrentPoint{position[nz-1].xs[nx-1], position[nz-1].z};
    if ((CurrentIndex == Size - 1) || (isCanonical() == false) ) {
        return false;
    }
	if (Consistant(CurrentPoint, 2, 0)) { // check 0
        for (int i = CurrentIndex + 1; i < Size; i++) {
            if (Consistant(twoDimCd{i % (int)PLANESIZE, i /(int)PLANESIZE}, 1, 1)) { //check 1 
                return true;
            } else if (Consistant(twoDimCd{i % (int)PLANESIZE, i /(int)PLANESIZE}, 2, 0)){ //check 0
                    
            } else {
                for (int j = i; j >= CurrentIndex + 1; j--) {
                    Consistant(twoDimCd{j % (int)PLANESIZE, j /(int)PLANESIZE}, 0, 0);
                }
                Consistant(CurrentPoint, 2, 1); //recover to be 1 
                return false;
            }
            
        }
        for (int i = Size - 1; i >= CurrentIndex + 1 ; i--) {
            twoDimCd aP{i % (int)PLANESIZE, i /(int)PLANESIZE};
            Consistant(aP, 0, 0);   
        }  
    } 
	Consistant(CurrentPoint, 2, 1); //recover to be 1  
	return false;
}

bool TrailSeedIterator::next() {
	if (toChild() == false) {
		do {
			if (toSibling() == true) {
				return true;
			}
			else if (toPar() == false) {
				return false;
			}
		} while (true);
	}
	else {
		return true;
	}
}

bool TrailSeedIterator::nextWithChildFlag(bool flag) {
    if (flag) {
        if (toChild() == false) {
		    do {
			    if (toSibling() == true) {
				    return true;
			    }
			    else if (toPar() == false) {
				    return false;
			    }
		    } while (true);
	    }
	    else {
		    return true;
	    }
    } else {
        do {
			if (toSibling() == true) {
				return true;
			}
			else if (toPar() == false) {
				return false;
			}
		} while (true);
    }
	
}

bool TrailSeedIterator::nextForThread() {
	if (toChild() == false) {
		do {
			if (toSiblingForThread() == true) {
				return true;
			}
			else if (toparForThread() == false) {
				return false;
			}
		} while (true);
	}
	else {
		return true;
	}
}


void TrailSeedIterator::updateOperateOnNewPoints(twoDimCd aP, int op, bool value) { //op: 0:del, 1:add, 2:modify //if modify, flip
    bitset<Size> fixed0;
    if (op == 0) {//del
        NewMatPiInverseLR2Fixed(BeforeSboxTempFixed, fixed0, aP, false);
        PiInverseLR1InverseFixed(AfterSboxTempFixed, aP, false);
        if (value) {
            BeforeNewMat.flip((int)aP.x + (int)aP.z * PLANESIZE);
            NewMatPiInverseLR2(BeforeSbox, aP);
            PiInverseLR1Inverse(AfterSbox, aP);
        }  
    } else if (op == 1) {//add
        NewMatPiInverseLR2Fixed(BeforeSboxTempFixed, fixed0, aP, true);
        PiInverseLR1InverseFixed(AfterSboxTempFixed, aP, true);
        if (value) {
            BeforeNewMat.flip((int)aP.x + (int)aP.z * PLANESIZE);
            NewMatPiInverseLR2(BeforeSbox, aP);
            PiInverseLR1Inverse(AfterSbox, aP);
        }
    } else {
        BeforeNewMat.flip((int)aP.x + (int)aP.z * PLANESIZE);
        NewMatPiInverseLR2(BeforeSbox, aP);
        PiInverseLR1Inverse(AfterSbox, aP);
    }
}

bool TrailSeedIterator::Consistant(twoDimCd aP, int op, int value) { //op and value: 1 and 1, 2 and 0, 2 and 1, 0 and 0, 0 and 1
    vector<uint8_t> inputsOldL, inputsOldM, inputsFixedOld;
    vector<uint8_t> outputsOldL, outputsOldM, outputsFixedOld;
    //forward
    generateLowerBoundBeforeSboxes(aP, inputsOldL, inputsFixedOld);
    generateMinBeforeSboxes(aP, inputsOldM);
    //backward
    generateLowerBoundRevAfterSboxes(aP, outputsOldL, outputsFixedOld); 
    generateMinRevAfterSboxes(aP, outputsOldM);

    //update
    updateOperateOnNewPoints(aP, op, value);
    
    vector<uint8_t> inputsL, inputsM, inputsFixed;
    vector<uint8_t> outputsL, outputsM, outputsFixed;
    //forward
    generateLowerBoundBeforeSboxes(aP, inputsL, inputsFixed);
    generateMinBeforeSboxes(aP, inputsM);
    //backward
    generateLowerBoundRevAfterSboxes(aP, outputsL, outputsFixed); 
    generateMinRevAfterSboxes(aP, outputsM);
    
    //forward weight change
    WEIGHTTYPE wtBefL = updateLowerBoundWeightBeforeSbox(inputsL, inputsFixed, inputsOldL, inputsFixedOld);
    WEIGHTTYPE wtBefM = updateMinWeightBeforeSbox(inputsM, inputsOldM);
    //backward weight change
    WEIGHTTYPE wtAftL = updateLowerBoundWeightRevAfterSbox(outputsOldL, outputsFixedOld, outputsL, outputsFixed);
    WEIGHTTYPE wtAftM = updateMinWeightRevAfterSbox(outputsM, outputsOldM);
    //update weight
    lowerBoundWeight += wtAftL + wtBefL;
    minBeforeSboxWeight += wtBefM;
    minAfterSboxWeight += wtAftM;
    minWeight +=  (wtBefM + wtAftM);

    if (op >= 1 && value == 1) {
        PushPoints(position, aP);
        nz = position.size();
        nx = position[nz - 1].xs.size();
    } else if ((op == 2 && value == 0) || (op == 0 && value == 1)) {
        PopPoints(position);
        nz = position.size();
        nx = position[nz - 1].xs.size();
    }
    
    if (lowerBoundWeight <= MaxCost2Round) {
        if (op == 1) {
            if (isCanonical()) {
                if (minWeight <= MaxCost2Round) {
                    Three_RoundTrailCore();
                    makeHisto();
                    mtx.lock();
                    display2RCore();
                    mtx.unlock();
                    //display2RCore();
                }
                return true;
            } else {
                return false;
            }
        }
        return(true);     
    } else {
        return(false);
    }             
    return(false);
	
}


void TrailSeedIterator::generateLowerBoundBeforeSboxes(twoDimCd aP, vector<uint8_t> &inputs, vector<uint8_t> &inputsFixed) {
    inputs.clear();
    inputsFixed.clear();
    int maxFixedIn = PLANESIZE - 1;
    for (int i = (int)aP.x + 1; i < (int)PLANESIZE; i++) {
        if (BeforeNewMatTempFixed[i + (int)aP.z * PLANESIZE]  == 0) {
            maxFixedIn = i - 1;
            break;
        }
    }
    if (maxFixedIn == (int)PLANESIZE - 1) {
        for (unsigned int i = maxFixedIn - NewMatFixedHd + 1; i < PLANESIZE + NewMatFixedL; i++) {
            int newx = i * PiInverse % PLANESIZE;
            int newz = ((int)aP.z + (LANESIZE - LROffset2[newx])) % LANESIZE;
            uint8_t input, inputFixed;
            SboxDiff1Side(inputFixed, input, twoDimCd{newx, newz}, BeforeSboxTempFixed, BeforeSbox);
            inputs.push_back(input);
            inputsFixed.push_back(inputFixed);
        }
    }
    for (int i = max(NewMatFixedL, (int)aP.x - NewMatFixedHd); i <= maxFixedIn - NewMatFixedHd; i++) {   
        int newx = i * PiInverse % PLANESIZE;
        int newz = ((int)aP.z + (LANESIZE - LROffset2[newx])) % LANESIZE;
        uint8_t input, inputFixed;
        SboxDiff1Side(inputFixed, input, twoDimCd{newx, newz}, BeforeSboxTempFixed, BeforeSbox);
        inputs.push_back(input);
        inputsFixed.push_back(inputFixed);
    }
}

void TrailSeedIterator::generateLowerBoundRevAfterSboxes(twoDimCd aP, vector<uint8_t> &outputs, vector<uint8_t> &outputsFixed) {
    outputs.clear();
    outputsFixed.clear();
     
    int newx = (int)aP.x * PiInverse % PLANESIZE;
    int newz = ((int)aP.z + LROffset1[newx]) % LANESIZE;
    uint8_t output, outputFixed;
    SboxDiff1Side(outputFixed, output, twoDimCd{newx, newz}, AfterSboxTempFixed, AfterSbox);
    outputs.push_back(output);
    outputsFixed.push_back(outputFixed);
}

void TrailSeedIterator::generateMinBeforeSboxes(twoDimCd aP, vector<uint8_t> &inputs) {
    inputs.clear();
    for (int i = 0; i < NewMatOffset.size(); i++) {
        int newx = ((int)aP.x + NewMatOffset[i]) * PiInverse % PLANESIZE;
        int newz = ((int)aP.z + (LANESIZE - LROffset2[newx])) % LANESIZE;
        uint8_t input = generateSbox(twoDimCd{newx, newz}, BeforeSbox);
        inputs.push_back(input);
    }
}

void TrailSeedIterator::generateMinRevAfterSboxes(twoDimCd aP, vector<uint8_t> &outputs) {
    outputs.clear();
     
    int newx = (int)aP.x * PiInverse % PLANESIZE;
    int newz = ((int)aP.z + LROffset1[newx]) % LANESIZE;
    uint8_t output = generateSbox(twoDimCd{newx, newz}, AfterSbox);
    outputs.push_back(output);
}


WEIGHTTYPE TrailSeedIterator::updateLowerBoundWeightBeforeSbox(vector<uint8_t> inputs, vector<uint8_t> inputsFixed, vector<uint8_t> inputsOld, vector<uint8_t> inputsFixedOld) {
    WEIGHTTYPE wt = 0;
    for (size_t i = 0; i < inputs.size(); i++) {
        wt += SboxDiffILowerBound[((inputs[i] & inputsFixed[i]) << 4) | inputsFixed[i]] - SboxDiffILowerBound[((inputsOld[i] & inputsFixedOld[i]) << 4) | inputsFixedOld[i]];
    }
    return(wt); 
}

WEIGHTTYPE TrailSeedIterator::updateLowerBoundWeightRevAfterSbox(vector<uint8_t> outputsOld, vector<uint8_t> outputsFixedOld, vector<uint8_t> outputs, vector<uint8_t> outputsFixed) { //op: true: add, false: del
    WEIGHTTYPE wt = 0;
    for (size_t i = 0; i < outputsOld.size(); i++) {
        wt+= SboxDiffOLowerBound[((outputs[i] & outputsFixed[i]) << 4) | outputsFixed[i]] - SboxDiffOLowerBound[((outputsOld[i] & outputsFixedOld[i]) << 4) | outputsFixedOld[i]];
    }
    return(wt); 
}

WEIGHTTYPE TrailSeedIterator::updateMinWeightBeforeSbox(vector<uint8_t> inputs, vector<uint8_t> inputsOld) {
    WEIGHTTYPE wt = 0;
    for (size_t i = 0; i < inputs.size(); i++) {
        wt += SboxDiffIMin[inputs[i]] - SboxDiffIMin[inputsOld[i]];
    }
    return(wt); 
}


WEIGHTTYPE TrailSeedIterator::updateMinWeightRevAfterSbox(vector<uint8_t> outputs, vector<uint8_t> outputsOld) { 
    WEIGHTTYPE wt = 0;
    for (size_t i = 0; i < outputsOld.size(); i++) {
        wt+= SboxDiffOMin[outputs[i]] - SboxDiffOMin[outputsOld[i]];
    }
    return(wt); 
}



bool TrailSeedIterator::isCanonical() {
	if (nz == 1 && position[nz-1].z !=0) {
		return false;
	}
	for (int i = nz - 1; i > 0; i--) {
		int h = 1;
		int x = LANESIZE - (int)position[nz - 1].z;
		for (int j = nz + i; j > i + 1; j--) {
			if (x < ((LANESIZE + (int)position[j % nz].z - (int)position[(j - 1) % nz].z) % LANESIZE)) {
				return false;
			}
			else if (x == ((LANESIZE + (int)position[j % nz].z - (int)position[(j - 1) % nz].z) % LANESIZE)) {
				h++;
				if (h == nz) {
					//cout << "Symmetry!!!!!!!!!\n";
                    for (int k = nz - 1; k >= 0; k--) {
                        if (position[k].xs != position[(k + i) % nz].xs) {
                            if (position[(k + i) % nz].xs.size() < position[k].xs.size()) {
                                return false;
                            } else if (position[(k + i) % nz].xs.size() == position[k].xs.size()) {
                                for (size_t l = 0; l < position[k].xs.size(); l++) {
                                    if (position[(k + i) % nz].xs[l] < position[k].xs[l]) {
                                        return false;
                                    }
                                    else if (position[(k + i) % nz].xs[l] > position[k].xs[l]) {
                                        k = -1;
                                        break;
                                    }      
                                }  
                            } else {
                                k = -1;
                            }    
                        }
                    }
				}
				else {
					x = position[nz - h + 1].z - position[nz - h].z;
				}
			}
			else {
				j = i;
			}
		}
	}
	return true;
}

void TrailSeedIterator::hi22(ofstream& fou) {
	fou << "   2-round \n";
	fou << setw(4) << "w(a)/w(b)" << ",";
	for (int i = 1; i <= MaxCost2Round - 1; i++) {
		fou << setw(3) << i << ",";
	}
	fou << "\n";
	for (int i = 1; i <= MaxCost2Round - 1; i++) {
		fou << setw(4) << i << ",     ";
		for (int j = 1; j <= MaxCost2Round - i; j++) {
			fou << setw(3) << his[i][j] << ",";
		}
		fou << "\n";
	}
}

void TrailSeedIterator::hi2(ofstream& fou) {
	fou << "   2-round \n";
	fou << setw(4) << "w" << setw(16) << "#";
	fou << "\n";
	for (int i = 4; i <= MaxCost2Round; i++) {
		fou << setw(4) << i;
		if (his2R[i] == 0) {
			fou << setw(16) << "-";
        }
		else {
			fou << setw(16) << his2R[i];
        }
		fou << "\n";
	}
}

void TrailSeedIterator::hi3(ofstream& fou, int *his3R) {
	fou << "   3-round \n";
	fou << setw(4) << "w" << setw(16) << "#";
	fou << "\n";
	for (int i = 7; i <= MaxCost4Round - 1; i++) {
		fou << setw(4) << i;
		if (his3R[i] == 0) {
			fou << setw(16) << "-";
        }
		else {
			fou << setw(16) << his3R[i];
        }
		fou << "\n";
	}
}

void TrailSeedIterator::hi4(ofstream& fou, int *his4R) {
	fou << "   4-round \n";
	fou << setw(4) << "w" << setw(16) << "#";
	fou << "\n";
	for (int i = 8; i <= MaxCost4Round; i++) {
		fou << setw(4) << i;
		if (his4R[i] == 0) {
			fou << setw(16) << "-";
        }
		else {
			fou << setw(16) << his4R[i];
        }
		fou << "\n";
	}
}

void TrailSeedIterator::showPosition(vector<planePoints> p, ofstream& fou) {
    for (int i = 0; i < (int)p.size(); i++) {
        fou << (unsigned)p[i].z << ": ";
        for (size_t j = 0; j < p[i].xs.size(); j++) {
            fou << (unsigned)p[i].xs[j] << ", ";
        }
        fou << endl;
    }
}

void TrailSeedIterator::showPosition(vector<planePoints> p) {
    for (int i = 0; i < (int)p.size(); i++) {
        cout << (unsigned)p[i].z << ": ";
        for (size_t j = 0; j < p[i].xs.size(); j++) {
            cout << (unsigned)p[i].xs[j] << ", ";
        }
        cout << endl;
    }
}

uint8_t TrailSeedIterator::generateSbox(twoDimCd aP, bitset<Size> arr) {
    int Sbox0Index = (int)aP.z * 16 + (int)aP.x / 4 * 4;
    return ((arr[Sbox0Index] << 3) | (arr[Sbox0Index + 1] << 2) | (arr[Sbox0Index + 2] << 1) | arr[Sbox0Index + 3]);
}

void TrailSeedIterator::SboxDiff2Side(uint8_t &fixed, uint8_t &input, uint8_t &output, twoDimCd aP, bitset<Size> fixedAll, bitset<Size> inputAll, bitset<Size> outputAll) {
    int Sbox0Index = (int)aP.z * 16 + (int)aP.x / 4 * 4;
    fixed = (fixedAll[Sbox0Index] << 3) | (fixedAll[Sbox0Index + 1] << 2) | (fixedAll[Sbox0Index + 2] << 1) | fixedAll[Sbox0Index + 3];
    input = (inputAll[Sbox0Index] << 3) | (inputAll[Sbox0Index + 1] << 2) | (inputAll[Sbox0Index + 2] << 1) | inputAll[Sbox0Index + 3];
    output = (outputAll[Sbox0Index] << 3) | (outputAll[Sbox0Index + 1] << 2) | (outputAll[Sbox0Index + 2] << 1) | outputAll[Sbox0Index + 3];
}

void TrailSeedIterator::SboxDiff1Side(uint8_t &fixed, uint8_t &input, twoDimCd aP, bitset<Size> fixedAll, bitset<Size> inputAll) {
    int Sbox0Index = (int)aP.z * 16 + (int)aP.x / 4 * 4;
    fixed = (fixedAll[Sbox0Index] << 3) | (fixedAll[Sbox0Index + 1] << 2) | (fixedAll[Sbox0Index + 2] << 1) | fixedAll[Sbox0Index + 3];
    input = (inputAll[Sbox0Index] << 3) | (inputAll[Sbox0Index + 1] << 2) | (inputAll[Sbox0Index + 2] << 1) | inputAll[Sbox0Index + 3];
}




void TrailSeedIterator::display2RCore(ofstream& fou) {
    fou << "Wight = " << minAfterSboxWeight << " + " << minBeforeSboxWeight << " = " << minWeight << endl;
    Min2Round = min(Min2Round, minWeight);
    his2R[(int)round(minWeight)]++;
    fou << "--------------------AfterSbox--------------------" << endl;
    display(AfterSbox, fou);
    fou << "--------------------BeforeNewMat--------------------" << endl;
    display(BeforeNewMat, fou);
    fou << "--------------------AfterNewMat--------------------" << endl;
    display(AfterNewMat, fou);
    fou << "--------------------BeforeSbox--------------------" << endl;
    display(BeforeSbox,fou);
}

void TrailSeedIterator::display2RCore() {
    Min2Round = min(Min2Round, minWeight);
    his2R[(int)round(minWeight)]++;
}

void TrailSeedIterator::display3RCoreBack(ofstream& fou) {
    fou << "Wight = " << minWeightAftSboxBack << " + " << minWeightBefSboxBack << " + " << minBeforeSboxWeight << " = " << minWeightBack + minBeforeSboxWeight << endl;
    Min3RoundBack = min(Min3RoundBack, minWeightBack + minBeforeSboxWeight);
    his3RBack[(int)round(minWeightBack + minBeforeSboxWeight)]++;
    fou << "--------------------2R-AfterSbox--------------------" << endl;
    display(AfterSboxBack, fou);
    fou << "--------------------2R-BeforeNewMat--------------------" << endl;
    display(BeforeNewMatBack, fou);
    fou << "--------------------2R-AfterNewMat--------------------" << endl;
    display(AfterNewMatBack, fou);
    fou << "--------------------2R-BeforeSbox--------------------" << endl;
    display(BeforeSboxBack,fou);
    fou << "--------------------3R-AfterSbox--------------------" << endl;
    display(AfterSbox, fou);
    fou << "--------------------3R-BeforeNewMat--------------------" << endl;
    display(BeforeNewMat, fou);
    fou << "--------------------3R-AfterNewMat--------------------" << endl;
    display(AfterNewMat, fou);
    fou << "--------------------3R-BeforeSbox--------------------" << endl;
    display(BeforeSbox,fou);
}


void TrailSeedIterator::display3RCoreBack() {
    cout << "Wight = " << minWeightAftSboxBack << " + " << minWeightBefSboxBack << " + " << minBeforeSboxWeight << " = " << minWeightBack + minBeforeSboxWeight << endl;
    cout << "--------------------2R-AfterSbox--------------------" << endl;
    display(AfterSboxBack);
    cout << "--------------------2R-BeforeNewMat--------------------" << endl;
    display(BeforeNewMatBack);
    cout << "--------------------2R-AfterNewMat--------------------" << endl;
    display(AfterNewMatBack);
    cout << "--------------------2R-BeforeSbox--------------------" << endl;
    display(BeforeSboxBack);
    cout << "--------------------3R-AfterSbox--------------------" << endl;
    display(AfterSbox);
    cout << "--------------------3R-BeforeNewMat--------------------" << endl;
    display(BeforeNewMat);
    cout << "--------------------3R-AfterNewMat--------------------" << endl;
    display(AfterNewMat);
    cout << "--------------------3R-BeforeSbox--------------------" << endl;
    display(BeforeSbox);
}


void TrailSeedIterator::display3RCoreFor(ofstream& fou) {
    fou << "Wight = " << minAfterSboxWeight << " + " << minWeightAftSboxFor << " + " << minWeightBefSboxFor  << " = " << minWeightFor + minAfterSboxWeight << endl;
    Min3RoundFor = min(Min3RoundFor, minWeightFor + minAfterSboxWeight);
    his3RFor[(int)round(minWeightFor + minAfterSboxWeight)]++;
    fou << "--------------------1R-AfterSbox--------------------" << endl;
    display(AfterSbox, fou);
    fou << "--------------------1R-BeforeNewMat--------------------" << endl;
    display(BeforeNewMat, fou);
    fou << "--------------------1R-AfterNewMat--------------------" << endl;
    display(AfterNewMat, fou);
    fou << "--------------------1R-BeforeSbox--------------------" << endl;
    display(BeforeSbox,fou);
    fou << "--------------------2R-AfterSbox--------------------" << endl;
    display(AfterSboxFor, fou);
    fou << "--------------------2R-BeforeNewMat--------------------" << endl;
    display(BeforeNewMatFor, fou);
    fou << "--------------------2R-AfterNewMat--------------------" << endl;
    display(AfterNewMatFor, fou);
    fou << "--------------------2R-BeforeSbox--------------------" << endl;
    display(BeforeSboxFor, fou);
}

void TrailSeedIterator::display3RCoreFor() {
    cout << "Wight = " << minAfterSboxWeight << " + " << minWeightAftSboxFor << " + " << minWeightBefSboxFor  << " = " << minWeightFor + minAfterSboxWeight << endl;
    cout << "--------------------1R-AfterSbox--------------------" << endl;
    display(AfterSbox);
    cout << "--------------------1R-BeforeNewMat--------------------" << endl;
    display(BeforeNewMat);
    cout << "--------------------1R-AfterNewMat--------------------" << endl;
    display(AfterNewMat);
    cout << "--------------------1R-BeforeSbox--------------------" << endl;
    display(BeforeSbox);
    cout << "--------------------2R-AfterSbox--------------------" << endl;
    display(AfterSboxFor);
    cout << "--------------------2R-BeforeNewMat--------------------" << endl;
    display(BeforeNewMatFor);
    cout << "--------------------2R-AfterNewMat--------------------" << endl;
    display(AfterNewMatFor);
    cout << "--------------------2R-BeforeSbox--------------------" << endl;
    display(BeforeSboxFor);
}

void TrailSeedIterator::display4RCoreBack(ofstream& fou) {
    fou << "Wight = " << minWeightAftSboxBack << " + " << minWeightBefSboxBack  << " + " << Weight3RFinal2RBack[FourCoreInd]  << " = " << minWeightBack + Weight3RFinal2RBack[FourCoreInd] << endl;
    Min4RoundBack = min(Min4RoundBack, minWeightBack + Weight3RFinal2RBack[FourCoreInd]);
    his4RBack[(int)round(minWeightBack + Weight3RFinal2RBack[FourCoreInd])]++;
    fou << "--------------------2R-AfterSbox--------------------" << endl;
    display(AfterSboxBack, fou);
    fou << "--------------------2R-BeforeNewMat--------------------" << endl;
    display(BeforeNewMatBack, fou);
    fou << "--------------------2R-AfterNewMat--------------------" << endl;
    display(AfterNewMatBack, fou);
    fou << "--------------------2R-BeforeSbox--------------------" << endl;
    display(BeforeSboxBack,fou);
    fou << "--------------------3R-AfterSbox--------------------" << endl;
    display(AfterSbox3RFinalR1Back[FourCoreInd], fou);
    fou << "--------------------3R-BeforeSbox--------------------" << endl;
    display(BeforeSbox3RFinalR1Back[FourCoreInd],fou);
    fou << "--------------------4R-AfterSbox--------------------" << endl;
    display(AfterSbox3RFinalR2Back[FourCoreInd], fou);
    fou << "--------------------4R-BeforeSbox--------------------" << endl;
    display(BeforeSbox3RFinalR2Back[FourCoreInd],fou);
}
void TrailSeedIterator::display4RCoreFor(ofstream& fou) {
    fou << "Wight = " << Weight3RFinal2RFor[FourCoreInd] << " + " << minWeightAftSboxFor << " + " << minWeightBefSboxFor  << " = " << minWeightFor + Weight3RFinal2RFor[FourCoreInd] << endl;
    Min4RoundFor = min(Min4RoundFor, minWeightFor + Weight3RFinal2RFor[FourCoreInd]);
    his4RFor[(int)round(minWeightFor + Weight3RFinal2RFor[FourCoreInd])]++;
    fou << "--------------------1R-AfterSbox--------------------" << endl;
    display(AfterSbox3RFinalR1For[FourCoreInd], fou);
    fou << "--------------------1R-BeforeSbox--------------------" << endl;
    display(BeforeSbox3RFinalR1For[FourCoreInd],fou);
    fou << "--------------------2R-AfterSbox--------------------" << endl;
    display(AfterSbox3RFinalR2For[FourCoreInd], fou);
    fou << "--------------------2R-BeforeSbox--------------------" << endl;
    display(BeforeSbox3RFinalR2For[FourCoreInd],fou);
    fou << "--------------------3R-AfterSbox--------------------" << endl;
    display(AfterSboxFor, fou);
    fou << "--------------------3R-BeforeNewMat--------------------" << endl;
    display(BeforeNewMatFor, fou);
    fou << "--------------------3R-AfterNewMat--------------------" << endl;
    display(AfterNewMatFor, fou);
    fou << "--------------------3R-BeforeSbox--------------------" << endl;
    display(BeforeSboxFor, fou);
}

void TrailSeedIterator::display(bitset<Size> arr, ofstream& fou) {
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < (int)LANESIZE; i++) {
            for (int k = 0; k < 4; k++) {
                if (arr[i * (int)PLANESIZE + 4* j + k] == 0) {
                    fou << ".";
                } else {
                    fou << "1";
                }      
            }
            fou << " ";
        }
        fou << endl;
    }
}

void TrailSeedIterator::display(bitset<Size> arr) {
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < (int)LANESIZE; i++) {
            for (int k = 0; k < 4; k++) {
                if (arr[i * (int)PLANESIZE + 4 * j + k] == 0) {
                    cout << ".";
                } else {
                    cout << "1";
                }      
            }
            cout << " ";
        }
        cout << endl;
    }
}

void TrailSeedIterator::makeHisto() {
	his[(int)round(minAfterSboxWeight)][(int)round(minBeforeSboxWeight)]++;
	hisss[(int)round(minWeight)]++;
}

void TrailSeedIterator::copyStateBackward(int aNumberOfRounds) {
    if (aNumberOfRounds == 3) {
        Weight3RFinal2RBack.push_back(minBeforeSboxWeight + minWeightBefSboxBack);
        Weight3RFinalMinAftSBoxBack.push_back(minWeightAftSboxBack);
        AfterSbox3RFinalR1Back.push_back(AfterSboxBack);
        BeforeSbox3RFinalR1Back.push_back(BeforeSboxBack);
	    AfterSbox3RFinalR2Back.push_back(AfterSbox);
        BeforeSbox3RFinalR2Back.push_back(BeforeSbox);
	}
	else if (aNumberOfRounds == 4) {
		
	}
}

void TrailSeedIterator::copyStateForward(int aNumberOfRounds) {
    if (aNumberOfRounds == 3) {
        Weight3RFinal2RFor.push_back(minAfterSboxWeight + minWeightAftSboxFor);
        Weight3RFinalMinBefSBoxFor.push_back(minWeightBefSboxFor);
        AfterSbox3RFinalR1For.push_back(AfterSbox);
        BeforeSbox3RFinalR1For.push_back(BeforeSbox);
	    AfterSbox3RFinalR2For.push_back(AfterSboxFor);
        BeforeSbox3RFinalR2For.push_back(BeforeSboxFor);
	}
	else if (aNumberOfRounds == 4) {

	}
}


