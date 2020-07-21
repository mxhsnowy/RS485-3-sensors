#include <mega8.h>
#include <delay.h>
#include <alcd.h>
#include <stdio.h>
#include <string.h>

#define SLAVE_CB 0XA1
#define SLAVE_PC 0XFE
int i=0,cnt,flag_rev;
char buffer_send[8]={0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00};
char buffer_rev[8];
char buffer[50];

void Uart_Init()
{
   UBRRH=0;  
   UBRRL=0x67;    // taan so 9600 F 16MHZ  
   UCSRA=0x00;
   UCSRC=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
   UCSRB=(1<<RXEN)|(1<<TXEN)|(1<<RXCIE); 
}    
  
void Send_Char(char chr)
{
    while(UDRE == 0);
    UDR=chr;
    delay_ms(1); 
}

int to16(char H_byte , char L_byte)
{
  return ((H_byte << 8) | L_byte);
}

interrupt [USART_RXC] void usart_rx_isr(void)
{ 
	buffer_rev[cnt]=UDR; 
	cnt++;
    if(cnt >= 8)
    {  
		cnt = 0;
		flag_rev=1;
	}
          
}
void Send_Slave_CB()
{
  buffer_send[0]=SLAVE_CB;
  lcd_gotoxy(0,0);
  lcd_puts("Send Slave CB");
  for( i = 0 ; i < 8 ; i++)
  {
      buffer_rev[i] =0;
  }
  for( i = 0 ; i < 8 ; i++)
  {
      Send_Char(buffer_send[i]);
  }
  delay_ms(200);
}
void Send_Slave_PC(char check_byte)
{
    if(check_byte == 0x01)
    {
        buffer_send[0] = SLAVE_PC;
        buffer_send[1] =  0x01;
        for( i = 2 ; i<8 ;i++)
        {
          buffer_send[i]= buffer_rev[i];
        }
    }
    else 
    {  
		buffer_send[0] = SLAVE_PC;
		buffer_send[1] =  0x00; 
    }
    for( i = 0 ; i < 8 ; i++)
      {
          Send_Char(buffer_send[i]);
      }
	delay_ms(2000);
    cnt=0;
}

int To16( char H , char L)
{
  return (H << 8 | L );
}

void check_Data_Rev()
{
  lcd_gotoxy(0,0);
  lcd_puts("Send Slave PC");
  if(buffer_rev[0] == 0xA1)
  {
	  Send_Slave_PC(0x01);
	  lcd_gotoxy(0,1);
	  sprintf(buffer,"%4d %4d %4d" , To16(buffer_rev[2],buffer_rev[3]),To16(buffer_rev[4],buffer_rev[5]),To16(buffer_rev[6],buffer_rev[7]));
	  lcd_puts(buffer);
  }
  else 
  {
	  Send_Slave_PC(0x00);
	  lcd_gotoxy(0,1);
	  lcd_puts("MAT KET NOI CB    ");
  }
}

void main(void)
{
	Uart_Init();
	lcd_init(16);
	lcd_puts("MASTER");
	#asm("sei")
	while (1)
      { 
		Send_Slave_CB();
		check_Data_Rev();
      }
}
