#include "../include/mymain.h"



int main(int argc, char* argv[]) {
	auto start = high_resolution_clock::now();
   
    
    foutHisto4.open("Histo4-r.txt");
	foutHisto3.open("Histo3-r.txt");
	foutHisto2.open("Histo2-r.txt");
	foutHisto2Det.open("Histo2-r_det.txt");

    Mf2R.open("2-round_core.txt");
    Mf3RBack.open("3-round_core_back.txt");
    Mf3RFor.open("3-round_core_for.txt");
    Mf4RBack.open("4-round_core_back.txt");
    Mf4RFor.open("4-round_core_for.txt");


	WEIGHTTYPE maxWeightFourRound = 29;
	WEIGHTTYPE maxWeightTwoRound = maxWeightFourRound / 2;

	

    assignTasks(46, maxWeightTwoRound, maxWeightFourRound, 10, 3);

	/* Computing the compiling time.*/
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<minutes>(stop - start);
	cout << "\n\nExecution time: " << duration.count() << " minutes" << endl;

    foutHisto4.close();
	foutHisto3.close();
	foutHisto2.close();
	foutHisto2Det.close();
    Mf2R.close();
    Mf3RBack.close();
    Mf3RFor.close();
    Mf4RBack.close();
    Mf4RFor.close();
	return 0;
}


