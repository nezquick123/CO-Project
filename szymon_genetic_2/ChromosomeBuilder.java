package com.sazer;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

public class ChromosomeBuilder {

    public static Chromosome build(List<Task> tasks, int m) {
        List<CPU> cpus = new ArrayList<>();
        for(int i=0;i<m;i++){
            cpus.add(new CPU());
        }
        tasks.forEach( (task) -> cpus.get((int) (Math.random() * m)).addTask(task));
        return new Chromosome(cpus);
    }

    public static Chromosome buildGreedy(List<Task> tasks, int m) {
        //Collections.shuffle(tasks);
        List<CPU> cpus = new ArrayList<>();
        for(int i=0;i<m;i++){
            cpus.add(new CPU());
        }
        int[] cpusWorkload = new int[m];
        for(int i=0;i<m;i++) {
            cpusWorkload[i] = 0;
        }
        int leastLoadedCpuId;
        for(Task t : tasks) {
            leastLoadedCpuId = 0;
            for(int i=0;i<m;i++) {
                leastLoadedCpuId = cpusWorkload[i] < cpusWorkload[leastLoadedCpuId] ? i :leastLoadedCpuId;
            }
            cpus.get(leastLoadedCpuId).addTask(t);
            cpusWorkload[leastLoadedCpuId] += t.getLength();
        }
        return new Chromosome(cpus);
    }
}
