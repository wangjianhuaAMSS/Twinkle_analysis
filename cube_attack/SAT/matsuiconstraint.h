#ifndef _MATSUICONSTRAINT_H_
#define _MATSUICONSTRAINT_H_


void Generate_MatsuiConstraint_le_Clauses(vector<int> x, vector<vector<int>> s, int m, int left, int right, ofstream &fClauses) {
    if ((m > 0) && (m < s[0].size())) {
        int k = s[0].size();
        int NumX = x.size();
        if ((left == 0) && (right < NumX - 1)) {
            for (int i = 1; i <= right; i++) {
                fClauses << "-" << x[i] << " -" << s[i-1][m-1] << " 0" << endl;
            }
        }
        else if ((left > 0) && (right < NumX - 1)) {
            for (int i = 0; i <= k - m - 1; i++) {
                fClauses << s[left - 1][i] << " -" << s[right][i + m] << " 0" << endl; 
            }
        }
        else if ((left > 0) && (right == NumX - 1)) {
            for (int i = 0; i <= k - m - 1; i++) {
                fClauses << s[left - 1][i] << " -" << s[right - 1][i + m] << " 0" << endl; 
                fClauses << s[left - 1][i] << " -" << x[right] << " -" << s[right - 1][i + m - 1] << " 0" << endl;
            }
            fClauses << s[left - 1][k - m] << " -" << x[right] << " -" << s[right - 1][k - 1] << " 0" << endl;
        }
        else {
            fClauses << "-" << s[right - 1][m] << " 0" << endl;
            fClauses << "-" << x[right] << " -" << s[right - 1][m - 1] << " 0" << endl;
        }
    }
    else if (m == 0){
        for (int i = left; i <= right; i++) {
            fClauses << "-" << x[i] << " 0" << endl;
        }
    }
}

int Count_MatsuiConstraint_le_Clauses(int NumX, int  k, int m, int left, int right) {
    int c = 0;
    if ((m > 0) && (m < k)) {
        if ((left == 0) && (right < NumX - 1)) {
            c = right;
        }
        else if ((left > 0) && (right < NumX - 1)) {
            c = k - m;
        }
        else if ((left > 0) && (right == NumX - 1)) {
            c = 2 * k -2 * m + 1;
        }
        else {
            if (m < k)
                c = 2;
            else 
                c = 0;
        }
    }
    else if (m == 0){
        c = right - left + 1;
    }
    return(c);
}
void Generate_MatsuiConstraint_ge_Clauses(vector<int> x, vector<vector<int>> s, int m, int left, int right, ofstream &fClauses) { 
        
    if (m > 0) {
        int k = s[0].size();
        int NumX = x.size();
        if ((left == 0) && (right < NumX - 1)) {
            fClauses << s[right][m - 1] << " 0" << endl;
        }
        else if ((left > 0) && (right < NumX - 1)) {
            for (int i = k - 1; i >= k - m; i--) {
                fClauses << "-" << s[left - 1][i] << " 0" << endl;
            }
            for (int i = k - m - 1; i >= 0; i--) {
                fClauses << "-" << s[left - 1][i] << " " << s[right][i + m] << " 0" << endl; 
            }
            fClauses << s[right][m - 1] << " 0" << endl;
        }
        else if ((left > 0) && (right == NumX - 1)) {
            for (int i = k - 1; i >= k - m; i--) {
                fClauses << "-" << s[left - 1][i] << " 0" << endl;
            }
            for (int i = k - m - 1; i >= 0; i--) {
                if (m > 1)
                    fClauses << "-" << s[left - 1][i] << " " << s[right - 1][i + m - 1] << " 0" << endl; 
                fClauses << "-" << s[left - 1][i] << " " << x[right] << " " << s[right - 1][i + m] << " 0" << endl;
            }
            if (m > 1)
                fClauses << s[right - 1][m - 2] << " 0" << endl;
            fClauses << x[right] << " " << s[right - 1][m - 1] << " 0" << endl;
        }
        else {
            if (m > 1)
                fClauses << s[right - 1][m - 2] << " 0" << endl;
            fClauses << s[right - 1][m - 1] << " " << x[right] << " 0" << endl;
        }
    }
}

int Count_MatsuiConstraint_ge_Clauses(int NumX, int  k, int m, int left, int right) {
    int c;
    if (m > 0) {
        if ((left == 0) && (right < NumX - 1)) {
            c = 1;
        }
        else if ((left > 0) && (right < NumX - 1)) {
            c = k + 1;
        }
        else if ((left > 0) && (right == NumX - 1)) {
            if (m > 1)
                c = 2 * k - m + 2;
            else
                c = k + 1;
        }
        else {
            if (m > 1)
                c = 2;
            else {
                c = 1;
            }
        }
    }
    else {
        c = 0;
    }
    return(c);
}
#endif