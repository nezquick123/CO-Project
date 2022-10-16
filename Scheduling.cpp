// Scheduling.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
using namespace std;
using namespace std::chrono;

int getIdOfMin(int* a, int n) {
    int id = 0;
    if (n > 0) {
        for (int i = 1; i < n; i++) {
            id = a[i] < a[id] ? i : id;
        }
    }  
    return id;
}

int getIdOfMax(int* a, int n) {
    int id = 0;
    if (n > 0) {
        for (int i = 1; i < n; i++) {
            id = a[i] > a[id]? i : id;
        }
    }
    return id;
}

int main()
{
    cout << "randomise (0) or file (1)? "<< endl;
    bool useFile;
    cin >> useFile;

    int m; // Number of CPUs
    int n; // Number of tasks
    int* cpuUsage; // Array tracking how much processing power has been used at each CPU
    int** tasksAtCpus; // Array of arrays which contain tasks currently kept at CPUs
    int* tasksAtCpusIds; // Array of indices used to manage tasksAtCpus
    int* p; // Array of input data of tasks
    auto start = high_resolution_clock::now(); // Starts tracking time

    if (useFile) { // Either using file or using generator 
        start = high_resolution_clock::now();
        ifstream dataInput("data.txt"); // Handling data input
        dataInput >> m >> n;     
        p = new int[n];
        int temp; 
        for (int i = 0; i < n; i++) {
            dataInput >> temp;
            p[i] = temp;
        }
        dataInput.close();
    }
    else {
        srand(time(NULL));  // Handling data input
        cout << "Enter number of tasks: ";
        cin >> n;
        cout << "Enter number of CPUs: ";
        cin >> m;
        cout << "Enter max task length: ";
        int maxTaskLength;
        cin >> maxTaskLength;

        start = high_resolution_clock::now();
        p = new int[n];
        int currentTask;
        for (int i = 0; i < n; i++) {
            currentTask = rand() % maxTaskLength + 1;
            p[i] = currentTask;
        }
    }
    cpuUsage = new int[m]; // Initialising every array
    tasksAtCpus = new int* [m];
    tasksAtCpusIds = new int[m];
    for (int i = 0; i < m; i++) {
        tasksAtCpus[i] = new int[n];
    }
    for (int i = 0; i < m; i++) {
        tasksAtCpusIds[i] = 0;
    }
    for (int i = 0; i < m; i++) {
        cpuUsage[i] = 0;
    }

    int currentMinId; // Id of that CPU which is currently the least busy
    for (int i = 0; i < n; i++) {
        currentMinId = getIdOfMin(cpuUsage, m);
        tasksAtCpus[currentMinId][tasksAtCpusIds[currentMinId]++] = p[i]; // Append task to CPU
        cpuUsage[currentMinId] += p[i]; // Adjust CPU usage
    }
       
    int cMax = cpuUsage[getIdOfMax(cpuUsage, m)]; // Get most busy CPU usage
    cout << cMax << endl;

    for (int i = 0; i < m; i++) { // Visualise
        for (int j = 0; j < tasksAtCpusIds[i]; j++) {
            cout << tasksAtCpus[i][j] << " ";
        }
        cout << endl;
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << duration.count() << endl;
}
   
