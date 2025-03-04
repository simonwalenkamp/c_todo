#ifndef REPO_H
#define REPO_H

void create(char description[]);
struct Task *readAll(int *taskCount);
void update(struct Task *task, int taskNumber);
struct Task *read(int taskNumber);

#endif