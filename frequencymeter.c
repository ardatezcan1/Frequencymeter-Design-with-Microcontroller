#include<reg52.h>

sfr lveri = 0x90;						//LCD Pinleri
sbit rs	= P3^2;
sbit rw	= P3^1;
sbit en	= P3^0;
sbit busy = P1^7;

unsigned char Unit[4];

void MSDelay(unsigned int);
void lcdkomut(unsigned char);
void lcdveri(unsigned char);
void lcdready(void);
void lcdinit(void);
void inttoLCD(unsigned int);

void main(void)
	{
	 unsigned int HI, LO, TOPLAM; //Frekans degerlerini tutmak icin
	 unsigned char l, OV, MSJ[16] = "Sinir disinda...";
	 OV = 0;
	 T0 = 1;
	 TMOD = 0x05;
	 TL0 = 0;
	 TL1 = 0;
	 lcdinit();
	 while(1)// Burada girilen frekansin buyuklugune gore siniflandirma yapilmistir
	 	{
		 do
		 	{
start: TR0 = 1;
			 MSDelay(145);
		 	 LO = TL0;
		 	 HI = TH0;
			 HI = HI << 8;
			 TOPLAM = HI | LO;
			 Unit[0] = 'H';
			 Unit[1] = 'z';
			 Unit[2] = ' ';
			 Unit[3] = ' ';
			 if(OV == 1)
			 	{
				 TOPLAM = TOPLAM / 1000;
				 TOPLAM = TOPLAM + 66;
				 if(TOPLAM > 100)
				 	{
					 for(l=0; l<16; l++)
					 	lcdveri(MSJ[l]);
					 for(l=0; l<16; l++)
					 	lcdkomut(0x10);
					 goto start; 
					 }
				 OV = 0;
				 Unit[0] = 'K';
			 	 Unit[1] = 'H';
			 	 Unit[2] = 'z';
				 Unit[3] = ' ';
				}
		 	 inttoLCD(TOPLAM);

		 	 TL0 = 0;
		 	 TH0 = 0;
			}
		 while(TF0 == 0);
		 OV = 1;
		 TR0 = 0;
		 TF0 = 0;
		}
	}

void lcdinit(void)//Raporda da(Cizelge 1.1) verilen girislere gore olusacak fonksiyonlar
	{
	 lcdkomut(0x38);//2 satirlik 5x7'lik matris
	 lcdkomut(0x0C);//Goruntuleme acik
	 lcdkomut(0x01);//Goruntu ekranini temizleme
	 lcdkomut(0x80);//Ilk satirdan baslamsi icin imleci ayarlama
	 lcdkomut(0x06);//Imlec konumunu bir arttirma
	}
	
void lcdkomut(unsigned char deger)//lcdinit fonksiyonundaki islemlerin yapilabilmesi için rs ve rw degerleri ayarlandi
	{
	 lcdready();
	 lveri = deger;
	 rs = 0;
	 rw = 0;
	 en = 1;
 	 MSDelay(1);
	 en = 0;
	}

void lcdveri(unsigned char deger)
	{
	 lcdready();
	 lveri = deger;//Degerler pinlere yuklenir
	 rs = 1;
	 rw = 0;
	 en = 1;								
	 MSDelay(1);
	 en = 0;
	}

void lcdready()//Bayraklari okuma ve adresleri sayma
	{
	 busy = 1;
	 rs = 0;
 	 rw = 1;
 	 do
		{
		 en = 0;
		 MSDelay(1);
		 en = 1;
		}
	 while(busy == 1);
	 return;
	}

void inttoLCD(unsigned int deger)//Gelen degerlerin formatini ASCII tablosundaki degerlerini bulup LCD'e yazdirilmasi. Her seferinde +48 eklenemsi ASCII tablosunda sayilarin 48'den sonra baslamasidir
	{
	 unsigned int x, y, z, d[5];
	 char l;
	 x = deger / 10; 										
	 d[0] = (deger % 10) + 48; 					
	 d[1] = (x % 10) + 48;	 						
	 y = x / 10; 												
	 d[2] = (y % 10) + 48; 							
	 z = y / 10; 												
	 d[3] = (z % 10) + 48;							
	 d[4] = (z / 10) + 48;							
	 if(d[4] == 48 & d[3] == 48 & d[2] == 48 & d[1] == 48)
		{
		  lcdveri(d[0]);
		  lcdveri(' ');
		  for(l=0; l<4; l++)
		 	 lcdveri(Unit[l]);
		  for(l=0 ; l<11 ; l++)
		  	 lcdveri(' ');
		  for(l=0 ; l<17 ; l++)
		 	 lcdkomut(0x10);
		  
		}
	 if(d[4] == 48 & d[3] == 48 & d[2] == 48 & d[1] != 48)
		{
		 for(l=1; l>=0; l--)
	 	 	lcdveri(d[l]);
		 lcdveri(' ');
		 for(l=0; l<4; l++)
		 	 lcdveri(Unit[l]);
		 for(l=0 ; l<10 ; l++)
		  	 lcdveri(' ');
		 for(l=0 ; l<17 ; l++)
		 	 lcdkomut(0x10);
		 
		}
	 if(d[4] == 48 & d[3] == 48 & d[2] != 48)
		{
		 for(l=2; l>=0; l--)
	 	 	lcdveri(d[l]);
		 lcdveri(' ');
		 for(l=0; l<4; l++)
		 	 lcdveri(Unit[l]);
		 for(l=0 ; l<9 ; l++)
		  	 lcdveri(' ');
		 for(l=0 ; l<17 ; l++)
		 	 lcdkomut(0x10);
		 
		}
	 if(d[4] == 48 & d[3] != 48)
	 	{
		 for(l=3; l>=0; l--)
	 	 	lcdveri(d[l]);
		 lcdveri(' ');
		 for(l=0; l<4; l++)
		 	lcdveri(Unit[l]);
		 for(l=0 ; l<8 ; l++)
		  	 lcdveri(' ');
		 for(l=0 ; l<17 ; l++)
		 	 lcdkomut(0x10);
		 
		}
	 if(d[4] != 48)
	 	{
		 for(l=4; l>=0; l--)
		 	lcdveri(d[l]);
		 lcdveri(' ');
		 for(l=0; l<4; l++)
		 	lcdveri(Unit[l]);
		 		 for(l=0 ; l<7 ; l++)
		  	 lcdveri(' ');
		 for(l=0 ; l<17 ; l++)
		 	 lcdkomut(0x10);

		}
	 }

	void MSDelay(unsigned int deger)	//Gecikmeyi ayarlamak icin
	{
	 unsigned int x,y;
	 for(x = 0; x < deger; x++)
		for(y = 0; y < 1275; y++);		
	}