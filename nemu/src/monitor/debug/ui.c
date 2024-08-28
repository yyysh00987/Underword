#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

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
	//1、分割args
	//2、把分割的后半部分换成int step
	//3、cpu_exec(step)
	char* tmp=args;
	char* part_args=strtok(tmp," ");
	int step;
	if(part_args==NULL){ //输入为空
		cpu_exec(-1);
		return 0;
	}
	sscanf(part_args,"%d",&step);
	cpu_exec(step);
	return 0;

}

static int cmd_info(char *args){
	char* tmp=args;
	strtok(tmp," ");
	char* part1=strtok(NULL," ");
	if(strcmp(part1,"r")==0){
		printf("eax   0x%x   %d\n",cpu.eax,cpu.eax);
		//eax, ecx, edx, ebx, esp, ebp, esi, edi
		printf("ecx   0x%x   %d\n",cpu.ecx,cpu.ecx);
		printf("ebx   0x%x   %d\n",cpu.ebx,cpu.ebx);
		printf("edx   0x%x   %d\n",cpu.edx,cpu.edx);
		printf("esp   0x%x   %d\n",cpu.esp,cpu.esp);
		printf("ebp   0x%x   %d\n",cpu.ebp,cpu.ebp);
		printf("esi   0x%x   %d\n",cpu.esi,cpu.esi);
		printf("edi   0x%x   %d\n",cpu.edi,cpu.edi);
	 
	}
	return 0;

}

static int cmd_x(char *args){
	char* tmp=args;
	char* part1_args=strtok(tmp," ");//N
	char* part2_args=strtok(NULL," "); //ADDR
	int n;
	int addr;
	sscanf(part1_args,"%d",&n);
	sscanf(part2_args,"%d",&addr);
	int i=0;
	for(;i<n;i++){
		int num=swaddr_read(addr, 4);
		printf("0x%x\n",num);
	}
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
	{"info ","r print all value of reg", cmd_info},
	{"x","x 10 0x10000,get 10*4 byte from 0x10000",cmd_x}

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
