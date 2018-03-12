#include <tasks/task.h>
#include <tasks/taskmanager.h>

using namespace mangoos;
using namespace mangoos::common;

Task::Task(GlobalDescriptorTable *gdt, void entrypoint())
{
    cpustate = (CPUState *)(stack + 4096 - sizeof(CPUState));
    cpustate->eax = 0;
    cpustate->ebx = 0;
    cpustate->ecx = 0;
    cpustate->edx = 0;

    cpustate->esi = 0;
    cpustate->edi = 0;
    cpustate->ebp = 0;

    /*    
    cpustate -> gs = 0;    
    cpustate -> fs = 0;    
    cpustate -> es = 0;    
    cpustate -> ds = 0;    
    */

    // cpustate -> error = 0;
    // cpustate -> esp = ;   F
    cpustate->eip = (uint32_t)entrypoint;
    cpustate->cs = gdt->CodeSegmentSelector();
    // cpustate -> ss = ;
    cpustate->eflags = 0x202;
}

Task::~Task() {

}