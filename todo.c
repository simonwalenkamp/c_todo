#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define TODO_FILE "todoList.bin"

struct Task
{
    char description[256];
    bool completed;
};

void addTask(char description[]);
struct Task *loadAllTasks(int *taskCount);
void printTasks(struct Task *tasks, int taskCount);
void addElement(struct Task newTask,
                struct Task tasks[],
                int *counter,
                int size);
bool isNumber(const char number[]);
void completeTask(struct Task *tasks, int taskCount, int taskNumber);

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

void addTask(char description[])
{
    struct Task newTask;

    strcpy(newTask.description, description);
    newTask.completed = false;

    FILE *file = fopen(TODO_FILE, "ab");

    fwrite(&newTask, sizeof(struct Task), 1, file);

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

// Function to add an element to an array of structs
void addElement(struct Task newTask,
                struct Task tasks[],
                int *counter,
                int size)
{
    // Ensure array bounds are not exceeded
    if (*counter < size)
    {
        tasks[*counter] = newTask;

        // Increment the count of element in the array
        (*counter)++;
    }
    else
    {
        printf("Array is full, cannot add more tasks.\n");
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
