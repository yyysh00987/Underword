#include "cpu/exec/template-start.h"

#define instr je

static void do_execute(){
   DATA_TYPE_S imm = op_src -> val;
 //  printf("\nimm:%d\n",imm);
    print_asm("je\t%x", cpu.eip + 1 + DATA_BYTE + imm);
    if (cpu.eflags.ZF == 1) cpu.eip += imm;
//    printf("\nZF:%d\n",cpu.eflags.ZF);
}

make_instr_helper(i)


#undef instr
/*------------------------------------------------------*/
#define instr jbe
static void do_execute(){
   DATA_TYPE_S imm = op_src->val;
   print_asm("jbe\t%x",cpu.eip + imm + DATA_BYTE + 1);
    if(cpu.eflags.ZF == 1 || cpu.eflags.CF == 1)
    cpu.eip += imm;
}
make_instr_helper(i)
 #undef instr
 /*-------------------------------------------------------*/

#define instr ja
static void do_execute() 
{
	DATA_TYPE_S imm = op_src -> val;
    print_asm("ja\t%x", cpu.eip + 1 + DATA_BYTE + imm);
    if (cpu.eflags.CF == 0 && cpu.eflags.ZF == 0) cpu.eip += imm;
}
make_instr_helper(i)
#undef instr

#define instr jle
static void do_execute() 
{	
	DATA_TYPE_S imm = op_src -> val;
    print_asm("jle\t%x", cpu.eip + 1 + DATA_BYTE + imm);
    if (cpu.eflags.ZF == 1 || cpu.eflags.SF != cpu.eflags.OF) cpu.eip += imm;
}
make_instr_helper(i)
#undef instr

// #define instr jae
// static void do_execute() 
// {
// 	DATA_TYPE_S imm = op_src -> val;
//     print_asm("jae\t%x", cpu.eip + 1 + DATA_BYTE + imm);
//     if (cpu.eflags.CF == 0) cpu.eip += imm;
// }
// make_instr_helper(i)
// #undef instr

// #define instr jb
// static void do_execute() 
// {
// 	DATA_TYPE_S imm = op_src -> val;
//     print_asm("jb\t%x", cpu.eip + 1 + DATA_BYTE + imm);
//     if (cpu.eflags.CF == 1) cpu.eip += imm;
// }
// make_instr_helper(i)
// #undef instr

// #define instr jg
// static void do_execute() 
// {
// 	DATA_TYPE_S imm = op_src -> val;
//     print_asm("jg\t%x", cpu.eip + 1 + DATA_BYTE + imm);
//     if (cpu.eflags.ZF == 0 && cpu.eflags.SF == cpu.eflags.OF) cpu.eip += imm;
// }
// make_instr_helper(i)
// #undef instr

// #define instr jge
// static void do_execute() 
// {
// 	DATA_TYPE_S imm = op_src -> val;
//     print_asm("jge\t%x", cpu.eip + 1 + DATA_BYTE + imm);
//     if (cpu.eflags.ZF == 1 || cpu.eflags.SF == cpu.eflags.OF) cpu.eip += imm;
// }
// make_instr_helper(i)
// #undef instr

// #define instr jl
// static void do_execute() 
// {
// 	DATA_TYPE_S imm = op_src -> val;
//     print_asm("jl\t%x", cpu.eip + 1 + DATA_BYTE + imm);
//     if (cpu.eflags.SF != cpu.eflags.OF) cpu.eip += imm;
// }
// make_instr_helper(i)
// #undef instr
// /*----------------------------------------------------*/
#define instr jne
static void do_execute() 
{
	DATA_TYPE_S imm = op_src -> val;
    print_asm("jne\t%x", cpu.eip + 1 + DATA_BYTE + imm);
    if (cpu.eflags.ZF == 0) cpu.eip += imm;
}
make_instr_helper(i)
#undef instr
// /*----------------------------------------------------*/
// #define instr jno
// static void do_execute() 
// {
// 	DATA_TYPE_S imm = op_src -> val;
// 	if (cpu.eflags.OF == 0)
// 		cpu.eip += imm;
// 	print_asm("jno\t%x", cpu.eip + 1 + DATA_BYTE + imm);
// }
// make_instr_helper(i)
// #undef instr
// /*----------------------------------------------------*/
// #define instr jnp
// static void do_execute() 
// {
// 	DATA_TYPE_S imm = op_src -> val;
// 	if (cpu.eflags.PF == 0)
// 		cpu.eip += imm;
// 	print_asm("jnp\t%x", cpu.eip + 1 + DATA_BYTE + imm);
// }
// make_instr_helper(i)
// #undef instr
// /*----------------------------------------------------*/
// #define instr jns
// static void do_execute() 
// {
// 	DATA_TYPE_S imm = op_src -> val;
//     print_asm("jns\t%x", cpu.eip + 1 + DATA_BYTE + imm);
//     if (cpu.eflags.SF == 0) cpu.eip += imm;
// }
// make_instr_helper(i)
// #undef instr
// /*----------------------------------------------------*/
// #define instr jo
// static void do_execute() 
// {
// 	DATA_TYPE_S imm = op_src -> val;
// 	if (cpu.eflags.OF == 1)
// 		cpu.eip += imm;
// 	print_asm("jo\t%x", cpu.eip + 1 + DATA_BYTE + imm);
// }
// make_instr_helper(i)
// #undef instr
// /*----------------------------------------------------*/
// #define instr jp
// static void do_execute() 
// {
// 	DATA_TYPE_S imm = op_src -> val;
// 	if (cpu.eflags.PF == 1)
// 		cpu.eip += imm;
// 	print_asm("jp\t%x", cpu.eip + 1 + DATA_BYTE + imm);
// }
// make_instr_helper(i)
// #undef instr
// /*----------------------------------------------------*/
// #define instr js
// static void do_execute() 
// {
// 	DATA_TYPE_S imm = op_src -> val;
//     print_asm("js\t%x", cpu.eip + 1 + DATA_BYTE + imm);
//     if (cpu.eflags.SF == 1) cpu.eip += imm;
// }
// make_instr_helper(i)
// #undef instr
// /*----------------------------------------------------*/

#include "cpu/exec/template-end.h"
