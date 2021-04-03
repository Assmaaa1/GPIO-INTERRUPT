#include "stm32f4xx.h" // Device header

void systickDelayMs(int n);
 
/* MACRO FOR MAIN LOOP */
#define AHB1_CLK_EN (1<<0)|(1<<3)
#define APB2_CLK_EN (1<<14)
#define PIN_PD15_MODE (1<<30)|(0<<31)
#define EXTERNAL_INTERRUPT_LINE (1<<4)
#define UNMASK_INTERRUPT (1<<1)
#define FALLING_EDGE_EN (1<<1)

/* MACRO FOR INTERRUPTION FUNCTION */
#define SET_PD15 (1<<15)
#define RESET_PD15 (1<<31)
#define RESET_INERRUPT (1<<1)

int main(void)
{
	__disable_irq();
	RCC->AHB1ENR |= AHB1_CLK_EN; //Enable port A and port D
	RCC->APB2ENR	|=APB2_CLK_EN ;  //Enable SYSCFG
	GPIOD->MODER |=PIN_PD15_MODE;   // PD15 is output
	SYSCFG->EXTICR[0]|= EXTERNAL_INTERRUPT_LINE;	  //configure exti for PB1 pushbutton 
	EXTI->IMR |= UNMASK_INTERRUPT;  // select channel 1 for PB1
	EXTI->FTSR |=FALLING_EDGE_EN; //selecting falling edge
	NVIC_EnableIRQ(EXTI1_IRQn); //enable EXTI channel 1
	
	__enable_irq(); //enable global interrupt
	
	while(1){}	
}
void EXTI1_IRQHandler(void) //blink the led when interrupt detected
{ 
	GPIOD->BSRR |=SET_PD15;
	systickDelayMs(300);
	GPIOD->BSRR |=RESET_PD15;
	systickDelayMs(300);
	GPIOD->BSRR |=SET_PD15;
	systickDelayMs(300);
	GPIOD->BSRR |=RESET_PD15;
	systickDelayMs(300);
	EXTI->PR=RESET_INERRUPT; // clear the interrupt flag		
}
void systickDelayMs(int n)
{
	int i;
	SysTick->LOAD =8000-1;
	SysTick->VAL	=0;
	SysTick->CTRL =0x5;
	
	for(i=0;i<n;i++)
	{
		while((SysTick->CTRL & 0x10000)==0){}
	}
	SysTick->CTRL=0;
}
