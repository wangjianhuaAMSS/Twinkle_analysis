#ifndef _GENERATEINDEX_H_
#define _GENERATEINDEX_H_


void Generate_Consecutive_Index(vector<int> &inv, int & BeginIndex, int num) {
    for (int i = 0; i < num; i++) {
        inv.push_back(BeginIndex);
        BeginIndex++;
    }
}

#endif