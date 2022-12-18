package com.sazer;

public class Task {
    private int id;
    private int length;

    public Task(int id, int length) {
        this.id = id;
        this.length = length;
    }

    public Task(Task task) {
        this.id = task.getId();
        this.length = task.getLength();
    }

    @Override
    public String toString() {
        return "Task{" +
                "id=" + id +
                ", length=" + length +
                '}';
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getLength() {
        return length;
    }

    public void setLength(int length) {
        this.length = length;
    }
}
