#include "trap.h"
//ok
int main() {
	int i = 1, sum = 0;
	while(i <= 100) {
		sum += i;
		i ++;
	}

	nemu_assert(sum == 5050);

	return 0;
}
