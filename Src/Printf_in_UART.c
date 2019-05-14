#include "Printf_in_UART.h"

#ifdef __cplusplus
extern "C" {
#endif

#if (USE_IDE == VisualGDB)
#include "sys\stat.h"
	
	int _fstat(int fd, struct stat *pStat)
	{
		pStat->st_mode = S_IFCHR;
		return 0;
	}

	int _close(int fd)
	{
		return -1;
	}

	int _isatty(int fd)
	{
		return 1;
	}

	int _lseek(int fd, int offset, int origin)
	{
		return -1;
	}
	
	int _write(int fd, char *pBuffer, int size)
	{
		for (int i = 0; i < size; i++)
		{
			while (__HAL_UART_GET_FLAG(&USE_USART, UART_FLAG_TXE) == RESET);
			//while (HAL_UART_GetState(&USE_USART) == HAL_UART_STATE_READY);
			HAL_UART_Transmit(&USE_USART, &pBuffer[i], sizeof(char), 1000);
		}
		return size;
	}

	int _read(int fd, char *pBuffer, int size)
	{
		for (int i = 0; i < size; i++)
		{
			//while (__HAL_UART_GET_FLAG(&USE_USART, UART_FLAG_RXNE) == RESET);
			//while (HAL_UART_GetState(&USE_USART) == HAL_UART_STATE_READY);
			HAL_UART_Receive(&USE_USART, &pBuffer[i], sizeof(char), 1000);
		}
		return size;
	}
	/*
	caddr_t _sbrk(int increment)
	{
		extern char end asm("end");
		register char *pStack asm("sp");

		static char *s_pHeapEnd;

		if (!s_pHeapEnd)
			s_pHeapEnd = &end;

		if (s_pHeapEnd + increment > pStack)
			return (caddr_t)-1;

		char *pOldHeapEnd = s_pHeapEnd;
		s_pHeapEnd += increment;
		return (caddr_t)pOldHeapEnd;
	}
	*/
#elif(USE_IDE == IARorKeil)
	///重定向c库函数printf到串口DEBUG_USART，重定向后可使用printf函数
	int fputc(int ch, FILE *f)
	{
		/* 发送一个字节数据到串口DEBUG_USART */
		HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 1000);

		return (ch);
	}

	///重定向c库函数scanf到串口DEBUG_USART，重写向后可使用scanf、getchar等函数
	int fgetc(FILE *f)
	{
		int ch;
		HAL_UART_Receive(&huart1, (uint8_t *)&ch, 1, 1000);
		return (ch);
	}

#endif //USE_IDE

/*****************  发送字符串 **********************/
void Usart_SendString(uint8_t *str)
{
	unsigned int k = 0;
	do
	{
		HAL_UART_Transmit(&huart1, (uint8_t *)(str + k), 1, 1000);
		k++;
	} while (*(str + k) != '\0');

}

void Receive_Return(void)
{
	uint8_t Receive_Buff[256];
	char    Receive_temp;
	static uint8_t Receive_Count = 0;

#if 1
	if (__HAL_UART_GET_FLAG(&USE_USART, UART_FLAG_RXNE) == SET)
	{
		HAL_UART_Receive(&USE_USART, &Receive_temp, 1, 1000);
		if (Receive_Count < sizeof(Receive_Buff))
		{
			Receive_Buff[Receive_Count++] = Receive_temp;
		}
		else
			Receive_Count = 0;

		if (Receive_temp == 0x0A)
		{
			HAL_UART_Transmit(&USE_USART, &Receive_Buff, Receive_Count, 1000);
			Receive_Count = 0;
		}
	}
#else
	scanf("%s", Receive_Buff);
	printf("%s\n", Receive_Buff);
#endif
	
}

#ifdef __cplusplus
}
#endif