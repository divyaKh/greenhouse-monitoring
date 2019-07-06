// interfacing ldr and lm35 with pic18f887

  //LCD Pins
sbit LCD_RS at RD0_bit;
sbit LCD_EN at RD1_bit;
sbit LCD_D4 at RD2_bit;
sbit LCD_D5 at RD3_bit;
sbit LCD_D6 at RD4_bit;
sbit LCD_D7 at RD5_bit;


sbit LCD_RS_Direction at TRISD0_bit;
sbit LCD_EN_Direction at TRISD1_bit;
sbit LCD_D4_Direction at TRISD2_bit;
sbit LCD_D5_Direction at TRISD3_bit;
sbit LCD_D6_Direction at TRISD4_bit;
sbit LCD_D7_Direction at TRISD5_bit;



 //Variables declarations
unsigned int adc_value;
unsigned char digit;

  /*unsigned long read_data;*/
  char *voltage = "00.00";
  void ShowADC(int x, int y, unsigned int adc_value);
  char txt[15];
  float value;

  void main()
  {
   ADCON1=0x80;
  TRISA = 0xFF; // PORTA is input as it receives values from ldr
  TRISE= 0xFF;
  Lcd_Init();   // Initialize LCD

    while(1)
    {
    
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1, 1, "Temp :");    //Display string on LCD position 1,1
    adc_value = adc_read(5);    // Get 10-bit results of AD conversion      (The LM35 output is connected to PORTA.0 / ADC port)
    Lcd_Cmd(_LCD_CURSOR_OFF);
    
    ShowADC(1,7,adc_value);    //row=1, column=7, char value to be displayed = adc_value
    delay_ms(500);
    
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1, 1, "LDR:");    //Display string on LCD position 1,1
    adc_value = adc_read(2);    // Get 10-bit results of AD conversion      (The LDR output is connected to PORTA.2 / ADC port)
    ShowADC(1,7,adc_value);    //row=1, column=7, char value to be displayed = adc_value
    delay_ms(500);
    

    }
  }

  // Routine to show the value of the ADC_read
  void ShowADC(int x, int y, unsigned int adc_value)
  {
  Lcd_Cmd(_LCD_CURSOR_OFF);
  value = adc_value * 0.4887;                   //This 10-bit digital value is then converted to the corresponding voltage by multiplying with 0.4887
 /*ie,
0v  =  0000000000
5v  =  1111111111
Resolution = (Vref+ – Vref-)/(1024-1)   (as it is 10 bit ADC)
= 5/1023
= 4.887 mV*/
 
   FloatToStr(value,txt);            // Convert temperature to string
    txt[4] = 0;
    Lcd_Out(2,3,txt);                 // Write string in second row
    Delay_ms(300);
  }