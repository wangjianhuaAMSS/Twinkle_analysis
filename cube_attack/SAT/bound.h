#ifndef _BOUND_H_
#define _BOUND_H_
bool if_compact_bounds(int unsol_n1, int sol_n2) {
    if (unsol_n1 == sol_n2 - 1)
        return(true);
    else 
        return(false);
}

int checkbound_update(int unsol_n1, int sol_n2, int mind) {
    if (unsol_n1 < 0) {//unsol_n1 undefined
        return(0);
    }
    else if (sol_n2 > 10000) { //sol_n2 undefined
        return(unsol_n1 + mind);
    }
    else {
        return(unsol_n1 + (sol_n2 - unsol_n1) / 2);
    }
}
#endif