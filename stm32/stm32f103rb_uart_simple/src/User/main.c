/**********************************
 * project: STM32F10X Simple UART
 *    file: main.c
 **********************************/

#include "stm32f10x.h"

#define UART1			USART1
#define UART1_GPIO		GPIOA
#define UART1_RxPin		GPIO_Pin_10
#define UART1_TxPin		GPIO_Pin_9

void UART1_init(void);
void uart_putc(uint8_t c);
void uart_puts(uint8_t *s);
uint8_t uart_getc(void);

int main()
{
	UART1_init();

	uart_puts("hello stm32!\r\n");

	while(1)
	{
		// loop back test
		uart_putc( uart_getc() );
	}

	return 0;
}

void UART1_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* enable clocks on peripherals */
	RCC_APB2PeriphClockCmd(
			RCC_APB2Periph_GPIOA |	// turn on GPIOA
			RCC_APB2Periph_USART1 |	// turn on USART1
			RCC_APB2Periph_AFIO,	// turn on alternate function
			ENABLE);

	/* Configure RX pin as input floating */
	GPIO_InitStructure.GPIO_Pin = UART1_RxPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(UART1_GPIO, &GPIO_InitStructure);
	/* Configure TX pin as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = UART1_TxPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(UART1_GPIO, &GPIO_InitStructure);

	/* configure serial port settings */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART1, &USART_InitStructure);

	/* enable UART1 peripheral */
	USART_Cmd(UART1, ENABLE);
}

void uart_putc(uint8_t c)
{
	// place the character to the data register
	USART_SendData(UART1, c);
	// wait until TXE=1 (data has been moved to the shift register)
	while(USART_GetFlagStatus(UART1, USART_FLAG_TXE) == RESET);
}

void uart_puts(uint8_t *s)
{
	while(*s) uart_putc(*s++);
}

uint8_t uart_getc(void)
{
	// wait until a character is received
	while(USART_GetFlagStatus(UART1, USART_FLAG_RXNE) == RESET);
	// get the character from the data register
	return (USART_ReceiveData(UART1) & 0xFF);
}
