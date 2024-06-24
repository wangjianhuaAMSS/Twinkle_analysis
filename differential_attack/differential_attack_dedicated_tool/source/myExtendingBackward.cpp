//#pragma once
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <stdint.h>
#include <algorithm>
#include <vector> 
#include "../include/myTrailSeedIterator.h"

using namespace std;

void TrailSeedIterator::BackwardExtension() {
	BeforeSboxBack.reset();
    BeforeNewMatBack.reset();
	AfterNewMatBack.reset();
	AfterSboxBack.reset();
    BeforeSboxTempFixedBack.reset();
    BeforeNewMatTempFixedBack.reset();
    AfterNewMatTempFixedBack.reset();
    AfterSboxTempFixedBack.reset();
	pos.clear();
    incompatibleSbox.reset();
    initialFixedBack();
    lowerWeightBack = TempAftSboxWeight;
    lowerWeightBefSboxBack = TempAftSboxWeight;
    minWeightAftSboxBack = 0;
    minWeightBefSboxBack = 0;
    minWeightBack = 0;
	if (initializationBack()) {
        while (nextBack() == true) {
	    }
    }	
}


bool TrailSeedIterator::initializationBack() {
    for (int i = 0; i < (int)Size; i++) {
        if (AfterNewMatFixedBack[i] == 0) {
            twoDimCd aP{i % (int)PLANESIZE, i /(int)PLANESIZE};
            if (ConsistantBack(aP, 1, 1)) {
                PushPoints(pos, aP);
                numz = pos.size();
                numx = pos[numz - 1].xs.size();
                return true;
            } else if (ConsistantBack(aP, 2, 0)){
            } else {
                return false;
            }
        } 
    }
    return(false);
}

void TrailSeedIterator::initialFixedBack() {
    SboxInverseFixedBack(BeforeSboxFixedBack, TempAftSbox, incompatibleSbox, incompatibleSboxNum);
    LR2InversePiFixedBack(AfterNewMatFixedBack, BeforeSboxFixedBack);
    NewMatInverseFixedBack(BeforeNewMatFixedBack, AfterNewMatFixedBack);
    PiInverseLR1InverseFixedBack(AfterSboxFixedBack, BeforeNewMatFixedBack);
}

bool TrailSeedIterator::toChildBack() {
    int currentIndex = (int)pos[numz-1].xs[numx - 1] + PLANESIZE * (int)pos[numz - 1].z;
    for (int i = currentIndex + 1; i < Size; i++) {
        if (AfterNewMatFixedBack[i] == 0) {
            twoDimCd aP{i % (int)PLANESIZE, i /(int)PLANESIZE};
            if (ConsistantBack(aP, 1, 1)) {
                PushPoints(pos, aP);
                numz = pos.size();
                numx = pos[numz - 1].xs.size();
                return true;
            } else if (ConsistantBack(aP, 2, 0)){
            } else {
                for (int j = i; j >= currentIndex + 1; j--) { 
                    if (AfterNewMatFixedBack[j] == 0) {
                        ConsistantBack(twoDimCd{j % (int)PLANESIZE, j /(int)PLANESIZE}, 0, 0);
                    }
                }
                return false;
            }
        }   
    }
    for (int i = Size - 1; i >= currentIndex + 1 ; i--) {
        if (AfterNewMatFixedBack[i] == 0) {
            twoDimCd aP{i % (int)PLANESIZE, i /(int)PLANESIZE};
            ConsistantBack(aP, 0, 0);
        }   
    }
	return false;
}

bool TrailSeedIterator::toSiblingBack() {
    int currentIndex = (int)pos[numz-1].xs[numx - 1] + PLANESIZE * (int)pos[numz - 1].z;
    twoDimCd currentPoint{pos[numz-1].xs[numx-1], pos[numz-1].z};
    if (currentIndex == Size - 1) {
        return false;
    }
	if (ConsistantBack(currentPoint, 2, 0)) { 
        for (int i = currentIndex + 1; i < Size; i++) {
            if (AfterNewMatFixedBack[i] == 0) {
                if (ConsistantBack(twoDimCd{i % (int)PLANESIZE, i /(int)PLANESIZE}, 1, 1)) { //check 1
                    PopPoints(pos);
                    PushPoints(pos, twoDimCd{i % (int)PLANESIZE, i /(int)PLANESIZE});
                    numz = pos.size();
                    numx = pos[numz - 1].xs.size();
                    return true;
                } else if (ConsistantBack(twoDimCd{i % (int)PLANESIZE, i /(int)PLANESIZE}, 2, 0)){ //check 0
                    
                } else {
                    for (int j = i; j >= currentIndex + 1; j--) {
                        if (AfterNewMatFixedBack[j] == 0) {
                            ConsistantBack(twoDimCd{j % (int)PLANESIZE, j /(int)PLANESIZE}, 0, 0);
                        }
                    }
                    ConsistantBack(currentPoint, 2, 1); 
                    return false;
                }
            }
        }
        for (int i = Size - 1; i >= currentIndex + 1 ; i--) {
            if (AfterNewMatFixedBack[i] == 0) {
                twoDimCd aP{i % (int)PLANESIZE, i /(int)PLANESIZE};
                ConsistantBack(aP, 0, 0);
            }   
        }  
    } 
	ConsistantBack(currentPoint, 2, 1);   
	return false;
}

bool TrailSeedIterator::toParBack() {
    if (numz == 1 && numx == 1) {
        return false;
    }
    int CurrentIndex = (int)pos[numz-1].xs[numx-1] + PLANESIZE * (int)pos[numz-1].z;
	if (ConsistantBack(twoDimCd{pos[numz-1].xs[numx-1], pos[numz-1].z}, 0, 1)) {
        PopPoints(pos);
        numz = pos.size();
        numx = pos[numz - 1].xs.size(); 
        int newIndex = (int)pos[numz-1].xs[numx-1] + PLANESIZE * (int)pos[numz-1].z;
        for (int i = CurrentIndex - 1; i > newIndex; i--) {
            if (AfterNewMatFixedBack[i] == 0) {
                ConsistantBack(twoDimCd{i % (int)PLANESIZE, i / (int)PLANESIZE}, 0, 0);
            }
        }       
		return true;
	} else {
        return false;
    }
}

bool TrailSeedIterator::nextBack() {
	if (toChildBack() == false) {
		do {
			if (toSiblingBack() == true) {
				return true;
			}
			else if (toParBack() == false) {
				return false;
			}
		} while (true);
	}
	else {
		return true;
	}
}


void TrailSeedIterator::updateOperateOnNewPointsBack(twoDimCd aP, int op, bool value) { //op: 0:del, 1:add, 2:modify //if modify, flip
    if (op == 0) {//del
        PiInverseLR2Fixed(BeforeSboxTempFixedBack, aP, false);
        NewMatInversePiInverseLR1InverseFixed(AfterSboxTempFixedBack, AfterNewMatFixedBack, aP, false);
        if (value) {
            AfterNewMatBack.flip((int)aP.x + (int)aP.z * PLANESIZE);
            PiInverseLR2(BeforeSboxBack, aP);
            NewMatInversePiInverseLR1Inverse(AfterSboxBack, aP);
        }  
    } else if (op == 1) {//add
        PiInverseLR2Fixed(BeforeSboxTempFixedBack, aP, true);
        NewMatInversePiInverseLR1InverseFixed(AfterSboxTempFixedBack, AfterNewMatFixedBack, aP, true);
        if (value) {
            AfterNewMatBack.flip((int)aP.x + (int)aP.z * PLANESIZE);
            PiInverseLR2(BeforeSboxBack, aP);
            NewMatInversePiInverseLR1Inverse(AfterSboxBack, aP);
        }
    } else {
        PiInverseLR2(BeforeSboxBack, aP);
        NewMatInversePiInverseLR1Inverse(AfterSboxBack, aP);
        AfterNewMatBack.flip((int)aP.x + (int)aP.z * PLANESIZE);
    }
}


bool TrailSeedIterator::ConsistantBack(twoDimCd aP, int op, int value) { 
    vector<uint8_t> inputsOld, outputsOld, inputsFixedOld;
    vector<uint8_t> outputsOldL, outputsOldM, outputsFixedOld;
    generateLowerBoundBeforeSboxesBack(aP, inputsOld, inputsFixedOld, outputsOld);
    generateLowerBoundRevAfterSboxesBack(aP, outputsOldL, outputsFixedOld); 
    generateMinRevAfterSboxesBack(aP, outputsOldM);

    updateOperateOnNewPointsBack(aP, op, value);
    
    vector<uint8_t> inputs, outputs, inputsFixed;
    vector<uint8_t> outputsL, outputsM, outputsFixed;
    //forward
    generateLowerBoundBeforeSboxesBack(aP, inputs, inputsFixed, outputs);
    //backward
    generateLowerBoundRevAfterSboxesBack(aP, outputsL, outputsFixed);
    generateMinRevAfterSboxesBack(aP, outputsM);
    
    //forward weight change
    WEIGHTTYPE wtBefL = updateLowerBoundWeightBeforeSboxBack(outputs, inputs, inputsFixed, inputsOld, inputsFixedOld);
    //backward weight change
    WEIGHTTYPE wtAftL = updateLowerBoundWeightRevAfterSboxBack(outputsOldL, outputsFixedOld, outputsL, outputsFixed);
    WEIGHTTYPE wtAftM = updateMinWeightRevAfterSboxBack(outputsM, outputsOldM);
    //update weight
    lowerWeightBack += wtAftL + wtBefL;
    lowerWeightBefSboxBack += wtBefL;
    minWeightAftSboxBack += wtAftM;
    minWeightBack += wtAftM;
    
    if (isSubCompatibleBack(aP, inputs[0],  outputs[0], inputsFixed[0])) {
        if(isCompatibleBack(aP, inputs, inputsOld, outputs)) { //update minweight at the same time 
            if (op == 1) {
                if (NumberOfRounds == 3) {
                    mtx.lock();
                    display3RCoreBack(Mf3RBack);
                    mtx.unlock();
                    copyStateBackward(NumberOfRounds);
                } else if (NumberOfRounds == 4) {
                    mtx.lock();
                    display4RCoreBack(Mf4RBack);
                    mtx.unlock();
                    copyStateBackward(NumberOfRounds);   
                }
            }
        }
        if (lowerWeightBefSboxBack <= maxBudgetBackward && lowerWeightBack <= MaxCost2RoundBack) { 
            return(true); 
        }
        else {
            return false;
        }    
    } else {
        int newx = (int)aP.x  * PiInverse % PLANESIZE;
	    int newz = ((int)aP.z + (LANESIZE - LROffset2[newx])) % LANESIZE;
        if (incompatibleSbox[newz * 4 + newx / 4] == 0) {
            incompatibleSbox[newz * 4 + newx / 4] = 1;
            minWeightBefSboxBack-= SboxDiffOfIvCLowerBound[outputs[0]][(inputsOld[0] << 4) | 0xf];
            minWeightBack -= SboxDiffOfIvCLowerBound[outputs[0]][(inputsOld[0] << 4) | 0xf];
            incompatibleSboxNum++;
        }
        return(false);
    }             
    return(false);
	
}


bool TrailSeedIterator::isCompatibleBack(twoDimCd aP, vector<uint8_t> inputs, vector<uint8_t> inputsOld, vector<uint8_t> outputs) { //inputs' size = 1
    int newx = (int)aP.x  * PiInverse % PLANESIZE;
	int newz = ((int)aP.z + (LANESIZE - LROffset2[newx])) % LANESIZE; 
    if (SboxDiffOfIvCLowerBound[outputs[0]][(inputs[0] << 4) | 0xf] == 255) {
        if (incompatibleSbox[newz * 4 + newx / 4] == 0) {
            incompatibleSbox[newz * 4 + newx / 4] = 1; 
            minWeightBefSboxBack -= SboxDiffOfIvCLowerBound[outputs[0]][(inputsOld[0] << 4) | 0xf];
            minWeightBack -= SboxDiffOfIvCLowerBound[outputs[0]][(inputsOld[0] << 4) | 0xf];
            incompatibleSboxNum++;
        }  
        return false;
    } else {
        if (incompatibleSbox[newz * 4 + newx / 4] == 1){        
            incompatibleSbox[newz * 4 + newx / 4] = 0;
            minWeightBefSboxBack += SboxDiffOfIvCLowerBound[outputs[0]][(inputs[0] << 4) | 0xf];
            minWeightBack += SboxDiffOfIvCLowerBound[outputs[0]][(inputs[0] << 4) | 0xf];
            incompatibleSboxNum--;  
        } else {
            minWeightBefSboxBack += updateMinWeightBeforeSboxBack(outputs, inputs, inputsOld);
            minWeightBack += updateMinWeightBeforeSboxBack(outputs, inputs, inputsOld);
        }
        if (incompatibleSboxNum == 0 && minWeightBefSboxBack <= maxBudgetBackward && minWeightBack <= MaxCost2RoundBack) {
            return(true);
        }
        else {
            return false;
        }
    } 
}

bool TrailSeedIterator::isSubCompatibleBack(twoDimCd aP, uint8_t input, uint8_t output, uint8_t inputFixed) {
    if (SboxDiffOfIvCLowerBound[output][((input & inputFixed) << 4) | inputFixed] == 255) {
        return false;
    } else {
        return true;
    }
}

void TrailSeedIterator::generateLowerBoundBeforeSboxesBack(twoDimCd aP, vector<uint8_t> &inputs, vector<uint8_t> &inputsFixed, vector<uint8_t> &outputs) {
    inputs.clear();
    outputs.clear();
    inputsFixed.clear();
    int newx = (int)aP.x * PiInverse % PLANESIZE;
    int newz = ((int)aP.z + (LANESIZE - LROffset2[newx])) % LANESIZE;
    uint8_t  input, output, inputFixed;
    SboxDiff2Side(inputFixed, input, output, twoDimCd{newx, newz}, BeforeSboxFixedBack | BeforeSboxTempFixedBack, BeforeSboxBack, TempAftSbox);
    inputs.push_back(input);
    inputsFixed.push_back(inputFixed);
    outputs.push_back(output);
}



void TrailSeedIterator::generateLowerBoundRevAfterSboxesBack(twoDimCd aP, vector<uint8_t> &outputs, vector<uint8_t> &outputsFixed) {
    outputs.clear();
    outputsFixed.clear();
    int maxFixedIn = PLANESIZE - 1;
    for (int i = (int)aP.x + 1; i < (int)PLANESIZE; i++) {
        if (AfterNewMatFixedBack[i + (int)aP.z * PLANESIZE]  == 0) {
            maxFixedIn = i - 1;
            break;
        }
    }
    if (maxFixedIn == (int)PLANESIZE - 1) {
        for (unsigned int i = maxFixedIn - NewMatInverseFixedHd + 1; i < PLANESIZE + NewMatInverseFixedL; i++) {
            int newx = i * PiInverse % PLANESIZE;
            int newz = ((int)aP.z + LROffset1[newx]) % LANESIZE;
            uint8_t output, outputFixed;
            SboxDiff1Side(outputFixed, output, twoDimCd{newx, newz}, AfterSboxTempFixedBack | AfterSboxFixedBack, AfterSboxBack);
            outputs.push_back(output);
            outputsFixed.push_back(outputFixed);
        }
    }
    for (int i = max(NewMatInverseFixedL, (int)aP.x - NewMatInverseFixedHd); i <= maxFixedIn - NewMatInverseFixedHd; i++) {   
        int newx = i * PiInverse % PLANESIZE;
        int newz = ((int)aP.z + LROffset1[newx]) % LANESIZE;
        uint8_t output, outputFixed;
        SboxDiff1Side(outputFixed, output, twoDimCd{newx, newz}, AfterSboxTempFixedBack | AfterSboxFixedBack, AfterSboxBack);
        outputs.push_back(output);
        outputsFixed.push_back(outputFixed);
    }
}


void TrailSeedIterator::generateMinRevAfterSboxesBack(twoDimCd aP, vector<uint8_t> &outputs) {
    outputs.clear();
    for (int i = 0; i < NewMatInverseOffset.size(); i++) {
        int newx = ((int)aP.x + NewMatInverseOffset[i]) * PiInverse % PLANESIZE;
        int newz = ((int)aP.z + LROffset1[newx]) % LANESIZE;
        uint8_t output = generateSbox(twoDimCd{newx, newz}, AfterSboxBack);
        outputs.push_back(output);
    }
}

WEIGHTTYPE TrailSeedIterator::updateLowerBoundWeightBeforeSboxBack(vector<uint8_t> outputs, vector<uint8_t> inputs, vector<uint8_t> inputsFixed, vector<uint8_t> inputsOld, vector<uint8_t> inputsFixedOld) {
    WEIGHTTYPE wt = 0;
    for (size_t i = 0; i < outputs.size(); i++) {
        wt += SboxDiffOfIvCLowerBound[outputs[i]][((inputs[i]&inputsFixed[i]) << 4) | inputsFixed[i]] - SboxDiffOfIvCLowerBound[outputs[i]][((inputsOld[i] & inputsFixedOld[i]) << 4) | inputsFixedOld[i]];
    }
    return(wt); 
}

WEIGHTTYPE TrailSeedIterator::updateLowerBoundWeightRevAfterSboxBack(vector<uint8_t> outputsOld, vector<uint8_t> outputsFixedOld, vector<uint8_t> outputs, vector<uint8_t> outputsFixed) { //op: true: add, false: del
    WEIGHTTYPE wt = 0;
    for (size_t i = 0; i < outputsOld.size(); i++) {
        wt+= SboxDiffOLowerBound[((outputs[i] & outputsFixed[i]) << 4) | outputsFixed[i]] - SboxDiffOLowerBound[((outputsOld[i] & outputsFixedOld[i]) << 4) | outputsFixedOld[i]];
    }
    return(wt); 
}

WEIGHTTYPE TrailSeedIterator::updateMinWeightBeforeSboxBack(vector<uint8_t> outputs, vector<uint8_t> inputs, vector<uint8_t> inputsOld) {
    WEIGHTTYPE wt = 0;
    for (size_t i = 0; i < outputs.size(); i++) {
        wt += SboxDiffOfIvCLowerBound[outputs[i]][(inputs[i] << 4) | 0xf] - SboxDiffOfIvCLowerBound[outputs[i]][(inputsOld[i] << 4) | 0xf];
    }
    return(wt); 
}


WEIGHTTYPE TrailSeedIterator::updateMinWeightRevAfterSboxBack(vector<uint8_t> outputs, vector<uint8_t> outputsOld) { 
    WEIGHTTYPE wt = 0;
    for (size_t i = 0; i < outputsOld.size(); i++) {
        wt+= SboxDiffOMin[outputs[i]] - SboxDiffOMin[outputsOld[i]];
    }
    return(wt); 
}



