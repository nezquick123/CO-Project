package com.sazer;

import java.util.ArrayList;
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
}
