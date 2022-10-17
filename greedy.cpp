#include <iostream>
#include <fstream>
#include <chrono>
using namespace std;
using namespace std::chrono;


void ver1(){
    ifstream file;
    file.open("test.txt");
    if(!file.good())
    {
        cerr << "Wrong file path" <<endl;
        exit(1);
    }
    int proc_n, tasks_n, next_task = 0; //next_task points to id of next task to execute
    file >> proc_n;
    file >> tasks_n;
    int * proc_sum = new int[proc_n]; //Array of current sums of time needed to do every task at each CPU
    int * proc_seek = new int[proc_n]; //Array of indexes of next free element proc_task array
    int * task_tab = new int [tasks_n];//Array of input tasks
        for(int i = 0; i<tasks_n; i++){
        file >> task_tab[i];
    }
    auto start = high_resolution_clock::now();
    int ** proc_task = new int*[proc_n]; //2D Array of tasks assigned to each processor
    for(int i = 0; i < proc_n; i++){
        proc_task[i] = new int[tasks_n];
        proc_sum[i] = 0;
        proc_seek[i] = 0;
        for(int j = 0; j < tasks_n; j++)
            proc_task[i][j] = -1;
    }
    int mini = 0;

    while(next_task < tasks_n){
        mini = 0;// Finding the processor with minimal CPU usage
        for(int i = 1; i < proc_n; i++){
            if(proc_sum[mini] > proc_sum[i]){
                mini = i;
            }
        }/////////////////////////////////////
        proc_sum[mini] += task_tab[next_task];
        proc_task[mini][proc_seek[mini]] = next_task + 1;
        next_task+=1;
        proc_seek[mini]+=1;
    }
        auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    for(int i = 0; i < proc_n; i++){
        for(int j = 0; j < tasks_n; j++){
            if(proc_task[i][j] == -1) break;
            cout << proc_task[i][j] << " ";
        }
        cout << endl;
    }
    cout << duration.count() << endl;
}

int main(){

    ver1();
    return 0;
}
