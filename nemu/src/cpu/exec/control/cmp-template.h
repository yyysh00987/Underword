#include "cpu/exec/template-start.h"

#define instr cmp 

void update_eflags_pf_zf_sf(uint32_t a);

static void do_execute(){
    DATA_TYPE a = op_dest->val;
    DATA_TYPE b = op_src->val;
    DATA_TYPE ans = a - b;

    cpu.eflags.CF = a < b;

    // update_eflags_pf_zf_sf(ans);
    const int parity_table [] = {
        0, 1, 0, 1,
        1, 0, 1, 0,
        0, 1, 0, 1,
        1, 0, 1, 0
    };

    uint8_t temp = ans & 0xff;
	cpu.eflags.PF = parity_table[temp & 0xf] ^ parity_table[temp >> 4];
	cpu.eflags.ZF = (ans == 0);
	cpu.eflags.SF = ans >> (DATA_BYTE * 8 - 1);

    int tem1 = (op_dest->val)>>((DATA_BYTE<<3)-1);
    int tem2 = (op_src->val)>>((DATA_BYTE<<3)-1);
    cpu.eflags.OF = (tem1 != tem2 && tem2 == cpu.eflags.SF);
   /* cpu.eflags.SF = MSB(ans);
    cpu.eflags.PF = get_pf(ans);
    cpu.eflags.ZF = (ans == 0);*/
    

    print_asm_template2();

}


#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif

make_instr_helper(r2rm)
make_instr_helper(rm2r)
make_instr_helper(i2rm)
make_instr_helper(i2a)

#include "cpu/exec/template-end.h"