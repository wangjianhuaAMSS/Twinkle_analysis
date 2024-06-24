#ifndef _SEQUENTIALENCODING2_H_
#define _SEQUENTIALENCODING2_H_


void Generate_SequentialEncoding_Clauses2(vector<int> x, int k, vector<vector<int>> &s, int &BeginIndex, ofstream &fClauses) {
    if (k > 0) {//x.size() >=2
        for (int i = 0; i < (int)x.size() - 1; i++) {
            vector<int> tmp;
            s.push_back(tmp);
            for (int j = 0; j < k; j++) {
                s[i].push_back(BeginIndex);
                BeginIndex++;
            }
        }
        fClauses << "-" << x[0] << " " << s[0][0]  << " 0" << endl;
        fClauses << x[0] << " -" << s[0][0]  << " 0" << endl;
        for (int j = 1; j < k; j++) {
            fClauses << "-" << s[0][j] << " 0" << endl;
        }
        for (int i = 1; i < (int)x.size() - 1; i++) {
            fClauses << "-" << x[i] << " " << s[i][0] << " 0" << endl;
            fClauses << "-" << s[i-1][0] << " " << s[i][0] << " 0" << endl;
            fClauses << x[i] << " " << s[i-1][0] << " -" << s[i][0] << " 0" << endl;
            for (int j = 1; j < k; j++) {
                fClauses << x[i] << " " << s[i-1][j] << " -" << s[i][j] << " 0" << endl;
                fClauses << "-" << s[i-1][j] << " " << s[i][j] << " 0" << endl;
                fClauses << s[i-1][j - 1] << " -" << s[i][j] << " 0" << endl;
                fClauses << "-" << s[i - 1][j - 1] << " -" << x[i] << " " << s[i][j] << " 0" << endl;
            }
            fClauses << "-" << x[i] << " -" << s[i-1][k-1] << " 0" << endl;
        }
        fClauses << "-" << x[x.size() - 1] << " -" << s[x.size() - 2][k-1] << " 0" << endl;
    }
    else {
        for (int i = 0; i < (int)x.size(); i++) {
            fClauses << "-" << x[i] << " 0" << endl;
        }
    }
}

int Count_SequentialEncoding_Clauses2(int NumX, int k) {
    if (k > 0) {
        return(4 * k * NumX - 7 * k + 2);
    }
    else {
        return(NumX);
    }
}

int Count_SequentialEncoding_Add_Dummy_Vars2(int NumX, int k) {
    if (k > 0) {
        return(k * (NumX - 1));
    }
    else {
        return(0);
    }
}
#endif