#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "repo/repo.h"

#define TODO_FILE "todoList.bin"

struct Task
{
    char description[256];
    bool completed;
};

void printTasks(struct Task *tasks, int taskCount);
bool isNumber(const char number[]);

int main(int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-add") == 0)
        {
            // TODO: Also check for whitespace
            if (argv[i + 1] == NULL || strcmp(argv[i + 1], "") == 0)
            {
                printf("Please provide a description for the task\n");
                return 0;
            }

            addTask(argv[i + 1]);
            return 0;
        }

        if (strcmp(argv[i], "-done") == 0)
        {
            if (argv[i + 1] == NULL || !isNumber(argv[i + 1]))
            {
                printf("Please provide the number of the task to complete\n");
                return 0;
            }

            int taskCount;
            struct Task *tasks = loadAllTasks(&taskCount);

            completeTask(tasks, taskCount, atoi(argv[i + 1]));

            return 0;
        }

        if (strcmp(argv[i], "-list") == 0)
        {
            int taskCount;
            struct Task *tasks = loadAllTasks(&taskCount);
            printTasks(tasks, taskCount);
            free(tasks);
            return 0;
        }
    }

    printf("Please provide an argument");
    return 0;
}

void printTasks(struct Task *tasks, int taskCount)
{
    for (int i = 0; i < taskCount; i++)
    {
        // Print with strikethrough
        if (tasks[i].completed)
            printf("%d: \033[9m%s\033[0m\n", i + 1, tasks[i].description);
        else
            printf("%d: %s\n", i + 1, tasks[i].description);
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