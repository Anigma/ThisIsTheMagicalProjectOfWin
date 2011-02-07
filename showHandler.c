#include <assert.h>
#include <stdio.h>
#include "interrupt.h"
#include "ULT.h"

int main(int argc, char **argv)
{
	int i;
	registerHandler();
	while(1)
	{
		if(i++ % 1000000 == 0)
		{
			printf(".");
			fflush(stdout);
		}

		// SIGALRM is enabled when handler restores interrupted thread's state
		assert(alarmIsEnabled());
	}
}
