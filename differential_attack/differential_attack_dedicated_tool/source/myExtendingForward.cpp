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

void TrailSeedIterator::ForwardExtension() {
	BeforeSboxFor.reset();
    BeforeNewMatFor.reset();
	AfterNewMatFor.reset();
	AfterSboxFor.reset();
    BeforeSboxTempFixedFor.reset();
    BeforeNewMatTempFixedFor.reset();
	AfterNewMatTempFixedFor.reset();
	AfterSboxTempFixedFor.reset();
	pos.clear();
    incompatibleSbox.reset();
    initialFixedFor();
    lowerWeightFor = TempBefSboxWeight;
    lowerWeightAftSboxFor = TempBefSboxWeight;
    minWeightBefSboxFor = 0;
    minWeightAftSboxFor = 0;
    minWeightFor = 0;
	if (initializationFor()) {
        while (nextFor() == true) {

	    }
    }	
}


bool TrailSeedIterator::initializationFor() {
    //Add points in BeforeNewMatFor
    for (int i = 0; i < (int)Size; i++) {
        if (BeforeNewMatFixedFor[i] == 0) {
            //cout << i << endl;
            twoDimCd aP{i % (int)PLANESIZE, i /(int)PLANESIZE};
            if (ConsistantFor(aP, 1, 1)) {
                PushPoints(pos, aP);
                numz = pos.size();
                numx = pos[numz - 1].xs.size();
                return true;
            } else if (ConsistantFor(aP, 2, 0)){
            } else {
                return false;
            }
        } 
    }
    return(false);
}

void TrailSeedIterator::initialFixedFor() {
    SboxFixedFor(AfterSboxFixedFor, TempBefSbox, incompatibleSbox, incompatibleSboxNum);
    LR1PiFixedFor(BeforeNewMatFixedFor, AfterSboxFixedFor);
    NewMatFixedFor(AfterNewMatFixedFor, BeforeNewMatFixedFor);
    PiInverseLR2FixedFor(BeforeSboxFixedFor, AfterNewMatFixedFor);
}

bool TrailSeedIterator::toChildFor() {
    int currentIndex = (int)pos[numz-1].xs[numx - 1] + PLANESIZE * (int)pos[numz - 1].z;
    for (int i = currentIndex + 1; i < Size; i++) {
        if (BeforeNewMatFixedFor[i] == 0) {
            twoDimCd aP{i % (int)PLANESIZE, i /(int)PLANESIZE};
            if (ConsistantFor(aP, 1, 1)) {
                PushPoints(pos, aP);
                numz = pos.size();
                numx = pos[numz - 1].xs.size();
                return true;
            } else if (ConsistantFor(aP, 2, 0)){
            } else {
                for (int j = i; j >= currentIndex + 1; j--) {
                    if (BeforeNewMatFixedFor[j] == 0) {
                        ConsistantFor(twoDimCd{j % (int)PLANESIZE, j /(int)PLANESIZE}, 0, 0);
                    }
                }
                return false;
            }
        }   
    }
    for (int i = Size - 1; i >= currentIndex + 1 ; i--) {
        if (BeforeNewMatFixedFor[i] == 0) {
            twoDimCd aP{i % (int)PLANESIZE, i /(int)PLANESIZE};
            ConsistantFor(aP, 0, 0);
        }   
    }
	return false;
}

bool TrailSeedIterator::toSiblingFor() {
    int currentIndex = (int)pos[numz-1].xs[numx - 1] + PLANESIZE * (int)pos[numz - 1].z;
    twoDimCd currentPoint{pos[numz-1].xs[numx-1], pos[numz-1].z};
    if (currentIndex == Size - 1) {
        return false;
    }
	if (ConsistantFor(currentPoint, 2, 0)) { // check 0
        for (int i = currentIndex + 1; i < Size; i++) {
            if (BeforeNewMatFixedFor[i] == 0) {
                if (ConsistantFor(twoDimCd{i % (int)PLANESIZE, i /(int)PLANESIZE}, 1, 1)) { //check 1
                    PopPoints(pos);
                    PushPoints(pos, twoDimCd{i % (int)PLANESIZE, i /(int)PLANESIZE});
                    numz = pos.size();
                    numx = pos[numz - 1].xs.size();
                    return true;
                } else if (ConsistantFor(twoDimCd{i % (int)PLANESIZE, i /(int)PLANESIZE}, 2, 0)){ //check 0
                    
                } else {
                    for (int j = i; j >= currentIndex + 1; j--) {
                        if (BeforeNewMatFixedFor[j] == 0) {
                            ConsistantFor(twoDimCd{j % (int)PLANESIZE, j /(int)PLANESIZE}, 0, 0);
                        }
                    }
                    ConsistantFor(currentPoint, 2, 1); //recover to be 1
                    return false;
                }
            }
        }
        for (int i = Size - 1; i >= currentIndex + 1 ; i--) {
            if (BeforeNewMatFixedFor[i] == 0) {
                twoDimCd aP{i % (int)PLANESIZE, i /(int)PLANESIZE};
                ConsistantFor(aP, 0, 0);
            }   
        }  
    } 
	ConsistantFor(currentPoint, 2, 1); //recover to be 1  
	return false;
}

bool TrailSeedIterator::toParFor() {
    if (numz == 1 && numx == 1) {
        return false;
    }
    int CurrentIndex = (int)pos[numz-1].xs[numx-1] + PLANESIZE * (int)pos[numz-1].z;
	if (ConsistantFor(twoDimCd{pos[numz-1].xs[numx-1], pos[numz-1].z}, 0, 1)) {
        PopPoints(pos);
        numz = pos.size();
        numx = pos[numz - 1].xs.size(); 
        int newIndex = (int)pos[numz-1].xs[numx-1] + PLANESIZE * (int)pos[numz-1].z;
        for (int i = CurrentIndex - 1; i > newIndex; i--) {
            if (BeforeNewMatFixedFor[i] == 0) {
                ConsistantFor(twoDimCd{i % (int)PLANESIZE, i / (int)PLANESIZE}, 0, 0);
            }
        }       
		return true;
	} else {
        return false;
    }
}

bool TrailSeedIterator::nextFor() {
	if (toChildFor() == false) {
		do {
			if (toSiblingFor() == true) {
				return true;
			}
			else if (toParFor() == false) {
				return false;
			}
		} while (true);
	}
	else {
		return true;
	}
}


void TrailSeedIterator::updateOperateOnNewPointsFor(twoDimCd aP, int op, bool value) { //op: 0:del, 1:add, 2:modify //if modify, flip
    if (op == 0) {//del
        NewMatPiInverseLR2Fixed(BeforeSboxTempFixedFor, BeforeNewMatFixedFor, aP, false);
        PiInverseLR1InverseFixed(AfterSboxTempFixedFor,  aP, false);
        if (value) {
            BeforeNewMatFor.flip((int)aP.x + (int)aP.z * PLANESIZE);
            NewMatPiInverseLR2(BeforeSboxFor, aP);
            PiInverseLR1Inverse(AfterSboxFor, aP);
        }  
    } else if (op == 1) {//add
        NewMatPiInverseLR2Fixed(BeforeSboxTempFixedFor, BeforeNewMatFixedFor,  aP, true);
        PiInverseLR1InverseFixed(AfterSboxTempFixedFor, aP, true);
        if (value) {
            BeforeNewMatFor.flip((int)aP.x + (int)aP.z * PLANESIZE);
            NewMatPiInverseLR2(BeforeSboxFor, aP);
            PiInverseLR1Inverse(AfterSboxFor, aP);
        }
    } else {
        BeforeNewMatFor.flip((int)aP.x + (int)aP.z * PLANESIZE);
        NewMatPiInverseLR2(BeforeSboxFor, aP);
        PiInverseLR1Inverse(AfterSboxFor, aP);
    }
}


bool TrailSeedIterator::ConsistantFor(twoDimCd aP, int op, int value) { 
    vector<uint8_t> inputsOldL, inputsOldM, inputsFixedOld;
    vector<uint8_t> outputsOldL, outputsOldM, inputsOld, outputsFixedOld;
    //forward
    generateLowerBoundBeforeSboxesFor(aP, inputsOldL, inputsFixedOld);
    generateMinBeforeSboxesFor(aP, inputsOldM);
    //backward
    generateLowerBoundRevAfterSboxesFor(aP, outputsOldL, outputsFixedOld, inputsOld); 
    generateMinRevAfterSboxesFor(aP, outputsOldM);

    //update
    updateOperateOnNewPointsFor(aP, op, value);
    
    vector<uint8_t> inputsL, inputsM, inputsFixed;
    vector<uint8_t> outputsL, outputsM, inputs, outputsFixed;
    //forward
    generateLowerBoundBeforeSboxesFor(aP, inputsL, inputsFixed);
    generateMinBeforeSboxesFor(aP, inputsM);
    //backward
    generateLowerBoundRevAfterSboxesFor(aP, outputsL, outputsFixed, inputs); 
    generateMinRevAfterSboxesFor(aP, outputsM);
    
    //forward weight change
    WEIGHTTYPE wtBefL = updateLowerBoundWeightBeforeSboxFor(inputsL, inputsFixed, inputsOldL, inputsFixedOld);
    WEIGHTTYPE wtBefM = updateMinWeightBeforeSboxFor(inputsM, inputsOldM);
    //backward weight change
    WEIGHTTYPE wtAftL = updateLowerBoundWeightRevAfterSboxFor(inputs, outputsOldL, outputsFixedOld, outputsL, outputsFixed);
    //update weight
    lowerWeightFor += wtAftL + wtBefL;
    lowerWeightAftSboxFor += wtAftL;
    minWeightBefSboxFor += wtBefM;
    minWeightFor += wtBefM;
    
    if (isSubCompatibleFor(aP,  outputsL[0], inputs[0], outputsFixed[0])) {
        if(isCompatibleFor(aP, outputsM, outputsOldM, inputs)) { //update minweight at the same time 
            if (op == 1) {
                if (NumberOfRounds == 3) {
                    mtx.lock();
                    display3RCoreFor(Mf3RFor);
                    mtx.unlock();
                    copyStateForward(NumberOfRounds);
                } else if (NumberOfRounds == 4) {
                    mtx.lock();
                    display4RCoreFor(Mf4RFor);
                    mtx.unlock();
                    copyStateForward(NumberOfRounds);
                }
            }
        } 
        if (lowerWeightAftSboxFor <= maxBudgetForward && lowerWeightFor <= MaxCost2RoundFor) {
            return(true);
        }
        else {
            return(false);
        }        
    } else {
        int newx = (int)aP.x  * PiInverse % PLANESIZE;
	    int newz = ((int)aP.z +  LROffset1[newx]) % LANESIZE; 
        if (incompatibleSbox[newz * 4 + newx / 4] == 0) {
            incompatibleSbox[newz * 4 + newx / 4] = 1;
            minWeightAftSboxFor-= SboxDiffIfOvCLowerBound[inputs[0]][(outputsOldM[0] << 4) | 0xf];
            minWeightFor -= SboxDiffIfOvCLowerBound[inputs[0]][(outputsOldM[0] << 4) | 0xf];
            incompatibleSboxNum++;
        }
        return(false);
    }             
    return(false);
	
}


bool TrailSeedIterator::isCompatibleFor(twoDimCd aP, vector<uint8_t> outputs, vector<uint8_t> outputsOld, vector<uint8_t> inputs) { //inputs' size = 1
    int newx = (int)aP.x  * PiInverse % PLANESIZE;
	int newz = ((int)aP.z +  LROffset1[newx]) % LANESIZE; 
    if (SboxDiffIfOvCLowerBound[inputs[0]][(outputs[0] << 4) | 0xf] == 255) {
        if (incompatibleSbox[newz * 4 + newx / 4] == 0) {
            incompatibleSbox[newz * 4 + newx / 4] = 1; 
            minWeightAftSboxFor -= SboxDiffIfOvCLowerBound[inputs[0]][(outputsOld[0] << 4) | 0xf];
            minWeightFor -= SboxDiffIfOvCLowerBound[inputs[0]][(outputsOld[0] << 4) | 0xf];
            incompatibleSboxNum++;
        }  
        return false;
    } else {
        if (incompatibleSbox[newz * 4 + newx / 4] == 1){        
            incompatibleSbox[newz * 4 + newx / 4] = 0;
            minWeightAftSboxFor += SboxDiffIfOvCLowerBound[inputs[0]][(outputs[0] << 4) | 0xf];
            minWeightFor += SboxDiffIfOvCLowerBound[inputs[0]][(outputs[0] << 4) | 0xf];
            incompatibleSboxNum--;  
        } else {
            minWeightAftSboxFor += updateMinWeightRevAfterSboxFor(inputs, outputs, outputsOld);
            minWeightFor += updateMinWeightRevAfterSboxFor(inputs, outputs, outputsOld);
        }
        if (incompatibleSboxNum == 0 && minWeightAftSboxFor <= maxBudgetForward && minWeightFor <= MaxCost2RoundFor)
            return(true);
        else {
            return false;
        }
    } 
}

bool TrailSeedIterator::isSubCompatibleFor(twoDimCd aP, uint8_t output, uint8_t input, uint8_t outputFixed) {
    if (SboxDiffIfOvCLowerBound[input][((output & outputFixed) << 4) | outputFixed] == 255) {
        return false;
    } else {
        return true;
    }
}


void TrailSeedIterator::generateLowerBoundBeforeSboxesFor(twoDimCd aP, vector<uint8_t> &inputs, vector<uint8_t> &inputsFixed) {
    inputs.clear();
    inputsFixed.clear();
    int maxFixedIn = PLANESIZE - 1;
    for (int i = (int)aP.x + 1; i < (int)PLANESIZE; i++) {
        if (BeforeNewMatFixedFor[i + (int)aP.z * PLANESIZE]  == 0) {
            maxFixedIn = i - 1;
            break;
        }
    }
    if (maxFixedIn == (int)PLANESIZE - 1) {
        for (unsigned int i = maxFixedIn - NewMatFixedHd + 1; i < PLANESIZE + NewMatFixedL; i++) {
            int newx = i * PiInverse % PLANESIZE;
            int newz = ((int)aP.z + (LANESIZE - LROffset2[newx])) % LANESIZE;
            uint8_t input, inputFixed;
            SboxDiff1Side(inputFixed, input, twoDimCd{newx, newz}, BeforeSboxFixedFor | BeforeSboxTempFixedFor, BeforeSboxFor);
            inputs.push_back(input);
            inputsFixed.push_back(inputFixed);
        }
    }
    for (int i = max(NewMatFixedL, (int)aP.x - NewMatFixedHd); i <= maxFixedIn - NewMatFixedHd; i++) {   
        int newx = i * PiInverse % PLANESIZE;
        int newz = ((int)aP.z + (LANESIZE - LROffset2[newx])) % LANESIZE;
        uint8_t input, inputFixed;
        SboxDiff1Side(inputFixed, input, twoDimCd{newx, newz}, BeforeSboxFixedFor | BeforeSboxTempFixedFor, BeforeSboxFor);
        inputs.push_back(input);
        inputsFixed.push_back(inputFixed);
    }
}


void TrailSeedIterator::generateLowerBoundRevAfterSboxesFor(twoDimCd aP, vector<uint8_t> &outputs, vector<uint8_t> &outputsFixed, vector<uint8_t> &inputs) {
    outputs.clear();
    outputsFixed.clear();
    inputs.clear();
     
    int newx = (int)aP.x * PiInverse % PLANESIZE;
    int newz = ((int)aP.z + LROffset1[newx]) % LANESIZE;
    uint8_t input, output, outputFixed;
    SboxDiff2Side(outputFixed, input, output, twoDimCd{newx, newz}, AfterSboxTempFixedFor | AfterSboxFixedFor, TempBefSbox, AfterSboxFor);
    outputs.push_back(output);
    outputsFixed.push_back(outputFixed);
    inputs.push_back(input);
}

void TrailSeedIterator::generateMinBeforeSboxesFor(twoDimCd aP, vector<uint8_t> &inputs) {
    inputs.clear();
    for (int i = 0; i < NewMatOffset.size(); i++) {
        int newx = ((int)aP.x + NewMatOffset[i]) * PiInverse % PLANESIZE;
        int newz = ((int)aP.z + (LANESIZE - LROffset2[newx])) % LANESIZE;
        uint8_t input = generateSbox(twoDimCd{newx, newz}, BeforeSboxFor);
        inputs.push_back(input);
    }
}

void TrailSeedIterator::generateMinRevAfterSboxesFor(twoDimCd aP, vector<uint8_t> &outputs) {
    outputs.clear();
    int newx = (int)aP.x * PiInverse % PLANESIZE;
    int newz = ((int)aP.z + LROffset1[newx]) % LANESIZE;
    uint8_t output = generateSbox(twoDimCd{newx, newz}, AfterSboxFor);
    outputs.push_back(output);
}


WEIGHTTYPE TrailSeedIterator::updateLowerBoundWeightBeforeSboxFor(vector<uint8_t> inputs, vector<uint8_t> inputsFixed, vector<uint8_t> inputsOld, vector<uint8_t> inputsFixedOld) {
    WEIGHTTYPE wt = 0;
    for (size_t i = 0; i < inputs.size(); i++) {
        wt += SboxDiffILowerBound[((inputs[i] & inputsFixed[i]) << 4) | inputsFixed[i]] - SboxDiffILowerBound[((inputsOld[i] & inputsFixedOld[i]) << 4) | inputsFixedOld[i]];
    }
    return(wt); 
}

WEIGHTTYPE TrailSeedIterator::updateLowerBoundWeightRevAfterSboxFor(vector<uint8_t> inputs, vector<uint8_t> outputsOld, vector<uint8_t> outputsFixedOld, vector<uint8_t> outputs, vector<uint8_t> outputsFixed) {
    WEIGHTTYPE wt = 0;
    for (size_t i = 0; i < outputsOld.size(); i++) {
        wt+= SboxDiffIfOvCLowerBound[inputs[i]][((outputs[i] & outputsFixed[i]) << 4) | outputsFixed[i]] - SboxDiffIfOvCLowerBound[inputs[i]][((outputsOld[i] & outputsFixedOld[i]) << 4) | outputsFixedOld[i]];
    }
    return(wt); 
}

WEIGHTTYPE TrailSeedIterator::updateMinWeightBeforeSboxFor(vector<uint8_t> inputs, vector<uint8_t> inputsOld) {
    WEIGHTTYPE wt = 0;
    for (size_t i = 0; i < inputs.size(); i++) {
        wt += SboxDiffIMin[inputs[i]] - SboxDiffIMin[inputsOld[i]];
    }
    return(wt); 
}


WEIGHTTYPE TrailSeedIterator::updateMinWeightRevAfterSboxFor(vector<uint8_t> inputs,vector<uint8_t> outputs, vector<uint8_t> outputsOld) { 
    WEIGHTTYPE wt = 0;
    for (size_t i = 0; i < outputsOld.size(); i++) {
        wt+= SboxDiffIfOvCLowerBound[inputs[i]][(outputs[i] << 4) | 0xf] - SboxDiffIfOvCLowerBound[inputs[i]][(outputsOld[i] << 4) | 0xf];
    }
    return(wt); 
}

