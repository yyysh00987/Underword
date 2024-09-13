#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute(){
	//printf("esp: %d\n",cpu.esp);
    int val = swaddr_read(cpu.esp, DATA_BYTE);//栈顶读数据
	//printf("val: %d\n",val);
    cpu.esp += DATA_BYTE; //让指针向上移动
//	printf("data byte: %d\n",DATA_BYTE);
	//printf("esp: %d\n",cpu.esp);
    OPERAND_W(op_src, val);//将读取的值 val 存储到 op_src 指定的操作数中。
    print_asm_template1();
}

make_instr_helper(rm)
make_instr_helper(r)

#include "cpu/exec/template-end.h"