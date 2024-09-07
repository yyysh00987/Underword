#include "cpu/exec/template-start.h"
#include "cpu/helper.h"

#define instr push

static void do_execute(){
    int len;
    if(DATA_BYTE == 2) len = 2;
    else len = 4;
    cpu.esp -=len;
    swaddr_write(cpu.esp, len ,op_src->val);
    print_asm_template1();
}

#if DATA_BYTE == 2 || DATA_BYTE == 4

make_instr_helper(r)
#endif

make_instr_helper(i)

#include "cpu/exec/template-end.h"