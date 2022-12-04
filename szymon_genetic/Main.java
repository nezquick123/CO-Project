package com.sazer;

import java.io.File;
import java.io.FileNotFoundException;
import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Scanner;

public class Main {
    private static final int GENERATIONS_NUMBER = 100;
    private static final int CHROMOSOMES_PER_GENERATION = 200;

    public static void main(String[] args) throws FileNotFoundException {
        // Data reading
        File data = new File("data.txt");
        Scanner fileInput = new Scanner(data);
        List<Task> tasks = new ArrayList<>();
        int m = fileInput.nextInt();
        int n = fileInput.nextInt();
        for(int i=0;i<n;i++) {
            tasks.add(new Task(i+1, fileInput.nextInt()));
        }
        System.out.println(tasks);

        List<Chromosome> chromosomeList = new ArrayList<>();
        for(int i=0;i<CHROMOSOMES_PER_GENERATION; i++) {
            chromosomeList.add(ChromosomeBuilder.build(tasks, m));
        }
        Chromosome winner = new Chromosome(chromosomeList.get(0));
        Chromosome potentialWinner;
        //System.out.println("Initial chromosomes");
        //chromosomeList.forEach(System.out::println);
        //int cmax = Integer.MAX_VALUE;
        for(int i=0;i<GENERATIONS_NUMBER;i++) {
            chromosomeList = ChromosomeShaker.middleCrossover(chromosomeList, n, CHROMOSOMES_PER_GENERATION);
            //System.out.println("first crossing");
            //chromosomeList.forEach(System.out::println);

            chromosomeList = ChromosomeShaker.adjustTimes(chromosomeList, m);
            //System.out.println("Adjusted");
            //chromosomeList.forEach(System.out::println);
            potentialWinner = new Chromosome(ChromosomeShaker.getCmax(chromosomeList));
            //System.out.printf("not changing, potenatial: %d, winner: %d\n", potentialWinner.getCmax(),winner.getCmax());
            if (potentialWinner.getCmax() < winner.getCmax()) {
               // System.out.printf("changing, potenatial: %d, winner: %d\n", potentialWinner.getCmax(),winner.getCmax());
                winner = new Chromosome(potentialWinner);
            }
            //cmax = Math.min(cmax, currentCmax);

            chromosomeList = ChromosomeShaker.mutate(chromosomeList, n);
            //System.out.println("Mutated");
            //chromosomeList.forEach(System.out::println);

            chromosomeList = ChromosomeShaker.adjustTimes(chromosomeList, m);
            //System.out.println("Adjusted");
            //chromosomeList.forEach(System.out::println);
            potentialWinner = new Chromosome(ChromosomeShaker.getCmax(chromosomeList));
            //System.out.printf("not changing, potenatial: %d, winner: %d\n", potentialWinner.getCmax(),winner.getCmax());
            if (potentialWinner.getCmax() < winner.getCmax()) {
                //System.out.printf("changing, potenatial: %d, winner: %d\n", potentialWinner.getCmax(),winner.getCmax());
                winner = new Chromosome(potentialWinner);
            }
            //cmax = Math.min(cmax, currentCmax);
        }
        System.out.println(winner.getCmax());
        System.out.println(winner);
    }
}
