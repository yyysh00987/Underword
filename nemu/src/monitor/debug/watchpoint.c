#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#include "nemu.h"

#define NR_WP 32

static WP wp_pool[NR_WP];

static WP *head, *free_;

void init_wp_pool() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
// 在 ui.c 文件的开头或相关头文件中

WP* new_wp(){//从 free_链表中返回一个空闲的监视点结构给head链表(放在尾巴上)，且将表达式，表达式的值赋给该监视点结构

	// if(free_==NULL){assert(0);}//确保有空节点

	// WP *tmp=free_;//获取当前空监视点
	// free_=free_->next;
	// tmp->next=NULL;

	// bool success = false;
	// tmp->val=strdup(e);
	// tmp->val = expr(tmp->val, &success);
	// assert(success);

	// if(head==NULL){
	// 	head=tmp;
	// }
	// else{
	// 	WP* h=head;
	// 	while(h->next!=NULL){
	// 		h=h->next;
	// 	}
	// h->next=tmp;
	//
//	return tmp;}

	WP *f,*p;
	f = free_;
	free_ = free_->next;
	f->next = NULL;
	p = head;
	if (p == NULL){head = f;p = head;}
	else {
		while (p->next!=NULL)p=p->next;
		p->next = f;
		}
	return f;


}

void free_wp(WP *wp){//将一个活的监视点释放回free链表中
	WP *h,*p;
	p=free_;
	if(p==NULL){
		free_=wp;
		p=free_;
	}
	else{
		while (p->next!=NULL)p=p->next;
		p->next = wp;
	}
	h=head;
	if(head==NULL){
		assert(0);
	}
	if (head->NO == wp->NO)
	{
	head = head->next;
	}
	else 
	{
	while (h->next != NULL && h->next->NO != wp->NO)
	{
		h = h->next;
		}
	if (h->next == NULL && h->NO == wp->NO)printf ("what hell");
	else if (h->next->NO == wp->NO)h->next = h->next->next;
	else assert (0);
	
	}
	wp->next = NULL;
	wp->val = 0;
	wp->b = 0;
	wp->expr[0] = '\0';
}

	

bool check_wp()
{
	WP *f;
	f = head;
	bool key = true;
	bool suc;
	while (f != NULL)
	{
		uint32_t tmp_expr = expr (f->expr,&suc);
		if (!suc)assert (1);
		if (tmp_expr != f->val)
		{
			key = false;
			if (f->b)
			{
				printf ("Hit breakpoint %d at 0x%08x\n",f->b, cpu.eip);
				f = f->next;
				continue;
			}
			printf ("Watchpoint %d: %s\n",f->NO,f->expr);
			printf ("Old value = %d\n",f->val);
			printf ("New value = %d\n",tmp_expr);
			f->val = tmp_expr;
		}
		f = f->next;
	}
	return key;
}

void info_wp()
{
	WP *f;
	f=head;
	while (f!=NULL)
	{
		printf ("Watchpoint %d: %s = %d\n",f->NO,f->expr,f->val);
		f = f->next;
	}
}


//1申请新空 2记录表达式
// 3当cpu_exec()执行完一条指令，对表达式求值：expr
/*while(执行结束前){
cpu_exec(1);
??
if() break;
}*/
//4比较表达式变化否 ，5如果是就咋听程序且nemu_state 变量设置为 STOP
// 6输出“Hint ,会ui_mainloop()
// watchpoint 0 at address 0x00100014”，其中 0 和 0x00100014 会根据
// 监视点的不同而不同，其中“0”表示所触发的监视点的编号，“0x00100014”
// 表示触发监视点指令的%eip 的取值（必须使用 16 进制表示）。
//命令：info w打印    d 删除


void delete_wp(int num)
{
	WP *f;
	f = &wp_pool[num];
	free_wp (f);
}