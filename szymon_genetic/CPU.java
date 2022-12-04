package com.sazer;

import java.util.ArrayList;
import java.util.List;

public class CPU {
    List<Task> tasks;

    public CPU() {
        this.tasks = new ArrayList<>();
    }

    public CPU(CPU cpu) {
        this.tasks = new ArrayList<>();
        this.tasks.addAll(cpu.getTasks());
    }

    public void addTask(Task task) {
        this.tasks.add(task);
    }

    public List<Task> getTasks() {
        return tasks;
    }

    public void setTasks(List<Task> tasks) {
        this.tasks = tasks;
    }

    @Override
    public String toString() {
        return "CPU{" +
                "tasks=" + tasks +
                '}';
    }
}
