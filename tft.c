/*
The demo is in 8 bit mode,
DB9-DB16 to pin D0-D7 ; DB0 – DB8 to GND ; LDEA to 3.3V  ; VCC to 5V ; GND to GND
RESET to A2
CS    to A3
WR    to A4
RS    to A5
*/


#include <bcm2835.h>
#include <stdio.h>
 
#define LCD_RS   RPI_V2_GPIO_P1_35  //G19
#define LCD_WR   RPI_V2_GPIO_P1_12  //G18
#define LCD_CS   RPI_V2_GPIO_P1_11  //G17
#define LCD_REST RPI_V2_GPIO_P1_36  //G16

#define D0 RPI_V2_GPIO_P1_38 //G20
#define D1 RPI_V2_GPIO_P1_40 //G21
#define D2 RPI_V2_GPIO_P1_15 //G22
#define D3 RPI_V2_GPIO_P1_16 //G23
#define D4 RPI_V2_GPIO_P1_18 //G24
#define D5 RPI_V2_GPIO_P1_22 //G25
#define D6 RPI_V2_GPIO_P1_37 //G26
#define D7 RPI_V2_GPIO_P1_13 //G27

void LCD_Writ_Bus(int VH,int VL)
{
  printf("Start LCD write!!!\n");

  unsigned char i,temp,data;
  int d[8]={D0,D1,D2,D3,D4,D5,D6,D7};
  printf("VH: %d\n",VH);
  printf("VL: %d\n",VL);

  data=VH;
  for(i=0;i<8;i++)
  {
    temp=(data&0x01);
    if(temp) {
     // bcm2835_gpio_write(d[i],HIGH);
	bcm2835_gpio_write(d[i],HIGH);
        printf("1 in %d\n",d[i]);

    } else {
      //bcm2835_gpio_write(i,LOW);
	bcm2835_gpio_write(d[i],LOW);
	printf("0 in %d\n",d[i]);

	}
	    // printf("%d\n",d[i]);
     data=data>>1;
	}

     bcm2835_gpio_write(LCD_WR,LOW);
	//bcm2835_gpio_write(LCD_WR,LOW);
     bcm2835_gpio_write(LCD_WR,HIGH);
	//  bcm2835_gpio_write(LCD_WR,HIGH);

     data=VL;
  
  for(i=0;i<8;i++)
  {
    temp=(data&0x01);
    if(temp){
	bcm2835_gpio_write(d[i],HIGH);
	             printf("1 in %d\n",d[i]);

     // bcm2835_gpio_write(d[i],HIGH);
   }  else {
	             printf("0 in %d\n",d[i]);

	bcm2835_gpio_write(d[i],LOW);
	// bcm2835_gpio_write(i,LOW);
   }
//	printf("%d\n",d[i]);
	data=data>>1;
  }

bcm2835_gpio_write(LCD_WR,LOW);
// bcm2835_gpio_write(LCD_WR,LOW);
bcm2835_gpio_write(LCD_WR,HIGH);
//  bcm2835_gpio_write(LCD_WR,HIGH);
}

void LCD_Write_COM(char VH,char VL)  //send com
{
//  bcm2835_gpio_write(LCD_RS,LOW);
bcm2835_gpio_write(LCD_RS,LOW);

  LCD_Writ_Bus(VH,VL);
}


void LCD_Write_DATA(char VH,char VL)    //send data
{
  bcm2835_gpio_write(LCD_RS,HIGH);
  LCD_Writ_Bus(VH,VL);
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
  LCD_Write_COM(0x00,0x20);
  LCD_Write_DATA(x1>>8,x1);
  LCD_Write_COM(0x00,0x21);
  LCD_Write_DATA(y1>>8,y1);
  LCD_Write_COM(0x00,0x50);
  LCD_Write_DATA(x1>>8,x1);
  LCD_Write_COM(0x00,0x52);
  LCD_Write_DATA(y1>>8,y1);
  LCD_Write_COM(0x00,0x51);
  LCD_Write_DATA(x2>>8,x2);
  LCD_Write_COM(0x00,0x53);
  LCD_Write_DATA(y2>>8,y2);
  LCD_Write_COM(0x00,0x22);
}

void LCD_Init(void)
{
  bcm2835_gpio_fsel(D0, BCM2835_GPIO_FSEL_OUTP);
bcm2835_gpio_fsel(D1, BCM2835_GPIO_FSEL_OUTP);
 bcm2835_gpio_fsel(D2, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(D3, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(D4, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(D5, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(D6, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(D7, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(LCD_REST, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(LCD_CS, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(LCD_RS, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(LCD_WR, BCM2835_GPIO_FSEL_OUTP);


  bcm2835_gpio_write(LCD_REST,HIGH);
  bcm2835_delay(5);   
  //delay(5);
  bcm2835_gpio_write(LCD_REST,LOW);
  bcm2835_delay(5);   
//delay(5);
  bcm2835_gpio_write(LCD_REST,HIGH);
  bcm2835_delay(5);   
  
//delay(5);

  bcm2835_gpio_write(LCD_CS,LOW);
  //************* Start Initial Sequence **********//
  LCD_Write_COM(0x00,0xE5);
  LCD_Write_DATA(0x78,0xF0); // set SRAM internal timing
  LCD_Write_COM(0x00,0x01);
  LCD_Write_DATA(0x01,0x00); // set SS and SM bit
  LCD_Write_COM(0x00,0x02);
  LCD_Write_DATA(0x07,0x00); // set 1 line inversion
  LCD_Write_COM(0x00,0x03);
  LCD_Write_DATA(0x10,0x30); // set GRAM write direction and BGR=1.
  LCD_Write_COM(0x00,0x04);
  LCD_Write_DATA(0x00,0x00); // Resize register
  LCD_Write_COM(0x00,0x08);
  LCD_Write_DATA(0x02,0x07); // set the back porch and front porch
  LCD_Write_COM(0x00,0x09);
  LCD_Write_DATA(0x00,0x00); // set non-display area refresh cycle ISC[3:0]
  LCD_Write_COM(0x00,0x0A);
  LCD_Write_DATA(0x00,0x00); // FMARK function
  LCD_Write_COM(0x00,0x0C);
  LCD_Write_DATA(0x00,0x00); // RGB interface setting
  LCD_Write_COM(0x00,0x0D);
  LCD_Write_DATA(0x00,0x00); // Frame marker Position
  LCD_Write_COM(0x00,0x0F);
  LCD_Write_DATA(0x00,0x00); // RGB interface polarity
  //*************Power On sequence ****************//
  LCD_Write_COM(0x00,0x10);
  LCD_Write_DATA(0x00,0x00); // SAP, BT[3:0], AP, DSTB, SLP, STB
  LCD_Write_COM(0x00,0x11);
  LCD_Write_DATA(0x00,0x07); // DC1[2:0], DC0[2:0], VC[2:0]
  LCD_Write_COM(0x00,0x12);
  LCD_Write_DATA(0x00,0x00); // VREG1OUT voltage
  LCD_Write_COM(0x00,0x13);
  LCD_Write_DATA(0x00,0x00); // VDV[4:0] for VCOM amplitude
  LCD_Write_COM(0x00,0x07);
  LCD_Write_DATA(0x00,0x01);
  bcm2835_delay(50); // Dis-charge capacitor power voltage
  LCD_Write_COM(0x00,0x10);
  LCD_Write_DATA(0x10,0x90); // 1490//SAP, BT[3:0], AP, DSTB, SLP, STB
  LCD_Write_COM(0x00,0x11);
  LCD_Write_DATA(0x02,0x27); // DC1[2:0], DC0[2:0], VC[2:0]
  bcm2835_delay(50); // Delay 50ms
  LCD_Write_COM(0x00,0x12);
  LCD_Write_DATA(0x00,0x1F); //001C// Internal reference voltage= Vci;
  bcm2835_delay(50); // Delay 50ms
  LCD_Write_COM(0x00,0x13);
  LCD_Write_DATA(0x15,0x00); //0x1000//1400   Set VDV[4:0] for VCOM amplitude  1A00
  LCD_Write_COM(0x00,0x29);
  LCD_Write_DATA(0x00,0x27); //0x0012 //001a  Set VCM[5:0] for VCOMH  //0x0025  0034
  LCD_Write_COM(0x00,0x2B);
  LCD_Write_DATA(0x00,0x0D); // Set Frame Rate   000C
  bcm2835_delay(50); // Delay 50ms
  LCD_Write_COM(0x00,0x20);
  LCD_Write_DATA(0x00,0x00); // GRAM horizontal Address
  LCD_Write_COM(0x00,0x21);
  LCD_Write_DATA(0x00,0x00); // GRAM Vertical Address
  // ----------- Adjust the Gamma Curve ----------//
  LCD_Write_COM(0x00,0x30);
  LCD_Write_DATA(0x00,0x00);
  LCD_Write_COM(0x00,0x31);
  LCD_Write_DATA(0x07,0x07);
  LCD_Write_COM(0x00,0x32);
  LCD_Write_DATA(0x03,0x07);
  LCD_Write_COM(0x00,0x35);
  LCD_Write_DATA(0x02,0x00);
  LCD_Write_COM(0x00,0x36);
  LCD_Write_DATA(0x00,0x08);//0207
  LCD_Write_COM(0x00,0x37);
  LCD_Write_DATA(0x00,0x04);//0306
  LCD_Write_COM(0x00,0x38);
  LCD_Write_DATA(0x00,0x00);//0102
  LCD_Write_COM(0x00,0x39);
  LCD_Write_DATA(0x07,0x07);//0707
  LCD_Write_COM(0x00,0x3C);
  LCD_Write_DATA(0x00,0x02);//0702
  LCD_Write_COM(0x00,0x3D);
  LCD_Write_DATA(0x1D,0x04);//1604
 
    //------------------ Set GRAM area ---------------//
  LCD_Write_COM(0x00,0x50);
  LCD_Write_DATA(0x00,0x00); // Horizontal GRAM Start Address
  LCD_Write_COM(0x00,0x51);
  LCD_Write_DATA(0x00,0xEF); // Horizontal GRAM End Address
  LCD_Write_COM(0x00,0x52);
  LCD_Write_DATA(0x00,0x00); // Vertical GRAM Start Address
  LCD_Write_COM(0x00,0x53);
  LCD_Write_DATA(0x01,0x3F); // Vertical GRAM Start Address
  LCD_Write_COM(0x00,0x60);
  LCD_Write_DATA(0xA7,0x00); // Gate Scan Line
  LCD_Write_COM(0x00,0x61);
  LCD_Write_DATA(0x00,0x01); // NDL,VLE, REV
  LCD_Write_COM(0x00,0x6A);
  LCD_Write_DATA(0x00,0x00); // set scrolling line
  //-------------- Partial Display Control ---------//
  LCD_Write_COM(0x00,0x80);
  LCD_Write_DATA(0x00,0x00);
  LCD_Write_COM(0x00,0x81);
  LCD_Write_DATA(0x00,0x00);
  LCD_Write_COM(0x00,0x82);
  LCD_Write_DATA(0x00,0x00);
  LCD_Write_COM(0x00,0x83);
  LCD_Write_DATA(0x00,0x00);
  LCD_Write_COM(0x00,0x84);
  LCD_Write_DATA(0x00,0x00);
  LCD_Write_COM(0x00,0x85);
  LCD_Write_DATA(0x00,0x00);
  //-------------- Panel Control -------------------//
  LCD_Write_COM(0x00,0x90);
  LCD_Write_DATA(0x00,0x10);
  LCD_Write_COM(0x00,0x92);
  LCD_Write_DATA(0x06,0x00);
  LCD_Write_COM(0x00,0x07);
  LCD_Write_DATA(0x01,0x33); // 262K color and display ON
  bcm2835_gpio_write(LCD_CS,HIGH);
}

void Pant(int VH,int VL)
{
  int i,j;
  bcm2835_gpio_write(LCD_CS,LOW);
  Address_set(0,0,240,320);
  for(i=0;i<320;i++)
  {
    for (j=0;j<240;j++)
    {
      LCD_Write_DATA(VH,VL);
    }

  }
  bcm2835_gpio_write(LCD_CS,HIGH);
}

//void setup()
//{
//  for(int p=0;p<20;p++)
//  {
//    pinMode(p,OUTPUT);
//bcm2835_gpio_fsel(p, BCM2835_GPIO_FSEL_OUTP);

 // }
 // LCD_Init();
//}

//void loop()
//{
  // Pant(0x0, 0x0);
  // delay(500);
  // Pant(0xff, 0x00);
  // delay(500);
  // Pant(0x00, 0xff);
  /// delay(500);
  // Pant(0xff, 0xff);
  // delay(500);
//}

int main(){

if (!bcm2835_init()) {        // Инициализация GPIO
                       //Завершение программы, если инициализация не удалась
	printf("bcm2835 init fail");
	return 1;
}
LCD_Init();
printf("LCD init OK");

while(1) {
   Pant(0x0, 0x0);
   bcm2835_delay(500);
   Pant(0xff, 0x00);
//   Pant(0xa5, 0x5a);
   bcm2835_delay(500);

//   Pant(0xb7, 0x34);
//   bcm2835_delay(500);

   bcm2835_delay(500);
   Pant(0x00, 0xff);
   bcm2835_delay(500);
   Pant(0xff, 0xff);
   bcm2835_delay(500);
}

return 0;

}
