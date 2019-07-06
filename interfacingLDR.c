// interfacing ldr and dht 11 with pic18f887

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

  //DHT11 Pins
sbit DHT11_Pin at RC5_bit;
sbit DHT11_Dir at TRISC5_bit;


 //Variables declarations
unsigned int adc_value,value,temp_res;
unsigned char digit;

unsigned int DHT11_TMP;       // 16 bits data
unsigned int DHT11_HUM;     // 16 bits data
char DHT11_CHKSM;          // 8 bit data
  /*unsigned long read_data;*/
  char *voltage = "00.00";
  long temp;
  char *text,txt[7],hum[7];
  unsigned char a;
  unsigned int hum_value, temp_value, chk_sum;

  void ShowADC(int x, int y, unsigned int adc_value);
  void ShowData();
   void DHT11_Start();
   void DHT11_init();
   void DHT11_Read();
   unsigned char DataReceived();

  
  void main()
  {

  TRISA = 0xFF; // PORTA is input as it receives values from ldr
  TRISC.RC5=1;  // PORTC pin5 is input as it receives data from dth11
  Lcd_Init();   // Initialize LCD

    while(1)
    {
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1, 1, " LDR :");    //Display string on LCD position 1,1
    adc_value = ADC_Read(2);    // Get 10-bit results of AD conversion      (The LDR output is connected to PORTA.2 / ADC port)
    ShowADC(1,7,adc_value);    //row=1, column=7, char value to be displayed = adc_value
    delay_ms(500);
    ShowData();
    }
  }

  // Routine to show the value of the ADC_read
  void ShowADC(int x, int y, unsigned int adc_value)
  {
  Lcd_Cmd(_LCD_CURSOR_OFF);
  digit = adc_value / 1000;                           //MSB (thousands place digit)
  LCD_Chr(x,y,48+digit);                              //display its ASCII value at position of x, y
  adc_value=adc_value-1000*digit;
  digit = (adc_value / 100);                          //(hundreds place)
  LCD_Chr_CP(48+digit);
  adc_value=adc_value-100*digit;
  digit = (adc_value / 10);                           //tens place value
  LCD_Chr_CP(48+digit);                               //display character at current position
  adc_value=adc_value-10*digit;                       //units place value
  digit = adc_value;
  LCD_Chr_CP(48+digit);
  delay_ms(30);

  }

  void StartSignal()
  {
   TRISC.RC5 = 0;    //Configure RC5 as output
   PORTC.RC5 = 1;
   PORTC.RC5 = 0;    //RC5 sends 0 to the sensor
   delay_ms(18);
   PORTC.RC5 = 1;    //RC5 sends 1 to the sensor
   delay_us(30);     //duration of start bit
   TRISC.RC5 = 1;    //Configure RC5 as input

   }

  void CheckResponse()
  {
   a = 0;
   while(!PORTC.RC5);         //0 is received at port C
   while (PORTC.RC5);          // 1 is recieved at port C
   a=1;
   }

   void ShowData() {
   StartSignal();
   CheckResponse();
   if(a == 1)
   {
      DHT11_Read();
    
    if(DataReceived()!=7)
    {
     Lcd_Cmd(_LCD_CURSOR_OFF);        // cursor off
     Lcd_Cmd(_LCD_CLEAR);             // clear LCD
     inttostr(hum_value,txt);
     text = " Humidity: ";
     Lcd_Out(1,1,text);
     lcd_out(1,10,txt);
     delay_ms(500);
     
     Lcd_Cmd(_LCD_CURSOR_OFF);        // cursor off
     Lcd_Cmd(_LCD_CLEAR);             // clear LCD
     inttostr(temp_value,txt);
     text = " Temp: ";
     Lcd_Out(1,1,text);
     lcd_out(1,8,txt);
     delay_ms(500);
     
      Lcd_Cmd(_LCD_CURSOR_OFF);        // cursor off
     Lcd_Cmd(_LCD_CLEAR);             // clear LCD
     inttostr(chk_sum,txt);
     text = " CheckSum: ";
     Lcd_Out(1,1,text);
     lcd_out(1,10,txt);
     delay_ms(500);
   }
}
   
    else {
     Lcd_Cmd(_LCD_CURSOR_OFF);        // cursor off
    Lcd_Cmd(_LCD_CLEAR);
    text="No response";
    Lcd_Out(1,3,text);
    text = "from DHT11";
    Lcd_Out(2,1,text);
    }
    delay_ms(2000);
    }
    

  unsigned char WaitForHigh()
  {
   char sayac=0;
   while(DHT11_Pin==0)
                      {
                       sayac++;
                       delay_us(1);
                       if(sayac>100)
                       return 0;
                      }

  }
  
  unsigned char WaitForLow()
  {
   char sayac=0;
   while(DHT11_Pin==1)
     {
                       sayac++;
                       delay_us(1);
                       if(sayac>100)                
                       return 0;
                      }
  }
  
  unsigned char DataReceived()
  {

   char count=0;
   while(DHT11_Pin==1)
     {
                       count++;
                       delay_us(1);
                      }

   if(count>=26 && count<=28)           // The data received is '0'  if low(50us) then high(26-28us)
   return 0;
   if(count>28 && count<=70)                       // The data received is '1'  if low(50us) then high(70us)
   return 1;
   else
   return 7;
  }
  

   void DHT11_Read()
  {
   char i=0,datar=0;
    hum_value=0;
    temp_value=0;
    chk_sum=0;

  if(DataReceived()!=7)
  {
   for(i=0;i<40;i++)
   {
   if(i<16)
        {
          while(WaitForHigh());
         datar = DataReceived();
         hum_value= hum_value + datar*(2^i);
        }
        
   if(i>=16 && i<32)
        {
            while(WaitForHigh());
            datar = DataReceived();
            temp_value= temp_value + datar*(2^(i-16));

        }
   if(i>=32&& i<40)
   {
            while(WaitForHigh());
            datar = DataReceived();
            chk_sum = chk_sum + datar*(2^(i-32));
   }
   }DHT11_DIR=0;
//   DHT11_Pin=1;
  }
  
  else
  { Lcd_Cmd(_LCD_CURSOR_OFF);        // cursor off
    Lcd_Cmd(_LCD_CLEAR);
    text="Erroneous data";
    Lcd_Out(1,3,text);
    text = "from DHT11";
    Lcd_Out(2,1,text);
    delay_ms(500);
    }
  
  }
  


  
  ///////////////////////////////////////////////////
   void DHT11_Start()
  {
     dht11_dir=0;//baslangic
     dht11_pin=1;
     delay_ms(50);
     dht11_pin=0;
     delay_ms(18);
     dht11_pin=1;
     dht11_dir=1;
     delay_us(40);
     waitforhigh();
     waitforlow();
  }
  ///////////////////////////////////////////////////
   void DHT11_init()
  {
   DHT11_DIR=0;
   DHT11_Pin=1;
  }