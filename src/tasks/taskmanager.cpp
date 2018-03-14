#include <tasks/taskmanager.h>
#include <tasks/task.h>

using namespace mangoos;

TaskManager::TaskManager()
{
    numTasks = 0;
    currentTask = -1;
}

TaskManager::~TaskManager() {}

bool TaskManager::AddTask(Task *task)
{
    if (numTasks >= 256)
        return false;
    tasks[numTasks++] = task;
    return true;
}

CPUState *TaskManager::Schedule(CPUState *cpustate)
{
    if (numTasks <= 0)
        return cpustate;
    if (currentTask >= 0)
        tasks[currentTask]->cpustate = cpustate;
    if (++currentTask >= numTasks)
        currentTask %= numTasks;
    return tasks[currentTask]->cpustate;
}