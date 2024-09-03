#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>


void cpu_exec(uint32_t);

WP* new_wp(char *e);



/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1); //c是运行，-1转换为无符号数表示最大整数
	//因为不要si无限执行，所以-1肯定要改
	//再去cpu_exec()里面看看这个参数跟执行什么时候end的关系
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);


static int cmd_si(char *args){//args 是字符串，要分成si 和 N
	//1、接收r
	//2、把r换成int step
	//3、cpu_exec(step)
	int step;
	if(args==NULL)step=1;
	sscanf(args,"%d",&step);
	cpu_exec(step);
	return 0;
}


static int cmd_info(char *args){
	if(strcmp(args,"r")==0){


printf("eax     0x%08x      %-10d\n", cpu.eax, cpu.eax);
printf("ecx     0x%08x      %-10d\n", cpu.ecx, cpu.ecx);
printf("ebx     0x%08x      %-10d\n", cpu.ebx, cpu.ebx);
printf("edx     0x%08x      %-10d\n", cpu.edx, cpu.edx);
printf("esp     0x%08x      %-10d\n", cpu.esp, cpu.esp);
printf("ebp     0x%08x      %-10d\n", cpu.ebp, cpu.ebp);
printf("esi     0x%08x      %-10d\n", cpu.esi, cpu.esi);
printf("edi     0x%08x      %-10d\n", cpu.edi, cpu.edi);
printf("eip     0x%08x      %-10d\n", cpu.eip, cpu.eip);

	}
	
	return 0;

}



static int cmd_x(char *args){
	char* tmp=args;
	char* part1_args=strtok(tmp," ");//N
	char* part2_args=strtok(NULL," "); //ADDR
	int n;
	swaddr_t addr; //这里卡了，一开始写的类型是int,没有注意函数要的类型是swaddr_t
	sscanf(part1_args,"%d",&n);
	sscanf(part2_args,"%x",&addr); //这里也卡了，弄成了%d，然后才改成%x
	printf("0x%x:",addr);
	int u=1;
	for(;u<=n;u++){
		swaddr_t num=swaddr_read(addr, 4);
		printf("0x%08x  ",num);
		addr+=4;//这里一开始写成了addr*=u;实际上应该是4，4，4的挪
	}
	return 0;
}

static int cmd_p(char *args){
bool success=true;
 uint32_t val =expr(args,&success);//调用expr函数
 if(success){
	printf("The result is 0x%x (%u)\n",val,val);

 }
 return 0;

}



static int cmd_w(char *args){
	WP* n_wp;
	n_wp=new_wp(args);
	printf("Set a new watchpoint %d:%s.\n",n_wp->NO,n_wp->expr);
	return 0;
}

static int cmd_d(char *args){
	char *arg=strtok(NULL," ");
	int num=0;
	sscanf(arg,"%d",&num);
	bool res=delete_watchpoint(num);
	if(res){
		printf("delete %d",num);
	}
	else
		printf("no %d :(",num);

	return 0;
}


static struct {
	char *name;
	char *description;
	int (*handler) (char *); //函数指针
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{"si","go on step by step",cmd_si},
	{"info","r 打印 w 监控 d删除监控点", cmd_info},
	{"x","x 10 0x10000,get 10*4 byte from 0x10000",cmd_x},
	{"p","culcuate the exp",cmd_p},
	{ "d", "Delete watchpoint", cmd_d },
	{"w","这是啥来这我也忘了",cmd_w}

	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
