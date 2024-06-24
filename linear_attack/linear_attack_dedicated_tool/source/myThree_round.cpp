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

void TrailSeedIterator::Three_RoundTrailCore() {
	NumberOfRounds = 3;
		
    /* Backward extension */
	maxBudgetBackward = MaxCost2Round - minBeforeSboxWeight;
	TempAftSbox = AfterSbox;
    TempAftSboxWeight = minAfterSboxWeight;
    MaxCost2RoundBack = MaxCost4Round - minBeforeSboxWeight - Min1Round;
	BackwardExtension();		
	
	/* Forward extension */
	maxBudgetForward = MaxCost2Round - minAfterSboxWeight;
	TempBefSbox = BeforeSbox;
    TempBefSboxWeight = minBeforeSboxWeight;
    MaxCost2RoundFor = MaxCost4Round - minAfterSboxWeight - Min1Round;
	ForwardExtension();
}
	