#include "cpu/exec/template-start.h"

#define instr call

make_helper(concat(call_i_, SUFFIX)) {          // SS:2
    //decode_i_v 译码, len = 操作数的长度 = 本指令长度 - 1
    int len = concat(decode_i_, SUFFIX) (eip + 1);
    //开栈
    reg_l (R_ESP) -= DATA_BYTE;
    //*rsp = eip + len
    swaddr_write (reg_l (R_ESP) , 4 , cpu.eip + len);   //ret的时候，新eip = 此时的eip+len，然后ret命令长度是1,运行完ret后eip再加上这个1,刚好是call的下一条指令
    //偏移量
    DATA_TYPE_S imm = op_src->val;
    //打印汇编
    print_asm("call %x",cpu.eip + 1 + len + imm);
    //修改eip
    cpu.eip += imm;
    return len + 1;
    //函数返回该指令长度之后，eip再加上该指令的长度len+1
}

make_helper(concat(call_rm_, SUFFIX)) {
	int len = concat(decode_rm_, SUFFIX) (eip + 1);
	cpu.esp -= DATA_BYTE;
	swaddr_write (cpu.esp , 4 , cpu.eip + len);
	DATA_TYPE_S imm = op_src->val;
	print_asm("call %x",imm);
	cpu.eip = imm - len - 1;
	return len + 1;
	print_asm_template1();
}


#include "cpu/exec/template-end.h"