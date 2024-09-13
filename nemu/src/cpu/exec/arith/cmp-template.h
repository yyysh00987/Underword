#include "cpu/exec/template-start.h"

#define instr cmp

void update_eflags_pf_zf_sf(uint32_t a);



static void do_execute(){
    printf("eax:%d\n",cpu.eax);
    printf("ecx:%d\n",cpu.ecx);
    DATA_TYPE a = op_dest->val;
    DATA_TYPE b = op_src->val;
    DATA_TYPE ans = a - b;
    printf("a:%d\n",a);
    printf("b:%d\n",b);
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



// DATA_TYPE result = op_dest->val - op_src->val;    //前一个操作数 - 后一个（无符号数）
//     //检查无符号数溢出
//     if(op_dest->val < op_src->val) cpu.eflags.CF = 1;
//     else cpu.eflags.CF = 0;
//     // = 结果的符号位
//     cpu.eflags.SF = result >> (DATA_BYTE*8 -1);
//     //检查有符号数溢出
//     int a = op_dest->val >> (DATA_BYTE*8 - 1);
//     int b = op_src->val >> (DATA_BYTE*8 - 1);
//     // a - b  = result
//     // +   -        -
//     // -   +        +
//     //因此，当a和b符号不同，结果和b的符号相同时，有符号数溢出
//     cpu.eflags.OF = ((a != b) && (b == cpu.eflags.SF));
//     // 结果==0 为1
//     cpu.eflags.ZF = !result;
//     // 结果的最低有效字节包含偶数个1 为1
//     result ^= result >>4;
// 	result ^= result >>2;
// 	result ^= result >>1;
// 	cpu.eflags.PF = !(result & 1);
//     //D3到D4是否进位
//     if(( (op_dest->val & 0xf) - (op_src->val & 0xf) ) >> 4) cpu.eflags.AF = 1;
//     else cpu.eflags.AF = 0;
//     print_asm_template2();
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif

make_instr_helper(r2rm)
make_instr_helper(rm2r)
make_instr_helper(i2rm)
make_instr_helper(i2a)

#include "cpu/exec/template-end.h"