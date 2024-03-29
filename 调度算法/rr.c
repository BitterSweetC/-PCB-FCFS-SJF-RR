#include<stdio.h>
#include<stdlib.h>

#define getpch(type) (type*)malloc(sizeof(type))
#define NOTHING NULL
#define TIME 2//默认时间片大小 default time slice size 
 
typedef struct pcd {
    char name[10];
    char state;
    int ntime;
    int rtime;
    int etime;
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
			printf("\t输入有误，输入只能是正整数...\n");
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

void insert(){
	if(!ready){
		ready = p;
		pfend = p;
	}else{
		pfend->link = p;
		pfend = p;
	}
}

void input() {
    int i, num;
    printf("\n请输入进程数量：");
    num = geti();
    for (i = 0; i < num; i++) {
        printf("\n进程编号：%d\n", i + 1);
        p = getpch(PCB);
        printf("输入进程名字：");
        scanf("%s", p->name);
        printf("输入进程执行时间：");
        p->ntime = geti();
        printf("\n");
        p->rtime = 0;
        p->state = 'w';
        p->etime = TIME;
        p->link = NOTHING;
        insert();
    }
}

void rr(){
	// 如果就绪队列中只有一个进程，则直接返回
	// If there is only one process in the ready queue, return directly
	if(!ready->link) return; 
	else{	
	 	// 将当前执行的进程保存在变量currentProcess中
	 	// Save the currently executing process in the variable currentProcess
	 	PCB *currentProcess = ready;
		// 将当前执行的进程移到就绪队列尾部
		// Move the currently executing process to the end of the ready queue
		ready = ready->link;
		// 将之前执行的进程插入到就绪队列尾部
		// Insert the previously executing process to the end of the ready queue
		PCB *temp = ready;
		// 找到就绪队列的末尾
		// Find the end of the ready queue
		while(temp->link != NULL){ 
			temp = temp->link;
	}
	// 将之前执行的进程插入到就绪队列的末尾
	// Insert the previously executing process to the end of the ready queue
	temp->link = currentProcess; 
	 // 将之前执行的进程的链接置为NULL，表示已经插入到就绪队列的末尾
	 // Set the link of the previously executing process to NULL, indicating it has been inserted to the end of the ready queue
	currentProcess->link = NULL;
  }
}

void addnew(){
	ready->state = 'W';
	input(); 
}

void disp(PCB *pr) {
    printf("\nProcess Name\tState\tExecution Time\tExecuted Time\tRemaining Time Slice\t\n");
    printf("|%s\t\t", pr->name);
    printf("|%c\t", pr->state);
    printf("|%d\t\t", pr->ntime);
    printf("|%d\t\t", pr->rtime);
    printf("|%d\t", pr->etime);
    printf("\n");
}

void check() {
    PCB *pr;
    printf("\n当前运行进程：%s", ready->name);
    disp(ready);
    pr = ready->link;
    printf("\n当前就绪队列状态为:\n");
    while (pr != NOTHING) {
        disp(pr);
        pr = pr->link;
    }
}

void destroy() {
    printf("\n进程[%s]已完成.\n", ready->name);
    p = ready;
    ready = ready->link;
    free(p);
}

void running() {
    (ready->rtime)++;
    (ready->etime)--;
    check();
    if(ready->rtime == ready->ntime){
    	destroy();
    	return;
	}else if(ready->etime == 0){
		ready->state = 'w';
		ready->etime = TIME;
		rr();
	}
}

int main() {
    char ch;
    input();
    while (ready != NOTHING) {
        printf("\nThe execute name:%s\n",ready->name);
        ready->state = 'R';
        running();
        printf("\n按 i 键添加新进程....按其他任意键继续运行...");
        fflush(stdin);
        ch = getchar();
        if (ch == 'i' || ch == 'I') {
           addnew();
        }
       
    }
    printf("\n\n 进程已完成\n");
    getchar();
    return 0;
}

