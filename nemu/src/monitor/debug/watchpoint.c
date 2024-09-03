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

	if(free_==NULL)return NULL;//确保有空节点

	WP *tmp=free_;//获取当前空监视点
	free_=free_->next;
	

	if(head==NULL){
		head=tmp;
		tmp->next=NULL;
	}
	else{
		tmp->next=head;
		head=tmp;
		}
	
	return tmp;
}




void free_wp(WP *wp){//将一个活的监视点释放回free链表中
	if(wp == head){
		head = wp->next;
	}else{
		WP *p=head;
		while(p->next != wp)
		      p = p->next;
		p->next = wp->next;
	}
	if(free_ == NULL){
		free_ = wp;
		wp->next = NULL;
	}else{
		wp->next = free_;
		free_ = wp;
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



bool check_wp(swaddr_t eip)
{	WP *wp = head;
	bool success = false, has_wp = false;
	while(wp != NULL){
		success = true;
		wp->new_val = expr(wp->exp,&success);
		assert(success);
		wp = wp->next;
	}
	wp = head;
	while(wp != NULL){
		if(wp->new_val != wp->old_val){
			has_wp = true;
			printf("Hint watchpoint %d at address 0x%.8x , expr = %s\n\nOld value = %d (0x%x)\nNew value = %d (0x%x)\n", wp->NO, cpu.eip, wp->exp, wp->old_val, wp->old_val, wp->new_val, wp->new_val);
			wp->old_val = wp->new_val;
		}
		wp = wp->next;
	}
	return has_wp;
}

void info_wp()
{	
	printf("NO\tType      \tExpression\n");
	WP *wp_temp = head;
	while(wp_temp != NULL){
		printf("%d\twatchpoint\t%s\n",wp_temp->NO,wp_temp->exp);
		wp_temp = wp_temp->next;
	}
}

void delete_wp(int no)
{
	WP *wp_temp = head;
	while(wp_temp != NULL){
		if(wp_temp->NO == no){
			free_wp(wp_temp);
			printf("Deleted watchponit %d.\n",no);
			return ;
		}
		wp_temp = wp_temp->next;
	}
	if(wp_temp == NULL)
	      printf("NO watchponit %d !\n",no);
}