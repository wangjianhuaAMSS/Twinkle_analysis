//#pragma once
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <stdint.h>
#include <algorithm>
#include <vector> 
#include "../include/twinkle.h"
#include "../include/myTrailSeedIterator.h"

using namespace std;

void TrailSeedIterator::Four_RoundTrailCore() {
    NumberOfRounds = 4;
	/* Backward extension */
	for (unsigned int i = 0; i < Weight3RFinal2RBack.size(); i++) {
		maxBudgetBackward = MaxCost4Round - Weight3RFinal2RBack[i] - Min1Round;
	    TempAftSbox = AfterSbox3RFinalR1Back[i];
        TempAftSboxWeight = Weight3RFinalMinAftSBoxBack[i];
        MaxCost2RoundBack = MaxCost4Round - Weight3RFinal2RBack[i];
        FourCoreInd = i;
	    BackwardExtension(); 
	}

	/* Forward extension */
	for (unsigned int i = 0; i < Weight3RFinal2RFor.size(); i++) {
		maxBudgetForward = MaxCost4Round - Weight3RFinal2RFor[i] - Min1Round;
	    TempBefSbox = BeforeSbox3RFinalR2For[i];
        TempBefSboxWeight = Weight3RFinalMinBefSBoxFor[i];
        MaxCost2RoundFor = MaxCost4Round - Weight3RFinal2RFor[i]; 
        FourCoreInd = i;
        ForwardExtension();
        
	}	
}