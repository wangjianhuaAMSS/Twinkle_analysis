#include "../include/myAssignTasks.h"

int thread_f(const TrailSeedIterator & T) {
    TrailSeedIterator t(T);
    while (t.nextForThread()) {
        if (t.nz == 1 && t.nx <= 5) {
            mtx.lock();
            cout << "Thread position" << endl;
            t.showPosition(t.position);
            mtx.unlock();
        }
    }
    t.Four_RoundTrailCore();
    mtx.lock();
    MINR1 = min(MINR1, t.Min1Round);
    MINR2 = min(MINR2, t.Min2Round);
    MINR3BACK = min(MINR3BACK, t.Min3RoundBack);
    MINR3FOR = min(MINR3FOR, t.Min3RoundFor);
    MINR4BACK = min(MINR4BACK, t.Min4RoundFor);
    MINR4FOR = min(MINR4FOR, t.Min4RoundFor);
    
    for (int i = 0; i < 100; i++) {
        MHis2R[i] += t.his2R[i];
        MHis3RBack[i] += t.his3RBack[i];
        MHis3RFor[i] += t.his3RFor[i];
        MHis4RBack[i] += t.his4RBack[i];
        MHis4RFor[i] += t.his4RFor[i];
    }
    for (int i = 0; i < 40; i++) {
        for (int j = 0; j < 40; j++) {
            MHis[i][j] += t.his[i][j];
        }
    }
    
    mtx.unlock();
    return(0);
}


void assignTasks(int threadNum, int maxWeightTwoRound, int maxWeightFourRound, WEIGHTTYPE HB, WEIGHTTYPE LB) {
    TrailSeedIterator T(maxWeightTwoRound, maxWeightFourRound);
    ThreadPool pool(threadNum);
    std::vector<std::future<int>> res;

    int tasksCount = 0;
	/* Tree search.*/
    bool flag = true;
	while (T.nextWithChildFlag(flag)) {
        if (LB <= T.lowerBoundWeight &&  T.lowerBoundWeight <= HB) {
            //TrailSeedIterator t(T);
            
            std::future<int>fu = pool.enqueue(thread_f, T);
            res.emplace_back(std::move(fu));
            flag = false;
            tasksCount++;
        } else {
            flag = true;
        }
        if (T.nz == 1 && T.nx <= 5) {
            mtx.lock();
            cout << "position" << endl;
            T.showPosition(T.position);
            mtx.unlock();
        }
    }
    T.Four_RoundTrailCore();
    while(1) {
        if (pool.isstop()) {
            mtx.lock();
            T.Min1Round = min(MINR1, T.Min1Round);
            T.Min2Round = min(MINR2, T.Min2Round);
            T.Min3RoundBack = min(MINR3BACK, T.Min3RoundBack);
            T.Min3RoundFor = min(MINR3FOR, T.Min3RoundFor);
            T.Min4RoundFor = min(MINR4BACK, T.Min4RoundFor);
            T.Min4RoundFor = min(MINR4FOR, T.Min4RoundFor);
    
            for (int i = 0; i < 100; i++) {
                T.his2R[i] += MHis2R[i];
                T.his3RBack[i] += MHis3RBack[i];
                T.his3RFor[i] += MHis3RFor[i];
                T.his4RBack[i] += MHis4RBack[i];
                T.his4RFor[i] += MHis4RFor[i];
            }
            for (int i = 0; i < 40; i++) {
                for (int j = 0; j < 40; j++) {
                    T.his[i][j] += MHis[i][j];
                }
            }
            


            T.hi2(foutHisto2);
	        T.hi22(foutHisto2Det);
            Mf2R << T.Min2Round << endl;

            foutHisto3 << "Backward" << endl;
            T.hi3(foutHisto3, T.his3RBack);
            Mf3RBack << "Min3RBack: " << T.Min3RoundBack << endl;

            foutHisto3 << "Forward" << endl;
            T.hi3(foutHisto3, T.his3RFor);
            Mf3RFor << "Min3RFor: " << T.Min3RoundFor << endl;


            foutHisto4 << "Backward" << endl;
            T.hi4(foutHisto4, T.his4RBack);
            Mf4RBack << "Min4RBack: " << T.Min4RoundBack << endl;
            foutHisto4 << "Forward" << endl;
            T.hi4(foutHisto4, T.his4RFor);
            Mf4RFor << "Min4RFor: " << T.Min4RoundFor << endl;   

            cout << "tasks number: " << tasksCount << endl; 
            
            mtx.unlock();
            break;
        }
        sleep(60);
    }
}


