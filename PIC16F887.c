/*
 * PIC16F887 - Maneja SONIDO
 * Recibe señales del ATmega para reproducir sonidos
 */

#include <built_in.h>

// Pines de comunicación con ATmega
#define SIGNAL_TO_ATMEGA   RB0_bit   // Salida: envía botón presionado
#define SIGNAL_FROM_ATMEGA RC0_bit   // Entrada: recibe evento de juego

// Pines de sonido
#define BUZZER_PIN         RC2_bit

// Sonidos
void playEatSound() {
    Sound_Play(1200, 60);
    Delay_ms(20);
    Sound_Play(1500, 50);
}

void playLoseSound() {
    unsigned char i;
    for(i = 10; i > 0; i--) {
        Sound_Play(400 + (i * 30), 50);
        Delay_ms(20);
    }
    Sound_Play(100, 300);
}

void playWinSound() {
    Sound_Play(523, 200);
    Delay_ms(50);
    Sound_Play(587, 200);
    Delay_ms(50);
    Sound_Play(659, 200);
    Delay_ms(50);
    Sound_Play(523, 400);
}

// Medir pulso del ATmega
unsigned char medirPulso() {
    unsigned int duracion = 0;
    while(SIGNAL_FROM_ATMEGA == 0 && duracion < 2000) {
        Delay_ms(1);
        duracion++;
    }
    if(duracion >= 2000) return 0;

    duracion = 0;
    while(SIGNAL_FROM_ATMEGA == 1 && duracion < 2000) {
        Delay_ms(1);
        duracion++;
    }

    if(duracion >= 50 && duracion <= 150) return 1;   // comió
    if(duracion >= 450 && duracion <= 550) return 2;  // perdió
    if(duracion >= 950 && duracion <= 1050) return 3; // ganó
    return 0;
}

void main() {
    // Configurar pines
    TRISB = 0xFF;  // Puerto B como entrada (botones)
    TRISC = 0x00;  // Puerto C como salida (buzzer)

    // Configurar pines específicos
    TRISB0_bit = 0;  // RB0 como salida (señal a ATmega)
    TRISC0_bit = 1;  // RC0 como entrada (señal de ATmega)
    TRISC2_bit = 0;  // RC2 como salida (buzzer)

    // Inicializar
    SIGNAL_TO_ATMEGA = 0;
    BUZZER_PIN = 0;
    Sound_Init(&PORTC, 2);

    // Bucle principal
    while(1) {
    
        // Recibir eventos del ATmega y reproducir sonido
        eventoRecibido = medirPulso();
        switch(eventoRecibido) {
            case 1: playEatSound(); break;
            case 2: playLoseSound(); break;
            case 3: playWinSound(); break;
        }
        Delay_ms(10);
    }
}
