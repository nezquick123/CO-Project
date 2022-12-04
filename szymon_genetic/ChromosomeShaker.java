package com.sazer;

import java.lang.reflect.Array;
import java.util.*;
import java.util.stream.Stream;

public class ChromosomeShaker {

    private static void crossTwo(Chromosome first, Chromosome second, int n) {
        int divisionPoint = n/2;
        List<Gene> cutout = new ArrayList<>();
        for(int i=divisionPoint;i<n;i++) {
            cutout.add(first.getGeneList().get(i));
        }
        for(int i=divisionPoint;i<n;i++) {
            first.getGeneList().set(i,second.getGeneList().get(i));
        }
        for(int i=divisionPoint;i<n;i++) {
            second.getGeneList().set(i,cutout.get(i-divisionPoint));
        }
    }
    public static List<Chromosome> middleCrossover(List<Chromosome> chromosomes, int n, int chromosomesInGen) {
        Random random = new Random();
        boolean[] alreadyCrossed = new boolean[chromosomesInGen];
        int idOfCrossingPair;
        Arrays.fill(alreadyCrossed, false);
        for(int i=0;i<chromosomesInGen;i++) {
            if (!alreadyCrossed[i]) {
                do{
                    idOfCrossingPair = random.nextInt(chromosomesInGen-1-i) + (i+1);
                } while(alreadyCrossed[idOfCrossingPair]);
//                System.out.printf("i, idofcrossinpair: %d, %d\n", i, idOfCrossingPair);
                ChromosomeShaker.crossTwo(chromosomes.get(i), chromosomes.get(idOfCrossingPair), n);
                alreadyCrossed[i] = true;
                alreadyCrossed[idOfCrossingPair] = true;
            }
        }
        return chromosomes;
    }

    public static List<Chromosome> adjustTimes(List<Chromosome> chromosomes, int m) {
        List<Chromosome> adjusted = new ArrayList<>();
        for(Chromosome chromosome : chromosomes) {
            int[] cpuTimes = new int[m];
            int adj;
            Arrays.fill(cpuTimes, 0);
            for (Gene gene : chromosome.getGeneList()) {
                if (gene.getStartTime() != cpuTimes[gene.getCpuId() - 1]) {
                    adj = cpuTimes[gene.getCpuId() - 1] - gene.getStartTime();
                    gene.setStartTime(gene.getStartTime() + adj);
                    gene.setFinishTime(gene.getFinishTime() + adj);
                }
                cpuTimes[gene.getCpuId() - 1] = gene.getFinishTime();
            }
            adjusted.add(chromosome);
        }
        return adjusted;
    }

    public static List<Chromosome> mutate(List<Chromosome> chromosomes, int n) {

        for(Chromosome chromosome : chromosomes) {
            Gene firstGene = chromosome.getGeneList().get((int)(Math.random()*n));
            Gene secondGene = chromosome.getGeneList().get((int)(Math.random()*n));
            int firstLength = firstGene.getFinishTime()- firstGene.getStartTime();
            int secondLength = secondGene.getFinishTime()- secondGene.getStartTime();
            int firstCpu = firstGene.getCpuId();
            int firstGeneStartingTime = firstGene.getStartTime();
            firstGene.setStartTime(secondGene.getStartTime());
            firstGene.setFinishTime(secondGene.getStartTime() + firstLength);
            secondGene.setStartTime(firstGeneStartingTime);
            secondGene.setFinishTime(firstGeneStartingTime + secondLength);
            firstGene.setCpuId(secondGene.getCpuId());
            secondGene.setCpuId(firstCpu);
        }
        return chromosomes;
    }

    public static Chromosome getCmax(List<Chromosome> chromosomes) {
        //Integer[] finishTimes;
        Chromosome bestInGeneration = null;
        int cmax = Integer.MAX_VALUE;
        for(Chromosome chromosome : chromosomes) {
            //finishTimes = chromosome.getGeneList().stream().map(Gene::getFinishTime).toArray(Integer[]::new);
            //System.out.println(Arrays.toString(finishTimes));
            chromosome.setCmax(Arrays.stream(Arrays.stream(chromosome.getGeneList().stream().map(Gene::getFinishTime).toArray(Integer[]::new)).mapToInt(Integer::intValue).toArray()).max().getAsInt());
            if (chromosome.getCmax() < cmax) {
                cmax = chromosome.getCmax();
                bestInGeneration = chromosome;
                //System.out.println(bestInGeneration);
            }
        }
        //System.out.println("Lowest this generation: " +cmax);
        return bestInGeneration;
    }
}
