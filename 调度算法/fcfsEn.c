#include<stdio.h>
#include<stdlib.h>

#define getpch(type) (type*)malloc(sizeof(type)) // Define macro for dynamic memory allocation
#define NOTHING 0 // Define constant

// Process Control Block (PCB) structure
typedef struct pcb {
    char name[10]; // Process name
    char state;    // Process state
    int ntime;     // Execution time
    int rtime;     // Executed time
    struct pcb *link; // Link to the next PCB
} PCB;

// 'ready': first PCB, 'pfend': last PCB, 'p': temporary PCB, 'stop': currently blocked process, 'stopend': last PCB in blocked queue, for adding new blocked process
PCB *ready = NOTHING, *pfend = NOTHING, *p = NOTHING, *stop = NOTHING, *stopend = NOTHING;

// Get input integer
int geti() {
    char ch;
    int i = 0;
    fflush(stdin); // Flush input buffer
    ch = getchar(); // Get character input
    while (ch == '\n') {
        fflush(stdin); // Flush input buffer
        ch = getchar(); // Get character input
    }
    while (ch != '\n') {
        if (ch > '9' || ch < '0') {
            printf("\tInput error, input should be a positive integer...\n");
            fflush(stdin); // Flush input buffer
            i = 0;
            ch = getchar(); // Get character input
        } else {
            i = i * 10 + (ch - '0'); // Convert character to integer
            ch = getchar(); // Get character input
        }
    }
    return i;
}

// First Come First Serve (FCFS) scheduling algorithm
void fcfs() {
    if (!ready) { // If the ready queue is empty
        ready = p; // Make the current process the first process in the ready queue
        pfend = p; // Update the last process in the ready queue
    } else {
        p->link = pfend->link; // Set the link of the current process to the position before the first process in the ready queue
        pfend->link = p; // Set the link of the last process in the ready queue to the current process
        pfend = p; // Update the last process in the ready queue
    }
}

// Input process information
void input() {
    int i, num;
    printf("\nEnter the number of processes: ");
    num = geti();
    for (i = 0; i < num; i++) {
        printf("\nProcess Number: %d\n", i + 1);
        p = getpch(PCB); // Dynamically allocate memory for PCB
        printf("Enter process name: ");
        scanf("%s", p->name); // Get process name
        printf("Enter process execution time: ");
        p->ntime = geti(); // Get process execution time
        printf("\n");
        p->rtime = 0;
        p->state = 'w'; // Set initial state to 'w' (waiting)
        p->link = NOTHING; // Set link to NULL
        fcfs(); // Add process to ready queue
    }
}

// Block a process
void block() {
    if (!stop) { // If the block queue is empty
        ready->state = 'b'; // Set the state of the process in the ready queue to blocked
        stop = ready; // Move the current process from ready queue to block queue
        stopend = ready; // Update the last process in the block queue
        ready = ready->link; // Move to the next process in ready queue
        stopend->link = NOTHING; // Set the link of the current process to NULL
    } else {
        ready->state = 'b'; // Set the state of the process in the ready queue to blocked
        stopend->link = ready; // Move the current process from ready queue to block queue
        stopend = ready; // Update the last process in the block queue
        ready = ready->link; // Move to the next process in ready queue
        stopend->link = NOTHING; // Set the link of the current process to NULL
    }
}

// Display process information
void disp(PCB *pr) {
    printf("\nProcess Name\tState\tExecution Time\tExecuted Time\t\n");
    printf("|%s\t", pr->name);
    printf("|%c\t", pr->state);
    printf("|%d\t", pr->ntime);
    printf("|%d\t", pr->rtime);
    printf("\n");
}

// Check process status
void check() {
    PCB *pr;
    printf("\nCurrent running process: %s", ready->name);
    disp(ready);
    pr = ready->link; // Assign the next process in the ready queue to pr
    printf("\nCurrent ready queue status:\n");
    while (pr != NOTHING) { // Traverse all processes in the ready queue
        disp(pr);
        pr = pr->link;
    }
    printf("\nBlocked queue status:\n");
    pr = stop; // Assign the first process in the block queue to pr
    while (pr != NOTHING) {
        disp(pr);
        pr = pr->link;
    }
}

// Destroy process
void destroy() {
    printf("\nProcess [%s] has completed.\n", ready->name);
    p = ready;
    ready = ready->link;
    free(p);
}

// Execute process
void running() {
    (ready->rtime)++; // Increment executed time
    check();
    if (ready->rtime == ready->ntime) { // If executed time equals execution time, destroy process
        destroy();
        return;
    }
}

// Wake up process
void wake() {
    if (stop == NOTHING) {
        printf("\nBlocked queue is empty, perform other operations.");
        return;
    } else { // If block queue is not empty
        p = stop; // Assign the first process in the block queue to temporary pointer p
        stop = stop->link; // Move to the next process in the block queue
        if (ready == NOTHING) { // If ready queue is empty
            ready = p; // Set the current process as the first process in the ready queue
            pfend = p; // Update the last process in the ready queue
            ready->link = NOTHING; // Set the link of the current process to NULL
        } else {
            pfend->link = p; // Insert the current process at the end of the ready queue
            pfend = p; // Update the last process in the ready queue
            pfend->link = NOTHING; // Set the link of the current process to NULL
        }
    }
}

int main() {
    char ch;
    input();
    while (ready != NOTHING || stop != NOTHING) { // When ready queue or block queue is not empty
        if (ready != NOTHING) {
            printf("\nCurrent running process: %s\n", ready->name);
            ready->state = 'R'; // Set state of current running process to running
            running();
        } else {
            check();
        }
        printf("\nPress 'i' to add new process....Press any other key to continue running...");
        fflush(stdin);
        ch = getchar();
        if (ch == 'i' || ch == 'I') {
            input(); // Add new process
        }
        if (ch == 's' || ch == 'S') {
            block(); // Block process
        }
        if (ch == 'w' || ch == 'W') {
            wake(); // Wake up process
        }
    }
    printf("\n\nProcesses have completed.\n");
    getchar();
    return 0;
}

