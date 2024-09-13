#include "cpu/exec/helper.h"
#include "memory/memory.h"

#define DATA_BYTE 2
#include "ret-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "ret-template.h"
#undef DATA_BYTE

make_helper_v(ret_none)
make_helper_v(ret_i)
#include "cpu/exec/template-end.h"