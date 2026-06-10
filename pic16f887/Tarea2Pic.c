/*
 * PIC16F887 - MODULO DE SONIDO PARA SNAKE
 *
 * RC0 <- Señal desde ATmega328P
 * RC2 -> Speaker
 */

#include <built_in.h>

#define SIGNAL_IN RC0_bit

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

    while(SIGNAL_IN == 1) {

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

    TRISC0_bit = 1;   // Entrada desde ATmega
    TRISC2_bit = 0;   // Speaker

    PORTC = 0;

    Sound_Init(&PORTC, 2);

    Delay_ms(300);

    while(1) {

        // Detectar flanco de subida
        if(SIGNAL_IN == 1) {

            unsigned int duracion;

            duracion = medirPulso();

            // COMER
            if(duracion >= 50 && duracion < 250) {

                playEatSound();
            }

            // PERDER
            else if(duracion >= 250 && duracion < 750) {

                playLoseSound();
            }

            // GANAR
            else if(duracion >= 750) {

                playWinSound();
            }

            // Esperar que la línea vuelva a LOW
            while(SIGNAL_IN == 1);

            // Tiempo muerto para evitar dobles detecciones
            Delay_ms(100);
        }
    }
}