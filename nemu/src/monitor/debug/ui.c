#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

char *in_which_func(swaddr_t addr);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static	char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");
//	line_read = readline("");
	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

int nr_exp = 0, nr_wp = 0; 

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {

	return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args);

static int cmd_info(char *args);

static int cmd_x(char *args);

static int cmd_p(char *args);

static int cmd_w(char *args);

static int cmd_d(char *args);

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "si", "The program pauses after executing N instructions in a single step(when N is not given, it defaults to 1)", cmd_si},
	{ "info", "info  r : Print register status, \ninfo w :Print watchpoints.",cmd_info},
	{ "x", "Examine memory: x SIZE ADDRESS.", cmd_x},
	{ "p", "Print value of the expression", cmd_p},
	{ "w", "Set a watchpoint for an expression.", cmd_w},
	{ "d", "Delete a watchpoint.", cmd_d},
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

static int cmd_si(char *args){
	char *arg = strtok(NULL," ");
	if(arg==NULL){
		cpu_exec(1);
		printf("0x%x\n",cpu.eip);
		return 0;
	}
	char *endstr = NULL;
	long int num=strtol(arg,&endstr,0);
	if((*endstr) != '\0'){
		printf("Invalid number %s.\n",args);
		return 0;
	}
	if(num > (uint32_t)(-1)){
		printf("Numeric constant too large.\n");
		return 0;
	}
	cpu_exec(num);
	printf("0x%x\n",cpu.eip);
	return 0;
}
static int cmd_info(char *args){
	char *arg = strtok(NULL, " ");
	if(strlen(arg)>1){
		printf("Undefined info command: \"%s\".  Try \"help info\".\n",args);
		return 0;
	}
	if(*arg == 'r'){
		int i;
		for(i = R_EAX; i <= R_EDI; i++){
			printf("%s\t0x%.8x\t%.10u\n", regsl[i], cpu.gpr[i]._32, cpu.gpr[i]._32);
		}printf("eip\t0x%.8x\t%.10u\n",cpu.eip,cpu.eip);
	}
	else if(*arg == 'w'){
		// todo
		print_wp();	
	}else{
		printf("Undefined info command: \"%s\".  Try \"help info\".\n",args);
		return 0;
	}
	return 0;
}

static int cmd_x(char *args){
	int len = 0;
	char exp[1024];
	if(args==NULL || sscanf(args, "%d %[^\n]", &len, exp)!=2){
		printf("Invalid Arguments %s.\n",args);
		return 0;
	}
	bool success = true;
	swaddr_t addr = expr(exp, &success);
	if(!success){
		printf("Invalid Address %s.\n",exp);
		return 0;
	}
	int i;
	for(i = 0;i < len; i++){
		printf("0x%08x:\t\t0x%08x\n", addr + i * 4, swaddr_read(addr + i * 4, 4));
	}
	return 0;
}

static int cmd_p(char *args){
	bool success = true;
	uint32_t val = expr(args , &success);
	if(success){
		printf("$%d = %u (0x%x)\n", nr_exp, val, val);
	//	printf("%u\n",val);
		nr_exp++;
	}
	return 0;
}

static int cmd_w(char *args){
	bool success = true;
	uint32_t val = expr(args , &success);
	if(!success){
		printf("Invalid expression!\n");
		return 0;
	}
	WP *wp = new_wp();
	if(wp != NULL){
		strcpy(wp->exp,args);
		wp->old_val = val;
		wp->new_val = val;
		wp->NO = nr_wp;
		printf("watchpoint %d : %s\n", nr_wp, args);
		nr_wp++;
	}else{
		printf("Too many watchpoint!\n");
	}
	return 0;
}	
static int cmd_d(char *args){
	int no;
	if(sscanf(args,"%d",&no) != 1){
		printf("Invalid Argumnts!");
		return 0;
	}
	delete_wp(no);	
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
