#include "utility.h"

void test(char* string, int boolean)
{
	if(boolean)
	{
		printColor(" [PASS] ", 1, 6);
		printf("%s\n", string);
	}
	else
	{
		printColor(" [FAIL] ", 1, 3);
		printf("%s\n", string);
	}
}

void error(char* string)
{
	printf("%s", string);
	exit(-1);
}

void* mallocSafely(size_t size)
{
	if(size==0) error("I refuse to allocate nothing");
	//let's use calloc for now, for testing purposes, if we need the speed, undo later
	//void* temp = malloc(size); 
	void* temp = calloc(size, 1);
	if(temp==NULL) error("Could not allocate memory!");
	return temp;
}

ucontext_t* getContext()
{
	ucontext_t* temp = (ucontext_t*) mallocSafely(sizeof(ucontext_t));
	int err = getcontext(temp);
	if(err) error("Could not get context!");
	return temp;
}

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

void printUContext(ucontext_t* mycontext) 
{
	uint sssp = getSSSPFromContext(mycontext);
	uint eip  = getEIPFromContext(mycontext);
	uint esp  = getESPFromContext(mycontext);
	uint uesp = getUESPFromContext(mycontext);

	printf("%.8x Address of the EIP in mycontext\n", eip);
	printf("%.8x Value of sssp\n", sssp);
	printf("%.8x Value of ESP in greg\n", esp);
	printf("%.8x Value of UESP in greg\n", uesp);
}

void printColor(char* string, int attribute, int color)
{
	/* 
	Attributes
		0	None
		1	Bold
		4	Underscore
		5	Blink
		7	Reverse
		8	Concealed
	Colors
		0	Black
		1	Red
		2	Green
		3	Yellow
		4	Blue
		5	Magenta
		6	Cyan
		7	White
	*/

	//printf("%c[%d;%d;%dm%s", 0x1B, attribute, 40+background, 30+foreground, string);
	printf("%c[%d;%dm%s", 0x1B, attribute, 30+color, string);
	printf("%c[0m",0x1B);
}

