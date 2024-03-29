#include<stdio.h>
#include<stdlib.h>

#define getpch(type) (type*)malloc(sizeof(type)) // ����꣬���ڶ�̬�����ڴ�ռ�
#define NOTHING 0 // ���峣��

// ���̿��ƿ� PCB �ṹ��
typedef struct pcd {
    char name[10]; // ������
    char state;    // ����״̬
    int ntime;     // ִ��ʱ��
    int rtime;     // ��ִ��ʱ��
    struct pcd *link; // ������һ�����̿��ƿ��ָ��
} PCB;
//ready:��һ��PCB,pfend:���һ��PCB,p:��ʱPCB,stop:��ǰ�������Ľ���,stopend:�����������һ��PCB,��������µı���������
PCB *ready = NOTHING, *pfend = NOTHING, *p = NOTHING, *stop = NOTHING, *stopend = NOTHING; 

// ��ȡ���������
int geti() {
    char ch;
    int i = 0;
    fflush(stdin); // ˢ�����뻺����
    ch = getchar(); // ��ȡ�ַ�����
    while (ch == '\n') {
        fflush(stdin); // ˢ�����뻺����
        ch = getchar(); // ��ȡ�ַ�����
    }
    while (ch != '\n') {
        if (ch > '9' || ch < '0') {
            printf("\t������������ֻ����������...\n");
            fflush(stdin); // ˢ�����뻺����
            i = 0;
            ch = getchar(); // ��ȡ�ַ�����
        } else {
            i = i * 10 + (ch - '0');//���ַ�ת��Ϊ���� 
            ch = getchar(); // ��ȡ�ַ�����
        }
    }
    return i;
}

// �����ȷ�������㷨
void fcfs() {
    if (!ready) { // �����������Ϊ��
        ready = p; // ����ǰ������Ϊ��һ�����̼����������
        pfend = p; // ���¾������е����һ������Ϊ��ǰ����
    } else { 
        p->link = pfend->link; // ����ǰ���̵�����ָ����������е�һ������֮ǰ��λ��
        pfend->link = p; // ���������е����һ������ָ��ǰ����
        pfend = p; // ���¾������е����һ������Ϊ��ǰ����
    }
}

// ���������Ϣ
void input() {
    int i, num;
    printf("\n���������������");
    num = geti();
    for (i = 0; i < num; i++) {
        printf("\n ���̱�ţ�%d\n", i + 1);
        p = getpch(PCB); // ��̬�����ڴ�ռ�����̿��ƿ�
        printf("����������֣�");
        scanf("%s", p->name); // ��ȡ������
        printf("�������ִ��ʱ�䣺");
        p->ntime = geti(); // ��ȡ����ִ��ʱ��
        printf("\n");
        p->rtime = 0;
        p->state = 'w'; 
        p->link = NOTHING; 
        fcfs(); 
    }
}

void block() {
    if (!stop) { // �����������Ϊ��
        ready->state = 'b'; // �����������еĽ���״̬��Ϊ����״̬
        stop = ready; // ����ǰ���������еĽ����Ƶ���������
        stopend = ready; // �����������е����һ������Ϊ��ǰ����
        ready = ready->link; // ��������ָ����һ������
        stopend->link = NOTHING; // ����ǰ���̵�������Ϊ��
    } else {
        ready->state = 'b'; // �����������еĽ���״̬��Ϊ����״̬
        stopend->link = ready; // ����ǰ���������еĽ����Ƶ���������
        stopend = ready; // �����������е����һ������Ϊ��ǰ����
        ready = ready->link; // ��������ָ����һ������
        stopend->link = NOTHING; // ����ǰ���̵�������Ϊ��
    }
}

void disp(PCB *pr) {
    printf("\n������\t״̬\tִ��ʱ��\t��ִ��ʱ��\t\n");
    printf("|%s\t", pr->name);
    printf("|%c\t", pr->state);
    printf("|%d\t", pr->ntime);
    printf("|%d\t", pr->rtime);
    printf("\n");
}

void check() {
    PCB *pr; 
    printf("\n��ǰ���н��̣�%s", ready->name); 
    disp(ready); 
    pr = ready->link; // �����������е���һ�����̸�ֵ�� pr
    printf("\n��ǰ��������״̬Ϊ:\n"); 
    while (pr != NOTHING) { // �������������е����н���
        disp(pr);
        pr = pr->link; 
    }
    printf("\n��������״̬Ϊ��\n"); 
    pr = stop; // �����������еĵ�һ�����̸�ֵ�� pr
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
    (ready->rtime)++; // ��ִ��ʱ���1
    check(); 
    if (ready->rtime == ready->ntime) { // ����ִ��ʱ�����ִ��ʱ�䣬���ٽ���
        destroy();
        return;
    }
}

void wake() {
    if (stop == NOTHING) { 
        printf("\n��������Ϊ�գ��������������");
        return; 
    } else { // ����������в�Ϊ��
        p = stop; // ���������еĵ�һ�����̸�ֵ����ʱָ����� p
        stop = stop->link; // ����������ָ����һ������
        if (ready == NOTHING) { // �����������Ϊ��
            ready = p; // ����ǰ��������Ϊ�������еĵ�һ������
            pfend = p; // ���¾������е����һ������Ϊ��ǰ����
            ready->link = NOTHING; // ����ǰ���̵�������Ϊ��
        } else { 
            pfend->link = p; // ����ǰ���̲��뵽�������е�ĩβ
            pfend = p; // ���¾������е����һ������Ϊ��ǰ����
            pfend->link = NOTHING; // ����ǰ���̵�������Ϊ��
        }
    }
}

int main() {
    char ch;
    input(); 
    while (ready != NOTHING || stop != NOTHING) { // ���������л��������в�Ϊ��ʱ
        if (ready != NOTHING) {
            printf("\n��ǰִ�н��̣�%s\n", ready->name);
            ready->state = 'R'; // ����ǰִ�н���״̬��Ϊ����״̬
            running(); 
        } else {
            check(); 
        }
        printf("\n�� i ������½���....�������������������...");
        fflush(stdin);
        ch = getchar();
        if (ch == 'i' || ch == 'I') {
            input(); // ����½���
        }
        if (ch == 's' || ch == 'S') {
            block(); // ��������
        }
        if (ch == 'w' || ch == 'W') {
            wake(); // ���ѽ���
        }
    }
    printf("\n\n ���������\n");
    getchar();
    return 0;
}

