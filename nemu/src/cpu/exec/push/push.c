#include "cpu/exec/helper.h"
#include "memory/memory.h"

#define instr push

#define DATA_BYTE 1
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "push-template.h"
#undef DATA_BYTE


make_helper_v(push_r)
