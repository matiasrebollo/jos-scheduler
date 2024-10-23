#include <inc/assert.h>
#include <inc/x86.h>
#include <inc/error.h>
#include <kern/spinlock.h>
#include <kern/env.h>
#include <kern/pmap.h>
#include <kern/monitor.h>

#define MAX_EXECS                                                              \
	10  // Number of max executions of an env in a queue before being downgraded
#define MAX_QUEUES 5  // Total number of queues
#define EXECS_BEFORE_UPGRADE                                                   \
	50  // Number of sched executions before all jobs are upgraded to prevent starvation

void sched_halt(void);

int executions = 0;

// Choose a user environment to run and run it.
void
sched_yield(void)
{
#ifdef SCHED_ROUND_ROBIN
	// Implement simple round-robin scheduling.
	//
	// Search through 'envs' for an ENV_RUNNABLE environment in
	// circular fashion starting just after the env this CPU was
	// last running. Switch to the first such environment found.
	//
	// If no envs are runnable, but the environment previously
	// running on this CPU is still ENV_RUNNING, it's okay to
	// choose that environment.
	//
	// Never choose an environment that's currently running on
	// another CPU (env_status == ENV_RUNNING). If there are
	// no runnable environments, simply drop through to the code
	// below to halt the cpu.

	int current_index = 0;

	if (curenv) {
		current_index =
		        ENVX(curenv->env_id) +
		        1;  // calculo el indice del proceso siguiente al que voy a frenar
	}

	for (int i = 0; i < NENV; i++) {
		int index = (current_index + i) % NENV;  // para hacerlo circular

		if (envs[index].env_status ==
		    ENV_RUNNABLE) {  // me fijo si el proceso es RUNNABLE y si lo es, lo elijo.
			env_run(&envs[index]);
		}
	}

	// si el for no encontro ningun proceso runnable, y el anterior sigue estando
	// en estado RUNNING (porque quizas termino en el medio), lo elijo.
	if (curenv && (curenv->env_status == ENV_RUNNING)) {
		env_run(curenv);
	}

#endif

#ifdef SCHED_PRIORITIES
	// Implement simple priorities scheduling.
	//
	// Environments now have a "priority" so it must be consider
	// when the selection is performed.
	//
	// Be careful to not fall in "starvation" such that only one
	// environment is selected and run every time.

	// Your code here - Priorities

	struct Env *env = NULL;
	int current_index = 0;

	if (curenv) {
		current_index = ENVX(curenv->env_id) + 1;
	}

	int current_priority = MIN_PRIORITY;

	for (int i = (current_index % NENV); i < NENV; i++) {
		if (envs[i].env_status == ENV_RUNNABLE &&
		    envs[i].env_priority >
		            current_priority) {  // If env is RUNNABLE and has better priority, it's selected
			env = &envs[i];
			current_priority = env->env_priority;
		}
	}

	if (env) {
		env_run(env);
		env->q_execution_count++;
		if (env->q_execution_count >= MAX_EXECS &&
		    env->env_priority >
		            MIN_PRIORITY) {  // env is downgraded when has excedeed
			                     // number of MAX_EXECS and it's not on the lowest queue
			env->env_priority--;
			env->q_execution_count = 0;
		}
	} else if (curenv && (curenv->env_status == ENV_RUNNING)) {
		env_run(curenv);
	} else {
		sched_halt();
	}

	executions++;
	if (executions >=
	    EXECS_BEFORE_UPGRADE) {  // After EXECS_BEFORE_UPGRADE execs, all jobs
		                     // are upgraded and their queue values are reset.
		int index;
		for (int i = 0; i < NENV; i++) {
			envs[i].env_priority = MAX_PRIORITY;
			envs[i].q_execution_count = 0;
		}
	}

#endif

	// sched_halt never returns
	sched_halt();
}

// Halt this CPU when there is nothing to do. Wait until the
// timer interrupt wakes it up. This function never returns.
//
void
sched_halt(void)
{
	int i;

	// For debugging and testing purposes, if there are no runnable
	// environments in the system, then drop into the kernel monitor.
	for (i = 0; i < NENV; i++) {
		if ((envs[i].env_status == ENV_RUNNABLE ||
		     envs[i].env_status == ENV_RUNNING ||
		     envs[i].env_status == ENV_DYING))
			break;
	}
	if (i == NENV) {
		cprintf("No runnable environments in the system!\n");
		while (1)
			monitor(NULL);
	}

	// Mark that no environment is running on this CPU
	curenv = NULL;
	lcr3(PADDR(kern_pgdir));

	// Mark that this CPU is in the HALT state, so that when
	// timer interupts come in, we know we should re-acquire the
	// big kernel lock
	xchg(&thiscpu->cpu_status, CPU_HALTED);

	// Release the big kernel lock as if we were "leaving" the kernel
	unlock_kernel();

	// Once the scheduler has finishied it's work, print statistics
	// on performance. Your code here

	// Reset stack pointer, enable interrupts and then halt.
	asm volatile("movl $0, %%ebp\n"
	             "movl %0, %%esp\n"
	             "pushl $0\n"
	             "pushl $0\n"
	             "sti\n"
	             "1:\n"
	             "hlt\n"
	             "jmp 1b\n"
	             :
	             : "a"(thiscpu->cpu_ts.ts_esp0));
}
