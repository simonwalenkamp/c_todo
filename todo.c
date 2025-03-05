#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "repos/repo.h"
#include "models/task.h "

void listTasks(bool completed);
void markTaskAsCompleted(int taskNumber);
void addTask(char description[]);
void printTasks(struct Task *tasks, int taskCount, bool completed);
bool isNumber(const char number[]);

int main(int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-add") == 0)
        {
            char *description = argv[i + 1];

            addTask(description);

            return 0;
        }

        if (strcmp(argv[i], "-done") == 0)
        {
            if (argv[i + 1] == NULL || !isNumber(argv[i + 1]))
            {
                printf("Please provide the number of the task to complete\n");
                return 0;
            }

            int taskNumber = atoi(argv[i + 1]);

            markTaskAsCompleted(taskNumber);

            return 0;
        }

        if (strcmp(argv[i], "-list") == 0)
        {
            bool completed = false;

            if (argv[i + 1] != NULL && strcmp(argv[i + 1], "--completed") == 0)
            {
                completed = true;
            }

            listTasks(completed);

            return 0;
        }
    }

    printf("Please provide an argument");

    return 0;
}

void listTasks(bool completed)
{
    int taskCount;

    struct Task *tasks = readAll(&taskCount);

    printTasks(tasks, taskCount, completed);

    free(tasks);
}

void markTaskAsCompleted(int taskNumber)
{
    struct Task *task = read(taskNumber);

    task->completed = true;

    update(task, taskNumber);

    free(task);
}

void addTask(char description[])
{
    // TODO: Also check for whitespace
    if (description == NULL || strcmp(description, "") == 0)
    {
        printf("Please provide a description for the task\n");
        return;
    }

    create(description);
}

void printTasks(struct Task *tasks, int taskCount, bool completed)
{
    for (int i = 0; i < taskCount; i++)
    {
        // Print with strikethrough
        if (tasks[i].completed)
        {
            if (!completed)
            {
                continue;
            }

            printf("%d: \033[9m%s\033[0m\n", i + 1, tasks[i].description);
        }
        else
        {
            printf("%d: %s\n", i + 1, tasks[i].description);
        }
    }
}

bool isNumber(const char number[])
{
    int i = 0;

    for (; number[i] != 0; i++)
    {
        if (!isdigit(number[i]))
            return false;
    }

    return true;
}