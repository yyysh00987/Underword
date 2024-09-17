#include "cpu/exec/template-start.h"

#define exec_template static void do_execute() {\
	if (IF_COND) OPERAND_W(op_dest, op_src->val);\
	print_asm_template2();\
}\
make_instr_helper(rm2r)


#define instr cmova
#define IF_COND cpu.eflags.CF == 0 && cpu.eflags.ZF == 0
exec_template;
#undef IF_COND
#undef instr


#define instr cmove
#define IF_COND cpu.eflags.ZF == 1
exec_template;
#undef IF_COND
#undef instr


#define instr cmovbe
#define IF_COND cpu.eflags.CF == 1 || cpu.eflags.ZF == 1
exec_template;
#undef IF_COND
#undef instr


#define instr cmovge
#define IF_COND cpu.eflags.SF == cpu.eflags.OF
exec_template;
#undef IF_COND
#undef instr


#define instr cmovne
#define IF_COND cpu.eflags.ZF == 0
exec_template;
#undef IF_COND
#undef instr


#define instr cmovns
#define IF_COND cpu.eflags.SF == 0
exec_template;
#undef IF_COND
#undef instr


#define instr cmovs
#define IF_COND cpu.eflags.SF == 1
exec_template;
#undef IF_COND
#undef instr



#include "cpu/exec/template-end.h"