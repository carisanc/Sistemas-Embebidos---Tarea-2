#line 1 "C:/Users/Public/Documents/Mikroelektronika/mikroC PRO for PIC/Examples/Tarea2Pic/Tarea2Pic.c"
#line 1 "c:/users/public/documents/mikroelektronika/mikroc pro for pic/include/built_in.h"
#line 12 "C:/Users/Public/Documents/Mikroelektronika/mikroC PRO for PIC/Examples/Tarea2Pic/Tarea2Pic.c"
void playEatSound(void);
void playLoseSound(void);
void playWinSound(void);
unsigned int medirPulso(void);

void playEatSound(void) {

 Sound_Play(2500, 120);
}

void playLoseSound(void) {

 Sound_Play(900, 120);
 Delay_ms(30);

 Sound_Play(700, 120);
 Delay_ms(30);

 Sound_Play(500, 120);
 Delay_ms(30);

 Sound_Play(250, 250);
}

void playWinSound(void) {

 Sound_Play(523, 120);
 Delay_ms(30);

 Sound_Play(659, 120);
 Delay_ms(30);

 Sound_Play(784, 120);
 Delay_ms(30);

 Sound_Play(1047, 300);
}

unsigned int medirPulso(void) {

 unsigned int tiempo = 0;

 while( RC0_bit  == 1) {

 Delay_ms(1);
 tiempo++;

 if(tiempo >= 1500)
 break;
 }

 return tiempo;
}

void main() {

 ANSEL = 0;
 ANSELH = 0;

 TRISC0_bit = 1;
 TRISC2_bit = 0;

 PORTC = 0;

 Sound_Init(&PORTC, 2);

 Delay_ms(300);

 while(1) {


 if( RC0_bit  == 1) {

 unsigned int duracion;

 duracion = medirPulso();


 if(duracion >= 50 && duracion < 250) {

 playEatSound();
 }


 else if(duracion >= 250 && duracion < 750) {

 playLoseSound();
 }


 else if(duracion >= 750) {

 playWinSound();
 }


 while( RC0_bit  == 1);


 Delay_ms(100);
 }
 }
}
