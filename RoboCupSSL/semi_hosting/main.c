#include <stm32f4xx.h>

int main(void)
	{
		int x = 56;
		while(1)
		{
			printf("%d", x);
			SH_SendChar('\n');
		}
	}
