//#pragma once
#ifndef _TRAILSEEDITERATOR_H_
#define _TRAILSEEDITERATOR_H_
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <algorithm>
#include <functional>
#include <array>
#include <vector> 
#include "twinkle.h"
#include "myglobalVar.h"





class TrailSeedIterator : public Twinkle
{
public:
	TrailSeedIterator(const WEIGHTTYPE aMaxCost2Round, const WEIGHTTYPE aMaxCost4Round);
    TrailSeedIterator(const TrailSeedIterator &T);
public:
    void showPosition(vector<planePoints> p, ofstream& fou);
    void showPosition(vector<planePoints> p);

    
    void SboxDiff2Side(uint8_t &fixed, uint8_t &input, uint8_t &output, twoDimCd aP, bitset<Size> fixedAll, bitset<Size> inputAll, bitset<Size> outputAll);
    uint8_t generateSbox(twoDimCd aP, bitset<Size> arr); 
    void SboxDiff1Side(uint8_t &fixed, uint8_t &input, twoDimCd aP, bitset<Size> fixedAll, bitset<Size> inputAll);
    

public:
    WEIGHTTYPE Min1Round = 255; //set a maximum value
    WEIGHTTYPE Min2Round = 255; //set a maximum value
    WEIGHTTYPE Min3RoundBack = 255; //set a maximum value
    WEIGHTTYPE Min3RoundFor = 255; //set a maximum value
    WEIGHTTYPE Min4RoundBack = 255; //set a maximum value
    WEIGHTTYPE Min4RoundFor = 255; //set a maximum value

    ofstream f2R;
    ofstream f3RBack;
    ofstream f3RFor;
    ofstream f4RBack;
    ofstream f4RFor;

public:

	int nz;
	int nx;
	vector<planePoints> position;
    bitset<Size> AfterNewMatTempFixed;
    bitset<Size> BeforeNewMatTempFixed;
    bitset<Size> AfterSboxTempFixed;
    bitset<Size> BeforeSboxTempFixed;
 
	bitset<TWINKLESIZE> AfterSbox;
	bitset<TWINKLESIZE> BeforeNewMat;
	bitset<TWINKLESIZE> AfterNewMat;
	bitset<TWINKLESIZE> BeforeSbox;


	WEIGHTTYPE MaxCost2Round;


	int his2R[100] = {0};

	int his3RBack[100] = {0};
	int his3RFor[100] = {0};
	int his4RBack[100] = {0};
	int his4RFor[100] = {0};
	int his[40][40] = { {0} };
	int hisss[50] = {0};

    WEIGHTTYPE MaxCost2RoundBack;

    WEIGHTTYPE MaxCost2RoundFor;


	WEIGHTTYPE MaxCost3Round;

	WEIGHTTYPE MaxCost4Round;

	WEIGHTTYPE minWeight;
	WEIGHTTYPE lowerBoundWeight;
    WEIGHTTYPE minBeforeSboxWeight;
    WEIGHTTYPE minAfterSboxWeight;
public:

	void initialization();

    bool Consistant(twoDimCd aP, int op, int value);

    void updateOperateOnNewPoints(twoDimCd aP, int op, bool value); 

	bool next();

	bool toChild();

	bool toSibling();

	bool toPar();

    bool nextForThread();
    bool toparForThread();
    bool toSiblingForThread();
    
    bool nextWithChildFlag(bool flag); 

    


    twoDimCd InitPos;

	bool isCanonical();
	
    void generateLowerBoundBeforeSboxes(twoDimCd aP, vector<uint8_t> &inputs, vector<uint8_t> &inputsFixed);
    void generateLowerBoundRevAfterSboxes(twoDimCd aP, vector<uint8_t> &outputs, vector<uint8_t> &outputsFixed);
    WEIGHTTYPE updateLowerBoundWeightBeforeSbox(vector<uint8_t> inputs, vector<uint8_t> inputsFixed, vector<uint8_t> inputsOld, vector<uint8_t> inputsFixedOld);
    WEIGHTTYPE updateLowerBoundWeightRevAfterSbox(vector<uint8_t> outputsOld, vector<uint8_t> outputsFixedOld, vector<uint8_t> outputs, vector<uint8_t> outputsFixed);

    void generateMinBeforeSboxes(twoDimCd aP, vector<uint8_t> &inputs);
	void generateMinRevAfterSboxes(twoDimCd aP, vector<uint8_t> &outputs);
    WEIGHTTYPE updateMinWeightBeforeSbox(vector<uint8_t> inputs, vector<uint8_t> inputsOld); 
    WEIGHTTYPE updateMinWeightRevAfterSbox(vector<uint8_t> outputs, vector<uint8_t> outputsOld);

	void makeHisto();

	void hi4(ofstream& fou, int *his4R);

	void hi3(ofstream& fou, int *his3R);

	void hi2(ofstream& fou);

	void hi22(ofstream& fou);


    void display(bitset<Size> arr, ofstream& fou);
    void display(bitset<Size> arr);


    void display2RCore(ofstream& fou);
	void display2RCore();
    void display3RCoreBack(ofstream& fou);
    void display3RCoreBack();
    void display3RCoreFor(ofstream& fou);
    void display3RCoreFor();
    void display4RCoreBack(ofstream& fou);
    void display4RCoreFor(ofstream& fou);


public:

public:
	void Three_RoundTrailCore();
	void Four_RoundTrailCore();


public:

	WEIGHTTYPE maxBudgetBackward;

    int numz;
    int numx;

	vector<planePoints> pos;

    bitset<int(Size/4)> incompatibleSbox;

    int incompatibleSboxNum;


	bitset<Size> BeforeNewMatBack;

	bitset<Size> AfterNewMatBack;

	bitset<Size> AfterSboxBack;

	bitset<Size> BeforeSboxBack;


    bitset<Size> TempAftSbox;


    bitset<Size> AfterNewMatFixedBack;
    bitset<Size> BeforeNewMatFixedBack;
    bitset<Size> AfterSboxFixedBack;
    bitset<Size> BeforeSboxFixedBack;

    bitset<Size> AfterNewMatTempFixedBack;
    bitset<Size> BeforeNewMatTempFixedBack;
    bitset<Size> AfterSboxTempFixedBack;
    bitset<Size> BeforeSboxTempFixedBack;

    WEIGHTTYPE lowerWeightBack;
    WEIGHTTYPE minWeightBack;
    WEIGHTTYPE lowerWeightBefSboxBack;
    WEIGHTTYPE minWeightBefSboxBack;
    WEIGHTTYPE minWeightAftSboxBack;
    WEIGHTTYPE TempAftSboxWeight;

public:
    void BackwardExtension();
    bool initializationBack();
    void initialFixedBack(); 
    bool toChildBack();
    bool toSiblingBack();
	bool toParBack();
    bool nextBack();
    void updateOperateOnNewPointsBack(twoDimCd aP, int op, bool value);
    bool ConsistantBack(twoDimCd aP, int op, int value);
    bool isCompatibleBack(twoDimCd aP, vector<uint8_t> inputs,  vector<uint8_t> inputsOld, vector<uint8_t> outputs);
    bool isSubCompatibleBack(twoDimCd aP, uint8_t input, uint8_t output, uint8_t inputsFixed);
    WEIGHTTYPE updateLowerBoundWeightBeforeSboxBack(vector<uint8_t> outputs, vector<uint8_t> inputs, vector<uint8_t> inputsFixed, vector<uint8_t> inputsOld, vector<uint8_t> inputsFixedOld);
    WEIGHTTYPE updateLowerBoundWeightRevAfterSboxBack(vector<uint8_t> outputsOld, vector<uint8_t> outputsFixedOld, vector<uint8_t> outputs, vector<uint8_t> outputsFixed);
    void generateLowerBoundBeforeSboxesBack(twoDimCd aP, vector<uint8_t> &inputs, vector<uint8_t> &inputsFixed, vector<uint8_t> &outputs);
    void generateLowerBoundRevAfterSboxesBack(twoDimCd aP, vector<uint8_t> &outputs, vector<uint8_t> &outputsFixed);    

	WEIGHTTYPE updateMinWeightBeforeSboxBack(vector<uint8_t> outputs, vector<uint8_t> inputs, vector<uint8_t> inputsOld);
    WEIGHTTYPE updateMinWeightRevAfterSboxBack(vector<uint8_t> outputs, vector<uint8_t> outputsOld);
    void generateMinRevAfterSboxesBack(twoDimCd aP, vector<uint8_t> &outputs);
    
public:

	WEIGHTTYPE maxBudgetForward;

	bitset<Size> BeforeNewMatFor;

	bitset<Size> AfterNewMatFor;

	bitset<Size> AfterSboxFor;

	bitset<Size> BeforeSboxFor;

    bitset<Size> TempBefSbox;

    bitset<Size> AfterNewMatFixedFor;
    bitset<Size> BeforeNewMatFixedFor;
    bitset<Size> AfterSboxFixedFor;
    bitset<Size> BeforeSboxFixedFor;

    bitset<Size> AfterNewMatTempFixedFor;
    bitset<Size> BeforeNewMatTempFixedFor;
    bitset<Size> AfterSboxTempFixedFor;
    bitset<Size> BeforeSboxTempFixedFor;

    WEIGHTTYPE lowerWeightFor;
    WEIGHTTYPE minWeightFor;
    WEIGHTTYPE lowerWeightAftSboxFor;
    WEIGHTTYPE minWeightBefSboxFor;
    WEIGHTTYPE minWeightAftSboxFor;
    WEIGHTTYPE TempBefSboxWeight;

public:

    void ForwardExtension();

    bool initializationFor();
    void initialFixedFor(); 

    bool toChildFor();
    bool toSiblingFor();
	bool toParFor();
    bool nextFor();
    void updateOperateOnNewPointsFor(twoDimCd aP, int op, bool value);
    bool ConsistantFor(twoDimCd aP, int op, int value);
    bool isCompatibleFor(twoDimCd aP, vector<uint8_t> inputs,  vector<uint8_t> inputsOld, vector<uint8_t> outputs);
    bool isSubCompatibleFor(twoDimCd aP, uint8_t input, uint8_t output, uint8_t inputsFixed);
    void generateLowerBoundBeforeSboxesFor(twoDimCd aP, vector<uint8_t> &inputs, vector<uint8_t> &inputsFixed);
    void generateLowerBoundRevAfterSboxesFor(twoDimCd aP, vector<uint8_t> &outputs, vector<uint8_t> &outputsFixed, vector<uint8_t> &inputs);
    WEIGHTTYPE updateLowerBoundWeightBeforeSboxFor(vector<uint8_t> inputs, vector<uint8_t> inputsFixed, vector<uint8_t> inputsOld, vector<uint8_t> inputsFixedOld);
    WEIGHTTYPE updateLowerBoundWeightRevAfterSboxFor(vector<uint8_t> inputs, vector<uint8_t> outputsOld, vector<uint8_t> outputsFixedOld, vector<uint8_t> outputs, vector<uint8_t> outputsFixed);
    void generateMinBeforeSboxesFor(twoDimCd aP, vector<uint8_t> &inputs);
    void generateMinRevAfterSboxesFor(twoDimCd aP, vector<uint8_t> &outputs);
    WEIGHTTYPE updateMinWeightBeforeSboxFor(vector<uint8_t> inputs, vector<uint8_t> inputsOld);
    WEIGHTTYPE updateMinWeightRevAfterSboxFor(vector<uint8_t> inputs,vector<uint8_t> outputs, vector<uint8_t> outputsOld);


public:
	void copyStateForward(int aNumberOfRounds);

	void copyStateBackward(int aNumberOfRounds);

	int NumberOfRounds;

	vector<bitset<Size>> BeforeSbox3RFinalR0Back;
    vector<bitset<Size>> BeforeSbox3RFinalR0For;


	vector<bitset<Size>> AfterSbox3RFinalR1Back;
    vector<bitset<Size>> AfterSbox3RFinalR1For;


	vector<bitset<Size>> BeforeSbox3RFinalR1Back;
    vector<bitset<Size>> BeforeSbox3RFinalR1For;


	vector<bitset<Size>> AfterSbox3RFinalR2Back;
    vector<bitset<Size>> AfterSbox3RFinalR2For;


	vector<bitset<Size>> BeforeSbox3RFinalR2Back;
    vector<bitset<Size>> BeforeSbox3RFinalR2For;


    vector<WEIGHTTYPE> Weight3RFinal2RBack;
    vector<WEIGHTTYPE> Weight3RFinalMinAftSBoxBack;
    vector<WEIGHTTYPE> Weight3RFinal2RFor;
    vector<WEIGHTTYPE> Weight3RFinalMinBefSBoxFor;

    int FourCoreInd;

};

#endif