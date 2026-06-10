#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/pgmspace.h>

// Botones
#define BTN_UP_PIN    PC0
#define BTN_RIGHT_PIN PC1
#define BTN_DOWN_PIN  PC2
#define BTN_LEFT_PIN  PC3
#define SOUND_PIN     PC4

// Dificultades
#define FACIL   1
#define MEDIO   2
#define DIFICIL 3

// Variables globales
static uint8_t  gameboard[8][8];
static uint8_t  snakeHeadRow, snakeHeadCol;
static uint8_t  foodRow, foodCol;
static uint8_t  snakeLength;
static uint16_t snakeSpeed;
static uint8_t  direction;
static uint8_t  pendingDirection;
static uint8_t  previousDirection;
static uint8_t  gameRunning;
static uint8_t  gameOver;
static uint8_t  win;
static uint8_t  dificultadSeleccionada;
static uint8_t  longitudParaGanar;
static void sonidoComer(void);
static void sonidoPerder(void);
static void sonidoGanar(void);
static uint16_t lfsr = 0xACE1u;

static const uint8_t PORT_FILAS[8] = {
    (1<<PD0),(1<<PD1),(1<<PD2),(1<<PD3),
    (1<<PD4),(1<<PD5),(1<<PD6),(1<<PD7)
};

// VELOCIDADES POR DIFICULTAD
#define VEL_FACIL   200
#define VEL_MEDIO   120
#define VEL_DIFICIL 60

// LETRAS
static const uint8_t LETRA_S[8] PROGMEM = {
    0b00111100,
    0b01000010,
    0b01000000,
    0b00111100,
    0b00000010,
    0b01000010,
    0b00111100,
    0b00000000
};

static const uint8_t LETRA_N[8] PROGMEM = {
    0b01000010,
    0b01100010,
    0b01010010,
    0b01001010,
    0b01000110,
    0b01000010,
    0b01000010,
    0b00000000
};

static const uint8_t LETRA_A[8] PROGMEM = {
    0b00011000,
    0b00100100,
    0b01000010,
    0b01111110,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00000000
};

static const uint8_t LETRA_K[8] PROGMEM = {
    0b01000010,
    0b01000100,
    0b01001000,
    0b01110000,
    0b01001000,
    0b01000100,
    0b01000010,
    0b00000000
};

static const uint8_t LETRA_E[8] PROGMEM = {
    0b01111110,
    0b01000000,
    0b01000000,
    0b01111100,
    0b01000000,
    0b01000000,
    0b01111110,
    0b00000000
};

static const uint8_t LETRA_F[8] PROGMEM = {
    0b01111110,
    0b01000000,
    0b01000000,
    0b01111100,
    0b01000000,
    0b01000000,
    0b01000000,
    0b00000000
};

static const uint8_t LETRA_C[8] PROGMEM = {
    0b00111100,
    0b01000010,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000010,
    0b00111100,
    0b00000000
};

static const uint8_t LETRA_D[8] PROGMEM = {
    0b01111000,
    0b01000100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000100,
    0b01111000,
    0b00000000
};

static const uint8_t LETRA_I[8] PROGMEM = {
    0b00111100,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00111100,
    0b00000000
};

static const uint8_t LETRA_M[8] PROGMEM = {
    0b01000010,
    0b01100110,
    0b01011010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00000000
};

static const uint8_t SIGNO_PREGUNTA[8] PROGMEM = {
    0b00111100,
    0b01000010,
    0b00000010,
    0b00001100,
    0b00010000,
    0b00000000,
    0b00010000,
    0b00000000
};

static const uint8_t SIGNO_EXCLAMACION[8] PROGMEM = {
    0b00010000,
    0b00111000,
    0b00111000,
    0b00010000,
    0b00010000,
    0b00000000,
    0b00010000,
    0b00000000
};

// CARITAS
static const uint8_t CARA_TRISTE[8] PROGMEM = {
    0b00111100,
    0b01000010,
    0b10100101,
    0b10000001,
    0b10011001,
    0b10100101,
    0b01000010,
    0b00111100
};

static const uint8_t CARA_FELIZ[8] PROGMEM = {
    0b00111100,
    0b01000010,
    0b10100101,
    0b10000001,
    0b10100101,
    0b10011001,
    0b01000010,
    0b00111100
};

// ============================================================
// INTERRUPCIÓN DE BOTONES (PARA EL JUEGO)
// ============================================================

ISR(PCINT0_vect) {
    uint8_t boton = 0;
    
    if (!(PINC & (1 << PC0))) boton = 1;
    else if (!(PINC & (1 << PC1))) boton = 2;
    else if (!(PINC & (1 << PC2))) boton = 3;
    else if (!(PINC & (1 << PC3))) boton = 4;
    
    if (boton != 0 && gameRunning) {
        if ((boton == 1 && previousDirection != 3) ||
            (boton == 2 && previousDirection != 4) ||
            (boton == 3 && previousDirection != 1) ||
            (boton == 4 && previousDirection != 2)) {
            pendingDirection = boton;
        }
    }
}

// FUNCIONES BÁSICAS
static void delay_ms(uint16_t ms) {
    while (ms--) _delay_ms(1);
}

static uint8_t rand8(void) {
    lfsr ^= lfsr >> 7;
    lfsr ^= lfsr << 9;
    lfsr ^= lfsr >> 13;
    return (uint8_t)(lfsr & 0xFF);
}

static uint8_t rand_range(uint8_t max) {
    return rand8() % max;
}

// MATRIZ LED
static void limpiarMatriz(void) {
    for (uint8_t row = 0; row < 8; row++) {
        PORTD = PORT_FILAS[row];
        PORTB = 0xFF;
        _delay_us(50);
    }
}

static void mostrarImagen(const uint8_t *img) {
    for (uint8_t k = 0; k < 30; k++) {
        for (uint8_t row = 0; row < 8; row++) {
            PORTD = PORT_FILAS[row];
            PORTB = ~pgm_read_byte(&img[row]);
            _delay_us(300);
        }
    }
}

static void mostrarPalabra(const uint8_t *letra1, const uint8_t *letra2, const uint8_t *letra3) {
    mostrarImagen(letra1);
    mostrarImagen(letra2);
    mostrarImagen(letra3);
}

// ANIMACIÓN "SNAKE" DESPLAZÁNDOSE
static void mostrarSnakeDesplazando(void) {
    const uint8_t *letras[] = {LETRA_S, LETRA_N, LETRA_A, LETRA_K, LETRA_E};
    int16_t total = 5 * 8 + 8;
    
    for (int16_t desplazamiento = 0; desplazamiento < total; desplazamiento++) {
        for (uint8_t frame = 0; frame < 15; frame++) {
            for (uint8_t fila = 0; fila < 8; fila++) {
                uint8_t filaData = 0;
                
                for (uint8_t col = 0; col < 8; col++) {
                    int16_t colOrigen = desplazamiento + col;
                    if (colOrigen >= 0 && colOrigen < 5 * 8) {
                        uint8_t letraIndex = colOrigen / 8;
                        uint8_t bitIndex = 7 - (colOrigen % 8);
                        uint8_t byteLetra = pgm_read_byte(&letras[letraIndex][fila]);
                        if (byteLetra & (1 << bitIndex)) {
                            filaData |= (1 << (7 - col));
                        }
                    }
                }
                
                PORTD = PORT_FILAS[fila];
                PORTB = ~filaData;
                _delay_us(80);
            }
        }
    }
}

// REFRESCAR PANTALLA DEL JUEGO
static void refrescarPantalla(void) {
    for (uint16_t i = 0; i < snakeSpeed; i++) {
        for (uint8_t row = 0; row < 8; row++) {
            uint8_t rowData = 0;
            for (uint8_t col = 0; col < 8; col++) {
                if (gameboard[row][col] > 0) {
                    rowData |= (1 << (7 - col));
                }
            }
            if (foodRow < 8 && foodCol < 8 && row == foodRow) {
                rowData |= (1 << (7 - foodCol));
            }
            PORTD = PORT_FILAS[row];
            PORTB = ~rowData;
            _delay_us(60);
        }
        _delay_us(60);
    }
}

// SONIDO HACIA PIC16F887
static void sonidoComer(void) {

    PORTC |= (1 << SOUND_PIN);
    delay_ms(10);
    PORTC &= ~(1 << SOUND_PIN);
}

static void sonidoPerder(void) {

    PORTC |= (1 << SOUND_PIN);
    delay_ms(90);
    PORTC &= ~(1 << SOUND_PIN);
}

static void sonidoGanar(void) {

    PORTC |= (1 << SOUND_PIN);
    delay_ms(200);
    PORTC &= ~(1 << SOUND_PIN);
}

// INICIALIZACIÓN DEL JUEGO
static void inicializarJuego(void) {
    switch (dificultadSeleccionada) {
        case FACIL:
            snakeSpeed = VEL_FACIL;
            longitudParaGanar = 5;   // 2 frutas (3 + 2 = 5)
            break;
        case MEDIO:
            snakeSpeed = VEL_MEDIO;
            longitudParaGanar = 6;   // 3 frutas (3 + 3 = 6)
            break;
        case DIFICIL:
            snakeSpeed = VEL_DIFICIL;
            longitudParaGanar = 8;   // 5 frutas (3 + 5 = 8)
            break;
        default:
            snakeSpeed = VEL_MEDIO;
            longitudParaGanar = 6;
            break;
    }
    
    gameRunning = 1;
    gameOver = 0;
    win = 0;

    for (uint8_t r = 0; r < 8; r++)
        for (uint8_t c = 0; c < 8; c++)
            gameboard[r][c] = 0;

    snakeHeadRow = 3;
    snakeHeadCol = 3;
    snakeLength = 3;
    direction = 2;
    pendingDirection = 2;
    previousDirection = 2;
}

static void dibujarInicio(void) {
    gameboard[3][3] = 3;
    gameboard[3][2] = 2;
    gameboard[3][1] = 1;
    foodRow = 5;
    foodCol = 5;
}

static void generarComida(void) {
    if (snakeLength >= longitudParaGanar) {
        sonidoGanar();
        win = 1;
        gameRunning = 0;
        return;
    }
    
    do {
        foodRow = rand_range(8);
        foodCol = rand_range(8);
    } while (gameboard[foodRow][foodCol] > 0);
}

// ACTUALIZAR SERPIENTE
static void actualizarSerpiente(void) {
    uint8_t newRow = snakeHeadRow;
    uint8_t newCol = snakeHeadCol;

    direction = pendingDirection;

    switch (direction) {
        case 1:
            if (newRow == 0) {
                gameOver = 1;
                gameRunning = 0;
                return;
            }
            newRow--;
            break;
        case 2:
            if (newCol == 7) {
                gameOver = 1;
                gameRunning = 0;
                return;
            }
            newCol++;
            break;
        case 3:
            if (newRow == 7) {
                gameOver = 1;
                gameRunning = 0;
                return;
            }
            newRow++;
            break;
        case 4:
            if (newCol == 0) {
                gameOver = 1;
                gameRunning = 0;
                return;
            }
            newCol--;
            break;
    }

    uint8_t ateFood = (newRow == foodRow && newCol == foodCol);

    if (gameboard[newRow][newCol] > 0 && !ateFood) {
        gameOver = 1;
        gameRunning = 0;
        return;
    }

    for (uint8_t r = 0; r < 8; r++)
        for (uint8_t c = 0; c < 8; c++)
            if (gameboard[r][c] > 0)
                gameboard[r][c]--;

    snakeHeadRow = newRow;
    snakeHeadCol = newCol;

    if (ateFood) {
        sonidoComer();
        snakeLength++;
        
        for (uint8_t r = 0; r < 8; r++)
            for (uint8_t c = 0; c < 8; c++)
                if (gameboard[r][c] > 0)
                    gameboard[r][c]++;
        
        gameboard[snakeHeadRow][snakeHeadCol] = snakeLength;
        generarComida();
    } else {
        gameboard[snakeHeadRow][snakeHeadCol] = snakeLength;
    }

    previousDirection = direction;
}

// MOSTRAR FINAL
static void mostrarGameOver(void) {
    limpiarMatriz();
    sonidoPerder();
    mostrarImagen(CARA_TRISTE);
    delay_ms(500);
    limpiarMatriz();
}

static void mostrarWin(void) {
    limpiarMatriz();
    mostrarImagen(CARA_FELIZ);
    delay_ms(500);
    limpiarMatriz();
}

// MENÚ DE DIFICULTAD
static void menuDificultad(void) {
    uint8_t seleccion = 0;
    uint8_t botonAnterior = 0;
    dificultadSeleccionada = FACIL;
    
    // Mostrar "?" velocidad normal
    limpiarMatriz();
    mostrarImagen(SIGNO_PREGUNTA);
    delay_ms(100);
    
    while (1) {
        uint8_t boton = 0;
        if (!(PINC & (1 << PC0))) boton = 1;
        else if (!(PINC & (1 << PC1))) boton = 2;
        else if (!(PINC & (1 << PC2))) boton = 3;
        else if (!(PINC & (1 << PC3))) boton = 4;
        
        // Detección por FLANCO
        if (boton != 0 && boton != botonAnterior) {
            if (boton == 1) {  // FACIL
                dificultadSeleccionada = FACIL;
                limpiarMatriz();
                mostrarPalabra(LETRA_F, LETRA_A, LETRA_C);
                delay_ms(100);
                seleccion = 1;
                
                limpiarMatriz();
                mostrarImagen(SIGNO_EXCLAMACION);
                delay_ms(100);
            }
            else if (boton == 2) {  // MEDIO
                dificultadSeleccionada = MEDIO;
                limpiarMatriz();
                mostrarPalabra(LETRA_M, LETRA_E, LETRA_D);
                delay_ms(100);
                seleccion = 1;
                
                limpiarMatriz();
                mostrarImagen(SIGNO_EXCLAMACION);
                delay_ms(100);
            }
            else if (boton == 3) {  // DIFICIL
                dificultadSeleccionada = DIFICIL;
                limpiarMatriz();
                mostrarPalabra(LETRA_D, LETRA_I, LETRA_F);
                delay_ms(100);
                seleccion = 1;
                
                limpiarMatriz();
                mostrarImagen(SIGNO_EXCLAMACION);
                delay_ms(100);
            }
            else if (boton == 4 && seleccion == 1) {  // CONFIRMAR
                limpiarMatriz();
                delay_ms(100);
                return;
            }
        }
        
        botonAnterior = boton;
        delay_ms(30);
    }
}

// FUNCIÓN PRINCIPAL
int main(void) {
    DDRD = 0xFF;
    PORTD = 0x00;
    DDRB = 0xFF;
    PORTB = 0xFF;
    
    DDRC = 0x00;

// PC4 como salida hacia PIC
DDRC |= (1 << SOUND_PIN);

PORTC = (1 << PC0) |
        (1 << PC1) |
        (1 << PC2) |
        (1 << PC3);

PORTC &= ~(1 << SOUND_PIN);
    
    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3);
    
    ADMUX = (1 << REFS0) | 7;
    ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    for (uint8_t i = 0; i < 50; i++) {
        _delay_us(10);
    }
    while (ADCSRA & (1 << ADSC));
    lfsr = ADC ^ 0xA5B6;
    ADCSRA &= ~(1 << ADEN);
    
    for (uint8_t i = 0; i < 30; i++) {
        rand8();
    }
    
    sei();
    
    // Mostrar "SNAKE" al encender
    limpiarMatriz();
    mostrarSnakeDesplazando();
    limpiarMatriz();
    delay_ms(200);
    
    // Bucle principal del juego
    while (1) {
        // Menú de dificultad
        menuDificultad();
        
        // Iniciar juego
        inicializarJuego();
        dibujarInicio();
        
        while (gameRunning) {
            refrescarPantalla();
            actualizarSerpiente();
            
            if (snakeLength >= longitudParaGanar) {
                sonidoGanar();
                gameRunning = 0;
                mostrarWin();
                break;
            }
            
            if (gameOver) {
                gameRunning = 0;
                mostrarGameOver();
                break;
            }
        }
        
        delay_ms(800);
    }
    
    return 0;
}
