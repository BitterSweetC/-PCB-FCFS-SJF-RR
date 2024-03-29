进程管理

该库包含了C语言版本的三种基础进程管理调度算法：先来先服务调度算法，短作业优先调度算法以及时间片轮转调度算法。

先来先服务调度算法按照进程到达的先后次序进行调度。

短作业优先算法按照进程剩余运行时间的长短进行调度，时间越短，其优先级越高，被调用的次序提前。
短作业优先算法有两种模式：抢占式SJF，非抢占式SJF。抢占式SJF的抢占发生于有比当前进程剩余时间片更短的进程到达时。
对一组指定的进程而言SJF是最优的，它给出了最短的平均等待时间。

时间片轮转算法按照时间片的长度依次调用进程直至全部进程结束。该调度关键点在于时间片大小的选取。
时间片的设置应考虑系统对响应时间的要求，就绪队列中的进程数目，系统的处理能力。







Process management

This library contains C language versions of three basic process management scheduling algorithms: first come first served scheduling algorithm, 
short job priority scheduling algorithm and time slice round robin scheduling algorithm.

The first-come, first-served scheduling algorithm schedules processes in the order in which they arrive.

The short job priority algorithm schedules according to the length of the remaining running time of the process. 
The shorter the time, the higher the priority and the order in which it is called.
The short job priority algorithm has two modes: preemptive SJF and non-preemptive SJF. 
Preemptive SJF preemption occurs when a process shorter than the remaining time slice of the current process arrives. 
SJF is optimal for a given set of processes, giving the shortest average waiting time.

The time slice rotation algorithm calls processes sequentially according to the length of the time slice until all processes end. 
The key point of this scheduling is the selection of time slice size. The setting of the time slice should consider the system's response time requirements, 
the number of processes in the ready queue, and the system's processing capabilities.
