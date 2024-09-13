#include "cpu/exec/template-start.h"

#define instr cmp 

static void do_execute () {
	printf("eax: %d\n",cpu.eax);

	printf("ecx: %d\n",cpu.ecx);


    printf("op_dest: %d\n",op_dest->val);

    printf("op_src: %d\n",op_src->val);
	DATA_TYPE result = op_dest->val - op_src->val;

	update_eflags_pf_zf_sf((DATA_TYPE_S)result);
	cpu.eflags.CF = result > op_dest->val;
	cpu.eflags.OF = MSB((op_dest->val ^ op_src->val) & (op_dest->val ^ result));

	print_asm_template2();
}


make_instr_helper(r2rm)
make_instr_helper(rm2r)
make_instr_helper(i2rm)
make_instr_helper(i2a)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif

#include "cpu/exec/template-end.h"