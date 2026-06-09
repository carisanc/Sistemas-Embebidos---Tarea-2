#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdint.h>

#define BTN_UP_PIN    PC0
#define BTN_RIGHT_PIN PC1
#define BTN_DOWN_PIN  PC2
#define BTN_LEFT_PIN  PC3
#define SIG_TO_PIC    PC4   // A4 - señal hacia PIC

// Activo bajo 
#define BTN_UP    (!(PINC & (1 << BTN_UP_PIN)))
#define BTN_RIGHT (!(PINC & (1 << BTN_RIGHT_PIN)))
#define BTN_DOWN  (!(PINC & (1 << BTN_DOWN_PIN)))
#define BTN_LEFT  (!(PINC & (1 << BTN_LEFT_PIN)))

#define SCROLL_SPEED 40

#define DIFICIL 1
#define MEDIO   2
#define FACIL   3

typedef struct {
    uint16_t velocidadInicial;
    uint16_t incrementoVelocidad;
    uint8_t  puntuacionExtra;
} DificultadConfig;

static const DificultadConfig dificultades[4] PROGMEM = {
    {0,   0,  0},
    {150, 5,  2},   // DIFICIL
    {250, 8,  1},   // MEDIO
    {350, 12, 1}    // FACIL
};

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
static uint16_t puntos;
static uint8_t  ultimoBoton;

static uint16_t lfsr = 0xACE1u;

static const uint8_t PORT_FILAS[8] = {
    (1<<PD0),(1<<PD1),(1<<PD2),(1<<PD3),
    (1<<PD4),(1<<PD5),(1<<PD6),(1<<PD7)
};

static const uint8_t SIGNO[8]  PROGMEM = {0x00,0x04,0x02,0x01,0xB1,0x0A,0x04,0x00};
static const uint8_t FELIZ[8]  PROGMEM = {0x00,0x3C,0x42,0xA5,0x81,0xA5,0x42,0x3C};
static const uint8_t TRISTE[8] PROGMEM = {0x00,0x3C,0x42,0xA5,0x81,0x99,0x42,0x3C};

static const uint8_t LETRA_A[8] PROGMEM = {0x18,0x24,0x42,0x7E,0x42,0x42,0x42,0x00};
static const uint8_t LETRA_C[8] PROGMEM = {0x3C,0x42,0x40,0x40,0x40,0x42,0x3C,0x00};
static const uint8_t LETRA_D[8] PROGMEM = {0x78,0x44,0x42,0x42,0x42,0x44,0x78,0x00};
static const uint8_t LETRA_E[8] PROGMEM = {0x7E,0x40,0x40,0x7C,0x40,0x40,0x7E,0x00};
static const uint8_t LETRA_F[8] PROGMEM = {0x7E,0x40,0x40,0x7C,0x40,0x40,0x40,0x00};
static const uint8_t LETRA_I[8] PROGMEM = {0x3C,0x18,0x18,0x18,0x18,0x18,0x3C,0x00};
static const uint8_t LETRA_K[8] PROGMEM = {0x42,0x44,0x48,0x70,0x48,0x44,0x42,0x00};
static const uint8_t LETRA_M[8] PROGMEM = {0x42,0x66,0x5A,0x42,0x42,0x42,0x42,0x00};
static const uint8_t LETRA_N[8] PROGMEM = {0x42,0x62,0x52,0x4A,0x46,0x42,0x42,0x00};
static const uint8_t LETRA_S[8] PROGMEM = {0x3C,0x42,0x40,0x3C,0x02,0x42,0x3C,0x00};

static const uint8_t CERO[8]   PROGMEM = {0x3C,0x42,0x46,0x4A,0x52,0x62,0x3C,0x00};
static const uint8_t UNO[8]    PROGMEM = {0x08,0x18,0x08,0x08,0x08,0x08,0x1C,0x00};
static const uint8_t DOS[8]    PROGMEM = {0x3C,0x42,0x02,0x1C,0x20,0x40,0x7E,0x00};
static const uint8_t TRES[8]   PROGMEM = {0x7E,0x02,0x04,0x1C,0x02,0x42,0x3C,0x00};
static const uint8_t CUATRO[8] PROGMEM = {0x04,0x0C,0x14,0x24,0x7E,0x04,0x04,0x00};
static const uint8_t CINCO[8]  PROGMEM = {0x7E,0x40,0x7C,0x02,0x02,0x42,0x3C,0x00};
static const uint8_t SEIS[8]   PROGMEM = {0x1C,0x20,0x40,0x7C,0x42,0x42,0x3C,0x00};
static const uint8_t SIETE[8]  PROGMEM = {0x7E,0x02,0x04,0x08,0x10,0x10,0x10,0x00};
static const uint8_t OCHO[8]   PROGMEM = {0x3C,0x42,0x42,0x3C,0x42,0x42,0x3C,0x00};
static const uint8_t NUEVE[8]  PROGMEM = {0x3C,0x42,0x42,0x3E,0x02,0x04,0x38,0x00};


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

static void mostrarImagen(const uint8_t *img) {
    for (uint8_t k = 0; k < 30; k++) {
        for (uint8_t j = 0; j < 8; j++) {
            PORTD = PORT_FILAS[j];
            PORTB = ~pgm_read_byte(&img[j]);
            _delay_us(500);
        }
    }
}

static void mostrarImagenMs(const uint8_t *img, uint16_t ms) {
    uint32_t ciclos = (uint32_t)ms * 1000UL / (8 * 5);
    for (uint32_t k = 0; k < ciclos; k++) {
        for (uint8_t j = 0; j < 8; j++) {
            PORTD = PORT_FILAS[j];
            PORTB = ~pgm_read_byte(&img[j]);
            _delay_us(500);
        }
    }
}

static void limpiarMatriz(void) {
    PORTD = 0x00;
    PORTB = 0xFF;
    _delay_ms(1);
}

static void mostrarTextoDesplazando(const uint8_t * const letras[], uint8_t numLetras) {
    int16_t total = (int16_t)numLetras * 8 + 8;
    for (int16_t desp = 0; desp < total; desp++) {
        for (uint8_t frame = 0; frame < SCROLL_SPEED; frame++) {
            for (uint8_t col = 0; col < 8; col++) {
                uint8_t colData = 0;
                int16_t colOrig = desp + col;
                if (colOrig >= 0 && colOrig < (int16_t)numLetras * 8) {
                    uint8_t li = colOrig / 8;
                    uint8_t ci = colOrig % 8;
                    colData = pgm_read_byte(&letras[li][ci]);
                }
                PORTD = PORT_FILAS[col];
                PORTB = ~colData;
                _delay_us(100);
            }
        }
    }
}

static void dibujarTablero(void) {
    for (uint8_t row = 0; row < 8; row++) {
        for (uint8_t col = 0; col < 8; col++) {
            PORTD = PORT_FILAS[row];
            PORTB = (gameboard[row][col] > 0) ? ~(1 << (7 - col)) : 0xFF;
            _delay_us(50);
        }
    }
}

/*
 * Envía evento al PIC mediante un pulso HIGH en PC4:
 *   1 (comio)  -> 100 ms
 *   2 (perdio) -> 500 ms
 *   3 (gano)   -> 1000 ms
 */
static void enviarEventoPIC(uint8_t evento) {
    uint16_t duracion = 0;
    switch (evento) {
        case 1: duracion = 100;  break;
        case 2: duracion = 500;  break;
        case 3: duracion = 1000; break;
        default: return;
    }
    DDRC  |=  (1 << SIG_TO_PIC);
    PORTC |=  (1 << SIG_TO_PIC);
    delay_ms(duracion);
    PORTC &= ~(1 << SIG_TO_PIC);
    DDRC  &= ~(1 << SIG_TO_PIC);
}

static uint8_t leerBoton(void) {
    if (BTN_UP)    return 1;
    if (BTN_RIGHT) return 2;
    if (BTN_DOWN)  return 3;
    if (BTN_LEFT)  return 4;
    return 0;
}

static uint8_t esperarCualquierBoton(void) {
    uint8_t b;
    do {
        b = leerBoton();
        _delay_ms(10);
    } while (b == 0);
    delay_ms(50);
    return b;
}

static void mostrarDificultad(void) {
    limpiarMatriz();
    delay_ms(200);
    switch (dificultadSeleccionada) {
        case DIFICIL:
            mostrarImagen(LETRA_D);
            mostrarImagen(LETRA_I);
            mostrarImagen(LETRA_F);
            break;
        case MEDIO:
            mostrarImagen(LETRA_M);
            mostrarImagen(LETRA_E);
            mostrarImagen(LETRA_D);
            break;
        case FACIL:
            mostrarImagen(LETRA_F);
            mostrarImagen(LETRA_A);
            mostrarImagen(LETRA_C);
            break;
    }
    delay_ms(800);
}

static void seleccionarDificultad(void) {
    dificultadSeleccionada = MEDIO;
    mostrarDificultad();

    while (1) {
        uint8_t b = leerBoton();
        if (b == 1) {                    // UP -> Difícil
            dificultadSeleccionada = DIFICIL;
            mostrarDificultad();
            delay_ms(200);
        } else if (b == 3) {             // DOWN -> Fácil
            dificultadSeleccionada = FACIL;
            mostrarDificultad();
            delay_ms(200);
        } else if (b == 2 || b == 4) {   // RIGHT o LEFT -> confirmar
            delay_ms(200);
            return;
        }
        delay_ms(50);
    }
}

static void inicializarJuego(void) {
    snakeSpeed = pgm_read_word(&dificultades[dificultadSeleccionada].velocidadInicial);
    puntos     = 0;

    for (uint8_t r = 0; r < 8; r++)
        for (uint8_t c = 0; c < 8; c++)
            gameboard[r][c] = 0;

    snakeHeadRow     = rand_range(8);
    snakeHeadCol     = rand_range(8);
    snakeLength      = 3;
    direction        = 1;
    pendingDirection = 1;
    previousDirection= 1;
    gameOver         = 0;
    win              = 0;
    ultimoBoton      = 0;

    for (uint8_t i = 0; i < snakeLength; i++) {
        int8_t row = (int8_t)snakeHeadRow - (int8_t)i;
        if (row < 0) row += 8;
        gameboard[(uint8_t)row][snakeHeadCol] = snakeLength - i;
    }

    foodRow = 255;
    foodCol = 255;
}

static void generarComida(void) {
    if (snakeLength >= 64) { win = 1; return; }
    do {
        foodRow = rand_range(8);
        foodCol = rand_range(8);
    } while (gameboard[foodRow][foodCol] > 0);
}

static void actualizarSerpiente(void) {
    uint8_t newRow = snakeHeadRow;
    uint8_t newCol = snakeHeadCol;

    direction = pendingDirection;

    switch (direction) {
        case 1: newRow = (newRow > 0) ? newRow - 1 : 7; break;
        case 2: newCol = (newCol < 7) ? newCol + 1 : 0; break;
        case 3: newRow = (newRow < 7) ? newRow + 1 : 0; break;
        case 4: newCol = (newCol > 0) ? newCol - 1 : 7; break;
    }

    uint8_t ateFood = (newRow == foodRow && newCol == foodCol);

    if (gameboard[newRow][newCol] > 0 && !ateFood) {
        gameOver    = 1;
        gameRunning = 0;
        enviarEventoPIC(2);
        return;
    }

    for (uint8_t r = 0; r < 8; r++)
        for (uint8_t c = 0; c < 8; c++)
            if (gameboard[r][c] > 0) gameboard[r][c]--;

    snakeHeadRow = newRow;
    snakeHeadCol = newCol;

    if (ateFood) {
        enviarEventoPIC(1);
        snakeLength++;
        puntos += pgm_read_byte(&dificultades[dificultadSeleccionada].puntuacionExtra);

        uint16_t inc = pgm_read_word(&dificultades[dificultadSeleccionada].incrementoVelocidad);
        uint16_t vel = pgm_read_word(&dificultades[dificultadSeleccionada].velocidadInicial);
        snakeSpeed = vel - (snakeLength - 3) * inc;
        if (snakeSpeed < 30) snakeSpeed = 30;

        for (uint8_t r = 0; r < 8; r++)
            for (uint8_t c = 0; c < 8; c++)
                if (gameboard[r][c] > 0) gameboard[r][c]++;

        gameboard[snakeHeadRow][snakeHeadCol] = snakeLength;
        foodRow = 255;
        foodCol = 255;
    } else {
        gameboard[snakeHeadRow][snakeHeadCol] = snakeLength;
    }

    previousDirection = direction;
}

static void mostrarNumero(uint8_t num) {
    const uint8_t *nums[10] = {
        CERO, UNO, DOS, TRES, CUATRO,
        CINCO, SEIS, SIETE, OCHO, NUEVE
    };
    if (num <= 9) mostrarImagen(nums[num]);
}

static void mostrarPuntuacionFinal(void) {
    uint8_t decenas  = puntos / 10;
    uint8_t unidades = puntos % 10;
    if (decenas > 0) { mostrarNumero(decenas); delay_ms(500); }
    mostrarNumero(unidades);
    delay_ms(1000);
}

static void mostrarMensajeBienvenida(void) {
    const uint8_t *letras[] = {LETRA_S, LETRA_N, LETRA_A, LETRA_K, LETRA_E};
    mostrarTextoDesplazando(letras, 5);
    delay_ms(500);
    mostrarImagen(SIGNO);
    delay_ms(800);
    limpiarMatriz();
}

static void mostrarGameOver(void) {
    mostrarImagenMs(TRISTE, 1000);
    mostrarPuntuacionFinal();
    delay_ms(1000);
    limpiarMatriz();
}

static void mostrarWin(void) {
    mostrarImagenMs(FELIZ, 1000);
    mostrarPuntuacionFinal();
    delay_ms(1000);
    limpiarMatriz();
}


int main(void) {
    // Puertos de la matriz LED
    DDRD  = 0xFF;
    PORTD = 0x00;
    DDRB  = 0xFF;
    PORTB = 0xFF;

    // PC0..PC3: entradas con pull-up
    // PC4: salida hacia PIC 
    DDRC  = 0x00;
    PORTC = (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3);

    // Semilla PRNG con ADC flotante (canal 7)
    ADMUX  = (1 << REFS0) | 7;
    ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    while (ADCSRA & (1 << ADSC));
    lfsr = ADC ^ 0xA5B6;
    ADCSRA &= ~(1 << ADEN);

    limpiarMatriz();

    // Animación de bienvenida
    mostrarMensajeBienvenida();

    while (1) {

        // 1. Esperar cualquier botón para entrar al menú de dificultad
        esperarCualquierBoton();

        // 2. Seleccionar dificultad (UP/DOWN para cambiar, RIGHT/LEFT para confirmar)
        seleccionarDificultad();

        // 3. Inicializar y jugar
        inicializarJuego();
        generarComida();
        gameRunning = 1;
        gameOver    = 0;
        win         = 0;

        while (gameRunning) {

            // Leer dirección
            uint8_t boton = leerBoton();
            if (boton != 0 && boton != ultimoBoton) {
                if ((boton == 1 && previousDirection != 3) ||
                    (boton == 2 && previousDirection != 4) ||
                    (boton == 3 && previousDirection != 1) ||
                    (boton == 4 && previousDirection != 2)) {
                    pendingDirection = boton;
                }
                ultimoBoton = boton;
            } else if (boton == 0) {
                ultimoBoton = 0;
            }

            // Generar comida si no existe
            if (foodRow == 255 || foodCol == 255) {
                generarComida();
                if (win) {
                    gameRunning = 0;
                    enviarEventoPIC(3);
                    mostrarWin();
                    break;
                }
            }

            // Retardo con refresco de pantalla
            for (uint16_t i = 0; i < snakeSpeed; i++) {
                PORTD = PORT_FILAS[foodRow];
                PORTB = ~(1 << (7 - foodCol));
                _delay_us(500);
                dibujarTablero();
                _delay_us(500);

                uint8_t b2 = leerBoton();
                if (b2 != 0 && b2 != ultimoBoton) {
                    if ((b2 == 1 && previousDirection != 3) ||
                        (b2 == 2 && previousDirection != 4) ||
                        (b2 == 3 && previousDirection != 1) ||
                        (b2 == 4 && previousDirection != 2)) {
                        pendingDirection = b2;
                    }
                    ultimoBoton = b2;
                }
            }

            actualizarSerpiente();

            if (snakeLength >= 64) {
                win         = 1;
                gameRunning = 0;
                enviarEventoPIC(3);
                mostrarWin();
                break;
            }

            if (gameOver) {
                gameRunning = 0;
                mostrarGameOver();
                break;
            }
        }

        // 4. Esperar cualquier botón para reiniciar
        delay_ms(500);
        esperarCualquierBoton();

        // 5. Mostrar bienvenida y repetir
        mostrarMensajeBienvenida();
    }

    return 0;
}