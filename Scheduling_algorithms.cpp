#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
typedef long double lld;
#define endl '\n';

#define JooQatry               \
    ios::sync_with_stdio(0); \
    cin.tie(0);              \
    cout.tie(0);
#define all(v) ((v).begin()), ((v).end())
#define pb push_back
#define MP make_pair
#define F first
#define S second
typedef pair<int, int> II;
typedef pair<int, pair<int, int> > PIP;

class Process {

    int process_number = 0;
    int process_arrival_time = 0;
    int process_running_time = 0;
    int process_waiting_time = 0;
    int process_complete_time = 0;
//    int process_turnAround_time = 0;

public:

    int get_number() {
        return process_number;
    }

    int get_arrival_time() {
        return process_arrival_time;
    }

    int get_running_time() {
        return process_running_time;
    }

    int get_waiting_time() {
        return process_waiting_time;
    }

    int get_complete_time() {
        return process_complete_time;
    }



    void set_number(int number) {
        process_number = number;
    }

    void set_arrival_time(int arrival_time) {
        process_arrival_time = arrival_time;
    }

    void set_running_time(int running_time) {
        process_running_time = running_time;
    }

    void set_waiting_time(int waiting_time) {
        process_waiting_time = waiting_time;
    }

    void set_complete_time(int complete_time) {
        process_complete_time = complete_time;
    }

};

bool compareByArrival(Process p, Process q) {
    return p.get_arrival_time() < q.get_arrival_time();
}

bool compareByNumber(Process p, Process q) {
    return p.get_number() < q.get_number();
}

bool compareByRunningTime(Process p, Process q) {
    return p.get_running_time() < q.get_running_time();
}


void print(Process p[], int processNum, double AvgTime) {
    //Sort by process number
    sort(p, p + processNum, compareByNumber);
    cout << "\n\n\t\t The Process Status \n\n";
    cout << "\tProcess ID\tArrival Time\trunning Time\tCompletion Time\t  Waiting Time";
    for (int i = 0; i < processNum; ++i)
        cout << "\n\t\t" << p[i].get_number() << "\t\t" << p[i].get_arrival_time() << "\t\t" << p[i].get_running_time()
             << "\t\t"
             << p[i].get_complete_time() << "\t\t" << p[i].get_waiting_time();
    cout << "\n\n\t\tAverage Waiting Time: " << AvgTime;
    cout << "\n\n\n";

}

// FCFS

void FirstComeFirstServed(Process p[], int processNum) {
    double avgWaitTime = 0.0;
    // Sort the processes according to arrival time
    sort(p, p + processNum, compareByArrival);

    for (int i = 0, curr = 0; i < processNum; ++i) {
        p[i].set_complete_time(max(curr, p[i].get_arrival_time()) + p[i].get_running_time());
        p[i].set_waiting_time((p[i].get_complete_time() - p[i].get_arrival_time()) - p[i].get_running_time());
        curr = p[i].get_complete_time();

        avgWaitTime += p[i].get_waiting_time();
    }
    avgWaitTime /= processNum;
    cout << "Average waiting time of FCFS: " << avgWaitTime << endl;
}

// SJF

void ShortestJobFirst(Process p[], int processNum) {

    int executedProcess = 0;
    bool visitedProcess[processNum];
    fill(visitedProcess, visitedProcess + processNum, false);
    vector<Process> processInQueue;

    // key: processNum, value: completionTime
    map<int, int> mp;

    for (int t = 0; executedProcess < processNum; ++t) { // t --> time
        for (int i = 0; i < processNum; ++i) { // i --> access the processes
            if (!visitedProcess[p[i].get_number() - 1] and p[i].get_arrival_time() <= t) {
                // To check if process is executed before and also whether it has arrived or not
                processInQueue.push_back(p[i]);
                visitedProcess[p[i].get_number() - 1] = true;
            }
        }
        // Start calculation from the minimum running time
        if (processInQueue.size() != 0) {
            auto minPosition = min_element(all(processInQueue), compareByRunningTime);
            Process processMinRunningTime = *minPosition;
            t += processMinRunningTime.get_running_time();
            mp[processMinRunningTime.get_number()] = t;
            executedProcess++;
            processInQueue.erase(minPosition);
        }
    }

    double avgWaitTime = 0.0;

    for (int i = 0; i < processNum; ++i) {
        p[i].set_complete_time(mp[p[i].get_number()]);
        p[i].set_waiting_time(p[i].get_complete_time() - p[i].get_arrival_time() - p[i].get_running_time());
        avgWaitTime += p[i].get_waiting_time();
    }
    avgWaitTime /= processNum;
    cout << "Average waiting time of SJF: " << avgWaitTime << endl;
}

// RR

void RoundRobin(Process p[], int processNum) {
    int tQuantum = rand() % 10 + 1;
//    cout << "\t Time quantum : ";
//    cin >> tQuantum;
    bool visitedProcess[processNum + 1];
    fill(visitedProcess, visitedProcess + processNum + 1, false);
    map<int, int> mp3;
    int executedProcess = 0, currTime = 0;
    queue<Process> ready_queue;
    while (executedProcess != processNum) {
        for (int i = 0; i < processNum; ++i) {
            if (!visitedProcess[p[i].get_number()] and p[i].get_arrival_time() == currTime) {
                ready_queue.push(p[i]);
                visitedProcess[p[i].get_number()] = true;
            }
        }
        if (!ready_queue.empty()) {
            Process pr = ready_queue.front();
            ready_queue.pop();
            int tq = min(tQuantum, pr.get_running_time());
            int b = pr.get_running_time();
            pr.set_running_time(pr.get_running_time() - tq);
            for (int i = currTime + 1; i <= currTime + tq; ++i) {
                for (int j = 0; j < processNum; ++j) {
                    if (!visitedProcess[p[j].get_number()] and p[j].get_arrival_time() == i) {
                        ready_queue.push(p[j]);
                        visitedProcess[p[j].get_number()] = true;
                    }
                }
            }
            currTime += tq;
            if (pr.get_running_time() == 0) {
                executedProcess++;
                pr.set_complete_time(currTime);
                mp3[pr.get_number()] = pr.get_complete_time();
            } else ready_queue.push(pr);
        } else currTime++;

    }

    double avgWaitTime = 0.0;

    for (int i = 0; i < processNum; ++i) {
        p[i].set_complete_time(mp3[p[i].get_number()]);
        p[i].set_waiting_time(p[i].get_complete_time() - p[i].get_arrival_time() - p[i].get_running_time());
        avgWaitTime += p[i].get_waiting_time();
    }

    avgWaitTime /= processNum;
    cout << "Average waiting time of RR: " << avgWaitTime << endl;
}

// SJRF

void ShortestJobRemainingFirst(Process p[], int processNum) {
    int t = 0, executedProcess = 0;
    double avgWaitTime = 0.0;
    vector<Process> processInQueue;
    bool visitedProcess[processNum];
    fill(visitedProcess, visitedProcess + processNum, false);
    map<int, int> mp2;
    while (executedProcess != processNum) {
        for (int i = 0; i < processNum; i++) {
            if (p[i].get_arrival_time() <= t and !visitedProcess[p[i].get_number() - 1]) {
                processInQueue.push_back(p[i]);
                visitedProcess[p[i].get_number() - 1] = true;
            }
        }

        if (processInQueue.size() != 0) {
            auto minPosition = min_element(all(processInQueue), compareByRunningTime);
            // Don't forget to set the quantum minus
            (*minPosition).set_running_time((*minPosition).get_running_time() - 1);
            t++;
            if ((*minPosition).get_running_time() == 0) {
                mp2[(*minPosition).get_number()] = t;
                executedProcess++;
                processInQueue.erase(minPosition);
            }

        } else t++;
    }
    for (int i = 0; i < processNum; i++) {
        p[i].set_complete_time(mp2[p[i].get_number()]);
        p[i].set_waiting_time(p[i].get_complete_time() - p[i].get_arrival_time() - p[i].get_running_time());
        avgWaitTime += p[i].get_waiting_time();
    }
    avgWaitTime /= processNum;
    cout << "Average waiting t of SJRF: " << avgWaitTime << endl;
}


int main() {
    int processNumber;
    cout << "Enter the number of processes:";
    cin >> processNumber;
    cout << endl;
    Process p[processNumber];
    srand(time(NULL));
    for (int i = 0; i < processNumber; ++i) {

        p[i].set_number(i + 1);
        p[i].set_arrival_time(rand() % 20 + 1);
        p[i].set_running_time(rand() % 20 + 1);
        p[i].set_complete_time(0);
        p[i].set_waiting_time(0);

    }
    FirstComeFirstServed(p, processNumber);
    ShortestJobFirst(p, processNumber);
    ShortestJobRemainingFirst(p, processNumber);
    RoundRobin(p, processNumber);


    sort(p, p + processNumber, compareByNumber);


    cout << "\n\n\t\t The Process Status \n\n";
    cout << "\tProcess Number\tArrival Time\trunning Time\tCompletion Time\t  Waiting Time";
    for (int i = 0; i < processNumber; ++i)
        cout << "\n\t\t" << p[i].get_number() << "\t\t" << p[i].get_arrival_time() << "\t\t" << p[i].get_running_time()
             << "\t\t"
             << p[i].get_complete_time() << "\t\t" << p[i].get_waiting_time();

    return 0;
}