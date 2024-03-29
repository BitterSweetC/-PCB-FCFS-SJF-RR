#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define getpch(type) (type*)malloc(sizeof(type))
#define NOTHING NULL

typedef struct pcd {
    char name[10];
    char state;
    int ntime;
    int rtime;
    struct pcd *link;
} PCB;

PCB *ready = NOTHING, *pfend = NOTHING, *p = NOTHING;

int geti() {
    char ch;
    int i = 0;
    fflush(stdin);
    ch = getchar();
    while(ch == '\n'){
        fflush(stdin);
        ch = getchar();
    }
    
    while(ch != '\n'){
        if(ch > '9' || ch < '0'){
            printf("\tInput error, please enter an integer...\n");
            fflush(stdin);
            i = 0;
            ch = getchar();
        }else{
            i = i * 10 +(ch - '0');
            ch = getchar();
        }
    }
    return i;
}

void sjf(){
    PCB *tp, *tempp;
    // �����������Ϊ��
    // If the ready queue is empty
    if(!ready){
    	// ����ǰ��������Ϊ�������еĵ�һ������
    	// Set the current process as the first process in the ready queue
        ready = p; 
        // ���¾������е����һ������
        // Update the last process of the ready queue
        pfend = p; 
    }else{
    	// ��ʼ��ָ���Ա�����������
    	// Initialize a pointer to traverse the ready queue
        tp = ready; 
        // ��ʼ��ָ���Ը��ٱ��������е�ǰһ���ڵ�
        // Initialize a pointer to keep track of the previous node while traversing
        tempp = NULL; 
        // �����������У�ֱ������ĩβ���ҵ�ִ��ʱ����̵Ľ���
        // Traverse the ready queue until reaching the end or finding a process with shorter execution time
        while(tp && p->ntime >= tp->ntime){ 
         // ����ǰһ���ڵ�ָ��
         // Update the previous node pointer
            tempp = tp;
            // �ƶ������������е���һ���ڵ�
            // Move to the next node in the ready queue
            tp = tp->link; 
        }
        // �������λ���ھ������еĿ�ͷ
        // If the insertion position is at the beginning of the ready queue
        if(!tempp){ 
        // ����ǰ�������ӵ��������е����ಿ��
        // Link the current process to the rest of the ready queue
            p->link = ready; 
            // ���¾�������ָ��Ϊ��ǰ����
            // Update the ready queue pointer to the current process
            ready = p; 
        }else{
        	// ��ǰһ���������ӵ���ǰ����
        	// Link the previous process to the current process
            tempp->link = p; 
            // ����ǰ�������ӵ��������е����ಿ��
             // Link the current process to the rest of the ready queue
            p->link = tp; 
        }
    }
}

void input() {
    int i, num;
    printf("\nEnter the number of processes: ");
    num = geti();
    for (i = 0; i < num; i++) {
        printf("\nProcess %d\n", i + 1);
        p = getpch(PCB);
        printf("Enter process name: ");
        scanf("%s", p->name);
        printf("Enter execution time: ");
        p->ntime = geti();
        p->rtime = 0;
        p->state = 'W';
        p->link = NOTHING;
        sjf();
    }
}

void disp(PCB *pr) {
	printf("\nProcess Name\tState\tExecution Time\tExecuted Time\t\n");
    printf("|%s\t", pr->name);
    printf("|%c\t", pr->state);
    printf("|%d\t", pr->ntime);
    printf("|%d\t", pr->rtime);
    printf("\n");
}

void check() {
    PCB *pr;
    printf("\nCurrent running process: %s", ready->name);
    disp(ready);
    pr = ready->link;
    printf("\nProcesses in ready queue:\n");
    while (pr != NOTHING) {
        disp(pr);
        pr = pr->link;
    }
}

void destroy() {
    printf("\nProcess [%s] completed.\n", ready->name);
    p = ready;
    ready = ready->link;
    free(p);
}

void running() {
    ready->state = 'R';
    (ready->rtime)++;
    check();
    if(ready->rtime == ready->ntime){
        destroy();
    }
}

int main() {
    char ch;
    input();
    while (ready != NOTHING) {
        printf("\n\nExecuting process: %s\n",ready->name);
        running();
        printf("\nPress 'i' to add new process, any other key to continue...");
        fflush(stdin);
        ch = getchar();
        if (ch == 'i' || ch == 'I') {
            input();
        }
    }
    printf("\n\nAll processes completed.\n");
    getchar();
    return 0;
}

