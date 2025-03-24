#ifndef UART_H
#define UART_H

// USART1
void configureUART1(int baudRate);
void sendUART1(char *sendBuffer);
void receiveUART1(int nBytes, char *receiveBuffer);

// USART2
void configureUART2(int baudRate);
void sendUART2(char *sendBuffer);
void receiveUART2(int nBytes,  char *receiveBuffer);

#endif // UART_H