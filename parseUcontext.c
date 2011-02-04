#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef __USE_GNU
#define __USE_GNU
#endif
#define SOL 99
#include <ucontext.h>


uint getSSSPFromContext(ucontext_t *uc)
{
	return (uint) ((uc->uc_stack).ss_sp);
}

uint getEIPFromContext(ucontext_t *uc)
{
	return (uint) (uc->uc_mcontext).gregs[REG_EIP];
}

// Craig: I think we want to use this one....
uint getESPFromContext(ucontext_t *uc)
{
	return (uint) (uc->uc_mcontext).gregs[REG_ESP];
}

uint getUESPFromContext(ucontext_t *uc)
{
	return (uint) (uc->uc_mcontext).gregs[REG_UESP];
}

int main(int argc, char **argv)
{
	// Get your context
	ucontext_t mycontext;
	int err = getcontext(&mycontext);

	// Do this project on a 32-bit x86 linux machine
	assert(__WORDSIZE == 32);
	assert(NGREG == 19);  
	assert(sizeof(mycontext) == 348);

	uint sssp = getSSSPFromContext(&mycontext);
	uint eip  = getEIPFromContext(&mycontext);
	uint esp  = getESPFromContext(&mycontext);
	uint uesp = getUESPFromContext(&mycontext);

	printf("%.8x Address of main()\n", (uint) &main);
	printf("%.8x Address of the EIP in mycontext\n", eip);
	printf("%.8x Address of err\n", (uint) &err);
	printf("%.8x Address of argc\n", (uint) &argc);
	printf("%.8x Value of sssp\n", sssp);
	printf("%.8x Value of ESP in greg\n", esp);
	printf("%.8x Value of UESP in greg\n", uesp);
	printf("%.8x Bytes between argc and err\n", (uint) (&argc - &err) );
	printf("%.8x Bytes between err and sp in context\n",  ((uint)&err) - ((uint)(getESPFromContext(&mycontext))));
	return 0;  
}

