#include <iostream>
#include <random>
#include <vector>
#include "ctime"
#include "cstdlib"
#include "algorithm"
#include "fstream"
using namespace std;
class Gen{
public:
    int task_id;
    int proc_id;
    int start_time;
    int stop_time;
    Gen(int tid, int pid, int start, int stop){
        task_id = tid;
        proc_id = pid;
        start_time = start;
        stop_time = stop;
    }
    void print(){
        cout << "["<<task_id << ", " << proc_id << ", " << start_time << ", " << stop_time <<"], ";
    }
};
class Genome{
public:
    vector <Gen> gens_vec;
    int tk_id = 1;
    int * nextInitTime;
    int proc;
    Genome(int proc_n) {
        nextInitTime = new int[proc_n];
        for(int i = 0; i<proc_n; i++){
            nextInitTime[i] = 0;
        }
        proc = proc_n;
    }
    void addGen(int task_len, int proc_id){
        Gen g(tk_id, proc_id, nextInitTime[proc_id-1], nextInitTime[proc_id-1]+task_len);
        tk_id++;
        nextInitTime[proc_id-1] += task_len+1;
        gens_vec.push_back(g);
    }

    int cMax(){
        int m = 0;
        for(auto & i : gens_vec){
            if(i.stop_time>m)
                m = i.stop_time;
        }
        return m;
    }

    void repair(){
        int * start = new int[proc];
        for(int i = 0; i < proc; i++)
            start[i] = 0;
        for(auto & i : gens_vec){
            int task_len = i.stop_time - i.start_time;
            i.start_time = start[i.proc_id-1];
            i.stop_time = i.start_time + task_len;
            start[i.proc_id-1] = i.stop_time + 1;
        }
    }
};

vector<Genome> generate_random(int *t, int t_n, int proc, int gen_n){
    int *task = new int[t_n];
    for(int i = 0; i < t_n; i++){
        task[i] = t[i];
    }
//    for(int i = 0; i < t_n; i++){
//        cout << task[i] << " ";
//    }
    cout << endl;
    vector<Genome> gen_arr;
    for(int i = 0; i < gen_n; i++){
        shuffle(&task[0], &task[t_n], std::mt19937(std::random_device()()));
        Genome g(proc);
        int curr_proc = 1;
        for(int j = 0; j < t_n; j++){
            g.addGen(task[j], curr_proc);
            if(curr_proc == proc)
                curr_proc = 1;
            else
                curr_proc++;
        }
        gen_arr.push_back(g);
    }
    return gen_arr;
}

void print_gen(vector<Genome> vec){
    for(int i = 0; i < vec.size(); i++){
        for(int j = 0; j < vec[i].gens_vec.size(); j++){
            vec[i].gens_vec[j].print();
        }
        cout <<" Cmax = " <<  vec[i].cMax() << endl;
    }
    cout << endl;
}


void mutate(Genome &gen){
    int length = gen.gens_vec.size();
    int n = rand()%length;
    for(int i = 0; i < n; i++) {
        int id1 = rand() % length;
        int id2 = rand() % length;
        swap(gen.gens_vec[id1], gen.gens_vec[id2]);
        gen.repair();
    }
}

void crossover(Genome &gen1, Genome &gen2){
    int length = gen1.gens_vec.size();
    int n = rand()%length/2;
    for(int i = 0; i < n; i++){
        int id = rand()%length;
        Gen temp = gen1.gens_vec[id];
        gen1.gens_vec[id] = gen2.gens_vec[id];
        gen2.gens_vec[id] = temp;
        gen1.repair();
        gen2.repair();
    }
}

bool sortcond(const vector <int> & v1, const vector <int> & v2){
    return v1[1] < v2[1];
}

int main() {
    int population = 200; // Even number
    int iteration_n = 800, crossover_n = 40;
    srand(time(nullptr));
    ifstream file;
    file.open("m50n200.txt");
    int proc_n, tasks_n;
    file >> proc_n;
    file >> tasks_n;
    int * task_tab = new int [tasks_n];//Array of input tasks
    for(int i = 0; i<tasks_n; i++){
        file >> task_tab[i];
    }
    vector <Genome> g_arr;
    vector <vector <int>> idcmax (population + crossover_n);

    g_arr = generate_random(task_tab, tasks_n, proc_n, population/2);
    for(int iter = 0; iter < iteration_n; iter++) {
        vector<Genome> sel_arr = g_arr;

        // mutate
        for (int i = 0; i < g_arr.size(); i++) {
            vector<int> pair = {i, g_arr[i].cMax()};
            idcmax[i] = pair;
            mutate(g_arr[i]);
            pair = {i + population / 2, g_arr[i].cMax()};
            idcmax[i + population / 2] = pair;
            sel_arr.push_back(g_arr[i]);
        }
        //crossover
        for(int i = 0; i < crossover_n/2; i++) {
            int id1 = rand() % g_arr.size();
            int id2 = rand() % g_arr.size();
            crossover(g_arr[id1], g_arr[id2]);
            vector <int> pair = {i + population, g_arr[id1].cMax()};
            idcmax[i + population] = pair;
            pair = {i + population + crossover_n/2, g_arr[id2].cMax()};
            idcmax[i + population + crossover_n/2] = pair;
            sel_arr.push_back(g_arr[id1]);
            sel_arr.push_back(g_arr[id2]);
        }
        sort(idcmax.begin(), idcmax.end(), sortcond);
        //print_gen(g_arr);
        for (int i = 0; i < population/2; i++) { // select
            g_arr[i] = sel_arr[idcmax[i][0]];
        }
    }
    cout << "PCmax = " << g_arr[0].cMax() << endl;
    for(int i = 0; i < tasks_n; i++){
        g_arr[0].gens_vec[i].print();
    }
    return 0;
}
