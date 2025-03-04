#ifndef REPO_H
#define REPO_H

void addTask(char description[]);
struct Task *loadAllTasks(int *taskCount);
void completeTask(struct Task *tasks, int taskCount, int taskNumber);

#endif