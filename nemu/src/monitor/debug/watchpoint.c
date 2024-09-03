#include "monitor/watchpoint.h"
#include "monitor/expr.h"

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

WP* new_wp(char *e){//从 free_链表中返回一个空闲的监视点结构给head链表(放在尾巴上)，且将表达式，表达式的值赋给该监视点结构

	if(free_==NULL){assert(0);}//确保有空节点

	WP *tmp=free_;//获取当前空监视点
	free_=free_->next;
	tmp->next=NULL;

	bool success = false;
	tmp->expr=strdup(e);
	tmp->old_val = expr(tmp->expr, &success);
	assert(success);

	if(head==NULL){
		head=tmp;
	}
	else{
		WP* h=head;
		while(h->next){
			h=h->next;
		}
	h->next=tmp;
	}

	return tmp;

}

bool free_wp(WP *p){//将一个活的监视点释放回free链表中
	if(p==NULL){
		return 0;
	}
	if(p==head){
		head=p->next;
	}
	else{
		WP *h=head;
		while(h->next!=p){
			h=h->next;
		}
		if(h==NULL) return 0;

		h->next=p->next;
	}
		p->next=free_;
		free_ = p;
		return 1;
}

bool test_change(){
		WP* p=head;
		bool success=true;
		int val;

		while (p->next)
		{
			p=p->next;
			val=expr(p->expr,&success);
			assert(success);
			if(val==p->old_val)return true;
		}
		
	return false;
}

void print_wp(){
	WP* p=head;
		bool success=true;
		int val;

		while (p->next)
		{
			p=p->next;
			val=expr(p->expr,&success);
			assert(success);
			printf("%d %s %d",p->NO,p->expr,p->old_val);
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


int set_watchpoint(char *e){
	uint32_t val;
	bool  success=true;
	val=expr(e,&success);
	assert(!success);	

	WP *p= new_wp(e);
	p->expr=strdup(e);
	p->old_val=val;

	p->next=head;
	head=p;//插到开头
	return p->NO;//返回新监视点的编号
}

bool delete_watchpoint(int NO){
	WP *p;
	WP *former=NULL;

	for(p=head;p!=NULL;former=p,p=p->next){
		if(p->NO==NO) break;
	}

	if(p==NULL){//没有这个监控点
		return false;
	}
	if(former==NULL){
		head=p->next;
	}
	else{
		former->next=p->next;
	}

	free_wp(p);
	return 1;

}

void list_watchpoint() {//列出所有监视点
	if(head == NULL) {//没有活着的监控点
		printf("No watchpoints\n");
		return;
	}

	else{
		printf("%8s\t%8s\t%8s\n", "NO", "Address", "Enable");
		 WP *p;
    for(p = head; p != NULL; p = p->next) {

        printf("%8d\t%s\t%#08x\n", p->NO, p->expr, p->old_val);
    }
	}


}