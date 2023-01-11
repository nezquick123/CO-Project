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
    int start_time = 0;
    int stop_time = 0;
    int len = 0;
    Gen(int tid, int pid, int start, int length){
        task_id = tid;
        proc_id = pid;
        start_time = start;
        len = length;
        stop_time = start + length-1;
    }
    void print(){
        cout << "["<<task_id << ", " << proc_id << ", " << start_time << ", " << stop_time <<"], ";
    }
};

bool gensortcond(const Gen& g1, const Gen& g2)
    {
        return (g1.len < g2.len);
    }

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
    void addGen(int task_len, int proc_id, int start_time){
        Gen g(tk_id, proc_id, start_time, task_len);
        tk_id++;
        nextInitTime[proc_id-1] += g.stop_time+1;
        gens_vec.push_back(g);
    }
    void sortgen(){
        sort(gens_vec.begin(),gens_vec.end(), gensortcond);
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
            int task_len = i.len;
            i.start_time = start[i.proc_id-1];
            i.stop_time = i.start_time + task_len;
            start[i.proc_id-1] = i.stop_time + 1;
        }
    }
};

void print_gen(vector<Genome> vec){
    for(int i = 0; i < vec.size(); i++){
        for(int j = 0; j < vec[i].gens_vec.size(); j++){
            vec[i].gens_vec[j].print();
        }
        cout <<" Cmax = " <<  vec[i].cMax() << endl;
    }
    cout << endl;
}


vector<Genome> generate_random(int *t, int t_n, int proc, int gen_n){
    int *task = new int[t_n];
    for(int i = 0; i < t_n; i++){
        task[i] = t[i];
    }
    default_random_engine defEngine(time(nullptr));
    uniform_int_distribution<int> intDistro(1, proc);
    //uniform_int_distribution<int> intmax(0, INT32_MAX);
    vector<Genome> gen_arr;
    for(int i = 0; i < gen_n; i++){
        Genome g(proc);
        int curr_proc;
        int task_time;
        for(int j = 0; j < t_n; j++){
            task_time = j;
            curr_proc = intDistro(defEngine);
            g.addGen(task[j], curr_proc, task_time);
        }
//        for(int j = 0; j < t_n; j++){
//            g.gens_vec[j].print();
//        }
        g.repair();
        gen_arr.push_back(g);
    }
    return gen_arr;
}

vector<Genome> generate_greedy(int *t, int t_n, int proc, int gen_n){
    int *task = new int[t_n];
    for(int i = 0; i < t_n; i++){
        task[i] = t[i];
    }
    vector<Genome> gen_arr;
    for(int i = 0; i < gen_n; i++){
        Genome g(proc);
        int curr_proc = 0;
        int task_time;

        for(int j = 0; j < t_n; j++){
            int min = g.nextInitTime[0];
            for(int p = 0; p < proc; p++){
                if (g.nextInitTime[p] <= min) {
                    min = g.nextInitTime[p];
                    curr_proc = p;
                }
            }
            //g.repair();
            g.addGen(task[j], curr_proc+1, min);
        }
    /*    for(int j = 0; j < t_n; j++){
          g.gens_vec[j].print();
        }*/

        gen_arr.push_back(g);
    }
    return gen_arr;
}





void mutate(Genome &gen, int n){
    int length = gen.gens_vec.size();
    n = rand()%length/2;
    for(int i = 0; i < n; i++) {
        int id1 = rand() % length;
        int id2 = rand() % length;
        int temp[2] = {gen.gens_vec[id1].proc_id,gen.gens_vec[id1].start_time};
        gen.gens_vec[id1].proc_id = gen.gens_vec[id2].proc_id;
        gen.gens_vec[id1].start_time = gen.gens_vec[id2].start_time;
        gen.gens_vec[id2].proc_id = temp[0];
        gen.gens_vec[id2].start_time = temp[1];
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
//Na razie najlepsze: pupulation: 600, crossover_n: 250, mutation_n: 250, elite_ratio:0.1

int main() {
    //PARAMETERS ---------------------------------------
    srand(time(nullptr));
    int population = 600, greedy_n = 20; // Even number
    int iteration_n = 1500000, crossover_n = 200, mutation_n = 200, runnig_time = 10;//seconds
    float elite_ratio = 0.1;
    ifstream file;
    file.open("test0.txt");
    //-------------------------------------------------
    int proc_n, tasks_n;
    file >> proc_n;
    file >> tasks_n;
    int * task_tab = new int [tasks_n];//Array of input tasks
    for(int i = 0; i<tasks_n; i++){
        file >> task_tab[i];
    }

    clock_t start = clock(); // start clock
    vector <Genome> g_arr;
    vector <vector <int>> idcmax (population + crossover_n);
    vector <Genome> rand_arr;
    g_arr = generate_random(task_tab, tasks_n, proc_n, population/2);
    for(int iter = 0; iter < iteration_n &&(clock() - start)<runnig_time*CLOCKS_PER_SEC; iter++) {
        vector<Genome> sel_arr = g_arr;
        //print_gen(g_arr);
        // mutate
        for (int i = 0; i < g_arr.size(); i++) {
            vector<int> pair = {i, g_arr[i].cMax()};
            idcmax[i] = pair;
            mutate(g_arr[i], mutation_n);
            pair = {i + population / 2, g_arr[i].cMax()};
            idcmax[i + population / 2] = pair;
            sel_arr.push_back(g_arr[i]);
        }
        //print_gen(g_arr);
        //crossover
        for(int i = 0; i < crossover_n; i+=2) {
            int id1 = rand() % g_arr.size();
            int id2 = rand() % g_arr.size();
            crossover(g_arr[id1], g_arr[id2]);
            vector <int> pair = {i + population, g_arr[id1].cMax()};
            idcmax[i + population] = pair;
            pair = {i + 1 + population, g_arr[id2].cMax()};
            idcmax[i + 1 + population] = pair;
            sel_arr.push_back(g_arr[id1]);
            sel_arr.push_back(g_arr[id2]);
        }
        //print_gen(g_arr);
        sort(idcmax.begin(), idcmax.end(), sortcond);

        int i = 0;
        for (i = 0; i < int((population + crossover_n)*elite_ratio); i++) { // select
            g_arr[i] = sel_arr[idcmax[i][0]];
        }
        sel_arr = generate_greedy(task_tab, tasks_n, proc_n, greedy_n);
        int j = 0;
        int istart = i;
        for(i; i < greedy_n+istart; i++){
         //   cout << sel_arr[j].cMax() <<endl;
            g_arr[i] = sel_arr[j++];
        }
        sel_arr = generate_random(task_tab, tasks_n, proc_n, (population+crossover_n)-int((population+crossover_n)*elite_ratio));
        j = 0;
        for (i ; i < g_arr.size(); i++) { // select DO P
            g_arr[i] = sel_arr[j++];
        }
        cout <<iter << " PCmax = " << g_arr[0].cMax() << endl;
    }
    cout << "PCmax = " << g_arr[0].cMax() << endl;
    for(int i = 0; i < tasks_n; i++){
        g_arr[0].gens_vec[i].print();
    }
    return 0;
}
