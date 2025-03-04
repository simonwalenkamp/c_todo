#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../model/task.h"

#define TODO_FILE "todoList.bin"

void addTask(char description[])
{
    struct Task newTask;

    strcpy(newTask.description, description);
    newTask.completed = false;

    FILE *file = fopen(TODO_FILE, "ab");

    fwrite(&newTask, sizeof(struct Task), 1, file);

    fclose(file);
}

void completeTask(struct Task *tasks, int taskCount, int taskNumber)
{
    // Convert 1-indexed task number to 0-indexed array index
    int index = taskNumber - 1;
    if (index < 0 || index >= taskCount)
    {
        printf("Task number %d does not exist.\n", taskNumber);
        return;
    }

    tasks[index].completed = true;

    // Open the file in write mode to update all tasks
    FILE *file = fopen(TODO_FILE, "wb");

    fwrite(tasks, sizeof(struct Task), taskCount, file);

    fclose(file);
}

struct Task *loadAllTasks(int *taskCount)
{
    int capacity = 10; // Initial capacity
    *taskCount = 0;
    struct Task *tasks = malloc(capacity * sizeof(struct Task));
    if (!tasks)
    {
        perror("memory allocation failed");
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(TODO_FILE, "rb");
    if (!file)
    {
        // File might not exist; return an empty list.
        return tasks;
    }

    struct Task temp;
    while (fread(&temp, sizeof(struct Task), 1, file) == 1)
    {
        if (*taskCount >= capacity)
        {
            capacity *= 2;
            tasks = realloc(tasks, capacity * sizeof(struct Task));
            if (!tasks)
            {
                perror("realloc failed");
                exit(EXIT_FAILURE);
            }
        }
        tasks[*taskCount] = temp;
        (*taskCount)++;
    }
    fclose(file);
    return tasks;
}