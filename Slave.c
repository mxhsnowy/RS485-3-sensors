#include <mega8.h>
#include <delay.h>
#include <alcd.h>
#include <stdio.h>
#include <string.h>

#define add_device   0xA1

#define ADC_VREF_TYPE ((0<<REFS1) | (0<<REFS0) | (0<<ADLAR))
char buffer_send[20];
char buffer_rev[20];
int cb0, cb1,cb2;
char buffer[100];
float TEMP;
int flag_rev,i;
int cnt=0;
unsigned int read_adc(unsigned char adc_input)
{
ADMUX=adc_input | ADC_VREF_TYPE;
// Delay needed for the stabilization of the ADC input voltage
delay_us(10);
// Start the AD conversion
ADCSRA|=(1<<ADSC);
// Wait for the AD conversion to complete
while ((ADCSRA & (1<<ADIF))==0);
ADCSRA|=(1<<ADIF);
return ADCW;
}



void ADC_init()
{
    ADMUX=ADC_VREF_TYPE;
    ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADFR) | (0<<ADIF) | (0<<ADIE) | (1<<ADPS2) | (0<<ADPS1) | (0<<ADPS0);
    SFIOR=(0<<ACME);
}

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
void debug(char *str, int len)
{

for( i = 0 ; i <  len ; i ++)   
      Send_Char(str[i]);


}
int to16(char H_byte , char L_byte)
{
  return ((H_byte << 8) | L_byte);
}
 interrupt [USART_RXC] void usart_rx_isr(void)
{
  //  if(UDR==add_device)flag_send=1;  
    buffer_rev[cnt]=UDR; 
    cnt++;
    if(cnt >= 8)
    {  
       cnt = 0;
      flag_rev=1;
    }      
}
void main(void)
{
    Uart_Init();
    ADC_init();
    lcd_init(16);
    lcd_puts("T     CB1    CB3");
    #asm("sei")
    while (1)
      {
        cb0=read_adc(0); 
        cb1=read_adc(1); 
        cb2=read_adc(2);   
        // Slave frame 
        buffer_send[0] = add_device;     buffer_send[1]= 0x00;
        buffer_send[2] = (cb0 >> 8);  buffer_send[3] =  (cb0 & 0x00FF);
        buffer_send[4] = (cb1 >> 8);  buffer_send[5] =  (cb1 & 0x00FF);
        buffer_send[6] = (cb2 >> 8);   buffer_send[7] =  (cb2 & 0x00FF); 
        TEMP = (cb0*4.8*10);
        TEMP = (float)TEMP/1023;
        TEMP = TEMP*10; 
        sprintf(buffer,"%4d %4d  %4d",cb0,cb1,cb2);
        lcd_gotoxy(0,1);
        lcd_puts(buffer);
        memset(buffer,0,100);
        delay_ms(100);
       
     
        if(flag_rev)
        {
         flag_rev=0; 
       
         if( buffer_rev [0] == add_device)
         {      
            for(i = 0 ; i < 8 ;i++)
                 Send_Char(buffer_send[i]);
                 
         }
        }
      }
}
