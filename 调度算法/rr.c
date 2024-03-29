#include<stdio.h>
#include<stdlib.h>

#define getpch(type) (type*)malloc(sizeof(type))
#define NOTHING NULL
#define TIME 2//Ĭ��ʱ��Ƭ��С default time slice size 
 
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
			printf("\t������������ֻ����������...\n");
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
    printf("\n���������������");
    num = geti();
    for (i = 0; i < num; i++) {
        printf("\n���̱�ţ�%d\n", i + 1);
        p = getpch(PCB);
        printf("����������֣�");
        scanf("%s", p->name);
        printf("�������ִ��ʱ�䣺");
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
	// �������������ֻ��һ�����̣���ֱ�ӷ���
	// If there is only one process in the ready queue, return directly
	if(!ready->link) return; 
	else{	
	 	// ����ǰִ�еĽ��̱����ڱ���currentProcess��
	 	// Save the currently executing process in the variable currentProcess
	 	PCB *currentProcess = ready;
		// ����ǰִ�еĽ����Ƶ���������β��
		// Move the currently executing process to the end of the ready queue
		ready = ready->link;
		// ��֮ǰִ�еĽ��̲��뵽��������β��
		// Insert the previously executing process to the end of the ready queue
		PCB *temp = ready;
		// �ҵ��������е�ĩβ
		// Find the end of the ready queue
		while(temp->link != NULL){ 
			temp = temp->link;
	}
	// ��֮ǰִ�еĽ��̲��뵽�������е�ĩβ
	// Insert the previously executing process to the end of the ready queue
	temp->link = currentProcess; 
	 // ��֮ǰִ�еĽ��̵�������ΪNULL����ʾ�Ѿ����뵽�������е�ĩβ
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
    printf("\n��ǰ���н��̣�%s", ready->name);
    disp(ready);
    pr = ready->link;
    printf("\n��ǰ��������״̬Ϊ:\n");
    while (pr != NOTHING) {
        disp(pr);
        pr = pr->link;
    }
}

void destroy() {
    printf("\n����[%s]�����.\n", ready->name);
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
        printf("\n�� i ������½���....�������������������...");
        fflush(stdin);
        ch = getchar();
        if (ch == 'i' || ch == 'I') {
           addnew();
        }
       
    }
    printf("\n\n ���������\n");
    getchar();
    return 0;
}

