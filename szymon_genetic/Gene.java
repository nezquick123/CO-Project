package com.sazer;

public class Gene{
    private int taskId;
    private int cpuId;
    private int startTime;
    private int finishTime;

    public Gene(int taskId, int cpuId, int startTime, int finishTime) {
        this.taskId = taskId;
        this.cpuId = cpuId;
        this.startTime = startTime;
        this.finishTime = finishTime;
    }

    public Gene (Gene gene) {
        this.taskId = gene.getTaskId();
        this.cpuId = gene.getCpuId();
        this.startTime = gene.getStartTime();
        this.finishTime = gene.getFinishTime();
    }

    @Override
    public String toString() {
        return String.format("[%d, %d, %d, %d]",
                this.taskId,
                this.cpuId,
                this.startTime,
                this.finishTime);
    }

    public int getTaskId() {
        return taskId;
    }

    public void setTaskId(int taskId) {
        this.taskId = taskId;
    }

    public int getCpuId() {
        return cpuId;
    }

    public void setCpuId(int cpuId) {
        this.cpuId = cpuId;
    }

    public int getStartTime() {
        return startTime;
    }

    public void setStartTime(int startTime) {
        this.startTime = startTime;
    }

    public int getFinishTime() {
        return finishTime;
    }

    public void setFinishTime(int finishTime) {
        this.finishTime = finishTime;
    }
}
