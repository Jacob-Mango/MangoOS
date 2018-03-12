
#ifndef __MANGOOS__TASK_MANAGER_H
#define __MANGOOS__TASK_MANAGER_H

#include <common/types.h>
#include <gdt.h>

namespace mangoos
{

class Task;
struct CPUState;

class TaskManager
{
  private:
    Task *tasks[256];
    int numTasks;
    int currentTask;

  public:
    TaskManager();
    ~TaskManager();
    bool AddTask(Task *task);
    CPUState *Schedule(CPUState *cpustate);
};
}
#endif