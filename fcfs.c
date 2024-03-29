#include<stdio.h>
#include<stdlib.h>

#define getpch(type) (type*)malloc(sizeof(type)) // 定义宏，用于动态分配内存空间
#define NOTHING 0 // 定义常量

// 进程控制块 PCB 结构体
typedef struct pcd {
    char name[10]; // 进程名
    char state;    // 进程状态
    int ntime;     // 执行时间
    int rtime;     // 已执行时间
    struct pcd *link; // 链接下一个进程控制块的指针
} PCB;
//ready:第一个PCB,pfend:最后一个PCB,p:临时PCB,stop:当前被阻塞的进程,stopend:阻塞队列最后一个PCB,方便添加新的被阻塞进程
PCB *ready = NOTHING, *pfend = NOTHING, *p = NOTHING, *stop = NOTHING, *stopend = NOTHING; 

// 获取输入的整数
int geti() {
    char ch;
    int i = 0;
    fflush(stdin); // 刷新输入缓冲区
    ch = getchar(); // 获取字符输入
    while (ch == '\n') {
        fflush(stdin); // 刷新输入缓冲区
        ch = getchar(); // 获取字符输入
    }
    while (ch != '\n') {
        if (ch > '9' || ch < '0') {
            printf("\t输入有误，输入只能是正整数...\n");
            fflush(stdin); // 刷新输入缓冲区
            i = 0;
            ch = getchar(); // 获取字符输入
        } else {
            i = i * 10 + (ch - '0');//将字符转化为整数 
            ch = getchar(); // 获取字符输入
        }
    }
    return i;
}

// 先来先服务调度算法
void fcfs() {
    if (!ready) { // 如果就绪队列为空
        ready = p; // 将当前进程作为第一个进程加入就绪队列
        pfend = p; // 更新就绪队列的最后一个进程为当前进程
    } else { 
        p->link = pfend->link; // 将当前进程的链接指向就绪队列中第一个进程之前的位置
        pfend->link = p; // 将就绪队列的最后一个进程指向当前进程
        pfend = p; // 更新就绪队列的最后一个进程为当前进程
    }
}

// 输入进程信息
void input() {
    int i, num;
    printf("\n请输入进程数量：");
    num = geti();
    for (i = 0; i < num; i++) {
        printf("\n 进程编号：%d\n", i + 1);
        p = getpch(PCB); // 动态分配内存空间给进程控制块
        printf("输入进程名字：");
        scanf("%s", p->name); // 获取进程名
        printf("输入进程执行时间：");
        p->ntime = geti(); // 获取进程执行时间
        printf("\n");
        p->rtime = 0;
        p->state = 'w'; 
        p->link = NOTHING; 
        fcfs(); 
    }
}

void block() {
    if (!stop) { // 如果阻塞队列为空
        ready->state = 'b'; // 将就绪队列中的进程状态置为阻塞状态
        stop = ready; // 将当前就绪队列中的进程移到阻塞队列
        stopend = ready; // 更新阻塞队列的最后一个进程为当前进程
        ready = ready->link; // 就绪队列指向下一个进程
        stopend->link = NOTHING; // 将当前进程的链接置为空
    } else {
        ready->state = 'b'; // 将就绪队列中的进程状态置为阻塞状态
        stopend->link = ready; // 将当前就绪队列中的进程移到阻塞队列
        stopend = ready; // 更新阻塞队列的最后一个进程为当前进程
        ready = ready->link; // 就绪队列指向下一个进程
        stopend->link = NOTHING; // 将当前进程的链接置为空
    }
}

void disp(PCB *pr) {
    printf("\n进程名\t状态\t执行时间\t已执行时间\t\n");
    printf("|%s\t", pr->name);
    printf("|%c\t", pr->state);
    printf("|%d\t", pr->ntime);
    printf("|%d\t", pr->rtime);
    printf("\n");
}

void check() {
    PCB *pr; 
    printf("\n当前运行进程：%s", ready->name); 
    disp(ready); 
    pr = ready->link; // 将就绪队列中的下一个进程赋值给 pr
    printf("\n当前就绪队列状态为:\n"); 
    while (pr != NOTHING) { // 遍历就绪队列中的所有进程
        disp(pr);
        pr = pr->link; 
    }
    printf("\n阻塞队列状态为：\n"); 
    pr = stop; // 将阻塞队列中的第一个进程赋值给 pr
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
    (ready->rtime)++; // 已执行时间加1
    check(); 
    if (ready->rtime == ready->ntime) { // 若已执行时间等于执行时间，销毁进程
        destroy();
        return;
    }
}

void wake() {
    if (stop == NOTHING) { 
        printf("\n阻塞队列为空，请进行其他操作");
        return; 
    } else { // 如果阻塞队列不为空
        p = stop; // 将阻塞队列的第一个进程赋值给临时指针变量 p
        stop = stop->link; // 将阻塞队列指向下一个进程
        if (ready == NOTHING) { // 如果就绪队列为空
            ready = p; // 将当前进程设置为就绪队列的第一个进程
            pfend = p; // 更新就绪队列的最后一个进程为当前进程
            ready->link = NOTHING; // 将当前进程的链接置为空
        } else { 
            pfend->link = p; // 将当前进程插入到就绪队列的末尾
            pfend = p; // 更新就绪队列的最后一个进程为当前进程
            pfend->link = NOTHING; // 将当前进程的链接置为空
        }
    }
}

int main() {
    char ch;
    input(); 
    while (ready != NOTHING || stop != NOTHING) { // 当就绪队列或阻塞队列不为空时
        if (ready != NOTHING) {
            printf("\n当前执行进程：%s\n", ready->name);
            ready->state = 'R'; // 将当前执行进程状态置为运行状态
            running(); 
        } else {
            check(); 
        }
        printf("\n按 i 键添加新进程....按其他任意键继续运行...");
        fflush(stdin);
        ch = getchar();
        if (ch == 'i' || ch == 'I') {
            input(); // 添加新进程
        }
        if (ch == 's' || ch == 'S') {
            block(); // 阻塞进程
        }
        if (ch == 'w' || ch == 'W') {
            wake(); // 唤醒进程
        }
    }
    printf("\n\n 进程已完成\n");
    getchar();
    return 0;
}

