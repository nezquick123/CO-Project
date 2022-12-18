package com.sazer;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;

public class Chromosome {
    private List<Gene> geneList;
    private List<CPU> cpus;
    private int cmax;

    public Chromosome(List<CPU> cpus) {
        this.geneList = new ArrayList<>();
        this.cpus = new ArrayList<>();
        this.cpus.addAll(cpus);
        this.cmax = Integer.MAX_VALUE;
        this.cpuConversion();
        this.sortGenes();
    }

    public Chromosome(Chromosome chromosome) {
        this.geneList = new ArrayList<>();
        for(Gene gene : chromosome.getGeneList()) {
            this.geneList.add(new Gene(gene));
        }
        this.cpus = new ArrayList<>();
        for(CPU cpu : chromosome.getCpus()) {
            this.cpus.add(new CPU(cpu));
        }
        this.cmax = chromosome.getCmax();
    }

    private void cpuConversion() {
        int currentCpuId = 1;
        int currentCpuTimer;
        for(CPU cpu : this.cpus) {
            currentCpuTimer = 0;
            for(Task task : cpu.tasks) {
                this.geneList.add(new Gene(
                        task.getId(),
                        currentCpuId,
                        currentCpuTimer,
                        currentCpuTimer+task.getLength()
                ));
                currentCpuTimer+=task.getLength();
            }
            currentCpuId++;
        }
    }

    private void sortGenes() {
        this.geneList.sort(
                Comparator.comparing((Gene gene) -> Integer.toString(gene.getTaskId()))
    );
    }

    @Override
    public String toString() {
        return "Chromosome{" +
                "geneList=" + geneList +
                '}';
    }

    public void printCmax(){
        System.out.println(this.getCmax());
    }

    public List<Gene> getGeneList() {
        return geneList;
    }

    public void setGeneList(List<Gene> geneList) {
        this.geneList = geneList;
    }

    public List<CPU> getCpus() {
        return cpus;
    }

    public void setCpus(List<CPU> cpus) {
        this.cpus = cpus;
    }

    public int getCmax() {
        return cmax;
    }

    public void setCmax(int cmax) {
        this.cmax = cmax;
    }
}
