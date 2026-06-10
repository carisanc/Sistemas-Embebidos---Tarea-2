
_playEatSound:

;Tarea2Pic.c,17 :: 		void playEatSound(void) {
;Tarea2Pic.c,19 :: 		Sound_Play(2500, 120);
	MOVLW      196
	MOVWF      FARG_Sound_Play_freq_in_hz+0
	MOVLW      9
	MOVWF      FARG_Sound_Play_freq_in_hz+1
	MOVLW      120
	MOVWF      FARG_Sound_Play_duration_ms+0
	MOVLW      0
	MOVWF      FARG_Sound_Play_duration_ms+1
	CALL       _Sound_Play+0
;Tarea2Pic.c,20 :: 		}
L_end_playEatSound:
	RETURN
; end of _playEatSound

_playLoseSound:

;Tarea2Pic.c,22 :: 		void playLoseSound(void) {
;Tarea2Pic.c,24 :: 		Sound_Play(900, 120);
	MOVLW      132
	MOVWF      FARG_Sound_Play_freq_in_hz+0
	MOVLW      3
	MOVWF      FARG_Sound_Play_freq_in_hz+1
	MOVLW      120
	MOVWF      FARG_Sound_Play_duration_ms+0
	MOVLW      0
	MOVWF      FARG_Sound_Play_duration_ms+1
	CALL       _Sound_Play+0
;Tarea2Pic.c,25 :: 		Delay_ms(30);
	MOVLW      78
	MOVWF      R12+0
	MOVLW      235
	MOVWF      R13+0
L_playLoseSound0:
	DECFSZ     R13+0, 1
	GOTO       L_playLoseSound0
	DECFSZ     R12+0, 1
	GOTO       L_playLoseSound0
;Tarea2Pic.c,27 :: 		Sound_Play(700, 120);
	MOVLW      188
	MOVWF      FARG_Sound_Play_freq_in_hz+0
	MOVLW      2
	MOVWF      FARG_Sound_Play_freq_in_hz+1
	MOVLW      120
	MOVWF      FARG_Sound_Play_duration_ms+0
	MOVLW      0
	MOVWF      FARG_Sound_Play_duration_ms+1
	CALL       _Sound_Play+0
;Tarea2Pic.c,28 :: 		Delay_ms(30);
	MOVLW      78
	MOVWF      R12+0
	MOVLW      235
	MOVWF      R13+0
L_playLoseSound1:
	DECFSZ     R13+0, 1
	GOTO       L_playLoseSound1
	DECFSZ     R12+0, 1
	GOTO       L_playLoseSound1
;Tarea2Pic.c,30 :: 		Sound_Play(500, 120);
	MOVLW      244
	MOVWF      FARG_Sound_Play_freq_in_hz+0
	MOVLW      1
	MOVWF      FARG_Sound_Play_freq_in_hz+1
	MOVLW      120
	MOVWF      FARG_Sound_Play_duration_ms+0
	MOVLW      0
	MOVWF      FARG_Sound_Play_duration_ms+1
	CALL       _Sound_Play+0
;Tarea2Pic.c,31 :: 		Delay_ms(30);
	MOVLW      78
	MOVWF      R12+0
	MOVLW      235
	MOVWF      R13+0
L_playLoseSound2:
	DECFSZ     R13+0, 1
	GOTO       L_playLoseSound2
	DECFSZ     R12+0, 1
	GOTO       L_playLoseSound2
;Tarea2Pic.c,33 :: 		Sound_Play(250, 250);
	MOVLW      250
	MOVWF      FARG_Sound_Play_freq_in_hz+0
	CLRF       FARG_Sound_Play_freq_in_hz+1
	MOVLW      250
	MOVWF      FARG_Sound_Play_duration_ms+0
	CLRF       FARG_Sound_Play_duration_ms+1
	CALL       _Sound_Play+0
;Tarea2Pic.c,34 :: 		}
L_end_playLoseSound:
	RETURN
; end of _playLoseSound

_playWinSound:

;Tarea2Pic.c,36 :: 		void playWinSound(void) {
;Tarea2Pic.c,38 :: 		Sound_Play(523, 120);
	MOVLW      11
	MOVWF      FARG_Sound_Play_freq_in_hz+0
	MOVLW      2
	MOVWF      FARG_Sound_Play_freq_in_hz+1
	MOVLW      120
	MOVWF      FARG_Sound_Play_duration_ms+0
	MOVLW      0
	MOVWF      FARG_Sound_Play_duration_ms+1
	CALL       _Sound_Play+0
;Tarea2Pic.c,39 :: 		Delay_ms(30);
	MOVLW      78
	MOVWF      R12+0
	MOVLW      235
	MOVWF      R13+0
L_playWinSound3:
	DECFSZ     R13+0, 1
	GOTO       L_playWinSound3
	DECFSZ     R12+0, 1
	GOTO       L_playWinSound3
;Tarea2Pic.c,41 :: 		Sound_Play(659, 120);
	MOVLW      147
	MOVWF      FARG_Sound_Play_freq_in_hz+0
	MOVLW      2
	MOVWF      FARG_Sound_Play_freq_in_hz+1
	MOVLW      120
	MOVWF      FARG_Sound_Play_duration_ms+0
	MOVLW      0
	MOVWF      FARG_Sound_Play_duration_ms+1
	CALL       _Sound_Play+0
;Tarea2Pic.c,42 :: 		Delay_ms(30);
	MOVLW      78
	MOVWF      R12+0
	MOVLW      235
	MOVWF      R13+0
L_playWinSound4:
	DECFSZ     R13+0, 1
	GOTO       L_playWinSound4
	DECFSZ     R12+0, 1
	GOTO       L_playWinSound4
;Tarea2Pic.c,44 :: 		Sound_Play(784, 120);
	MOVLW      16
	MOVWF      FARG_Sound_Play_freq_in_hz+0
	MOVLW      3
	MOVWF      FARG_Sound_Play_freq_in_hz+1
	MOVLW      120
	MOVWF      FARG_Sound_Play_duration_ms+0
	MOVLW      0
	MOVWF      FARG_Sound_Play_duration_ms+1
	CALL       _Sound_Play+0
;Tarea2Pic.c,45 :: 		Delay_ms(30);
	MOVLW      78
	MOVWF      R12+0
	MOVLW      235
	MOVWF      R13+0
L_playWinSound5:
	DECFSZ     R13+0, 1
	GOTO       L_playWinSound5
	DECFSZ     R12+0, 1
	GOTO       L_playWinSound5
;Tarea2Pic.c,47 :: 		Sound_Play(1047, 300);
	MOVLW      23
	MOVWF      FARG_Sound_Play_freq_in_hz+0
	MOVLW      4
	MOVWF      FARG_Sound_Play_freq_in_hz+1
	MOVLW      44
	MOVWF      FARG_Sound_Play_duration_ms+0
	MOVLW      1
	MOVWF      FARG_Sound_Play_duration_ms+1
	CALL       _Sound_Play+0
;Tarea2Pic.c,48 :: 		}
L_end_playWinSound:
	RETURN
; end of _playWinSound

_medirPulso:

;Tarea2Pic.c,50 :: 		unsigned int medirPulso(void) {
;Tarea2Pic.c,52 :: 		unsigned int tiempo = 0;
	CLRF       medirPulso_tiempo_L0+0
	CLRF       medirPulso_tiempo_L0+1
;Tarea2Pic.c,54 :: 		while(SIGNAL_IN == 1) {
L_medirPulso6:
	BTFSS      RC0_bit+0, BitPos(RC0_bit+0)
	GOTO       L_medirPulso7
;Tarea2Pic.c,56 :: 		Delay_ms(1);
	MOVLW      3
	MOVWF      R12+0
	MOVLW      151
	MOVWF      R13+0
L_medirPulso8:
	DECFSZ     R13+0, 1
	GOTO       L_medirPulso8
	DECFSZ     R12+0, 1
	GOTO       L_medirPulso8
	NOP
	NOP
;Tarea2Pic.c,57 :: 		tiempo++;
	INCF       medirPulso_tiempo_L0+0, 1
	BTFSC      STATUS+0, 2
	INCF       medirPulso_tiempo_L0+1, 1
;Tarea2Pic.c,59 :: 		if(tiempo >= 1500)
	MOVLW      5
	SUBWF      medirPulso_tiempo_L0+1, 0
	BTFSS      STATUS+0, 2
	GOTO       L__medirPulso32
	MOVLW      220
	SUBWF      medirPulso_tiempo_L0+0, 0
L__medirPulso32:
	BTFSS      STATUS+0, 0
	GOTO       L_medirPulso9
;Tarea2Pic.c,60 :: 		break;
	GOTO       L_medirPulso7
L_medirPulso9:
;Tarea2Pic.c,61 :: 		}
	GOTO       L_medirPulso6
L_medirPulso7:
;Tarea2Pic.c,63 :: 		return tiempo;
	MOVF       medirPulso_tiempo_L0+0, 0
	MOVWF      R0+0
	MOVF       medirPulso_tiempo_L0+1, 0
	MOVWF      R0+1
;Tarea2Pic.c,64 :: 		}
L_end_medirPulso:
	RETURN
; end of _medirPulso

_main:

;Tarea2Pic.c,66 :: 		void main() {
;Tarea2Pic.c,68 :: 		ANSEL = 0;
	CLRF       ANSEL+0
;Tarea2Pic.c,69 :: 		ANSELH = 0;
	CLRF       ANSELH+0
;Tarea2Pic.c,71 :: 		TRISC0_bit = 1;   // Entrada desde ATmega
	BSF        TRISC0_bit+0, BitPos(TRISC0_bit+0)
;Tarea2Pic.c,72 :: 		TRISC2_bit = 0;   // Speaker
	BCF        TRISC2_bit+0, BitPos(TRISC2_bit+0)
;Tarea2Pic.c,74 :: 		PORTC = 0;
	CLRF       PORTC+0
;Tarea2Pic.c,76 :: 		Sound_Init(&PORTC, 2);
	MOVLW      PORTC+0
	MOVWF      FARG_Sound_Init_snd_port+0
	MOVLW      2
	MOVWF      FARG_Sound_Init_snd_pin+0
	CALL       _Sound_Init+0
;Tarea2Pic.c,78 :: 		Delay_ms(300);
	MOVLW      4
	MOVWF      R11+0
	MOVLW      12
	MOVWF      R12+0
	MOVLW      51
	MOVWF      R13+0
L_main10:
	DECFSZ     R13+0, 1
	GOTO       L_main10
	DECFSZ     R12+0, 1
	GOTO       L_main10
	DECFSZ     R11+0, 1
	GOTO       L_main10
	NOP
	NOP
;Tarea2Pic.c,80 :: 		while(1) {
L_main11:
;Tarea2Pic.c,83 :: 		if(SIGNAL_IN == 1) {
	BTFSS      RC0_bit+0, BitPos(RC0_bit+0)
	GOTO       L_main13
;Tarea2Pic.c,87 :: 		duracion = medirPulso();
	CALL       _medirPulso+0
	MOVF       R0+0, 0
	MOVWF      main_duracion_L2+0
	MOVF       R0+1, 0
	MOVWF      main_duracion_L2+1
;Tarea2Pic.c,90 :: 		if(duracion >= 50 && duracion < 250) {
	MOVLW      0
	SUBWF      R0+1, 0
	BTFSS      STATUS+0, 2
	GOTO       L__main34
	MOVLW      50
	SUBWF      R0+0, 0
L__main34:
	BTFSS      STATUS+0, 0
	GOTO       L_main16
	MOVLW      0
	SUBWF      main_duracion_L2+1, 0
	BTFSS      STATUS+0, 2
	GOTO       L__main35
	MOVLW      250
	SUBWF      main_duracion_L2+0, 0
L__main35:
	BTFSC      STATUS+0, 0
	GOTO       L_main16
L__main27:
;Tarea2Pic.c,92 :: 		playEatSound();
	CALL       _playEatSound+0
;Tarea2Pic.c,93 :: 		}
	GOTO       L_main17
L_main16:
;Tarea2Pic.c,96 :: 		else if(duracion >= 250 && duracion < 750) {
	MOVLW      0
	SUBWF      main_duracion_L2+1, 0
	BTFSS      STATUS+0, 2
	GOTO       L__main36
	MOVLW      250
	SUBWF      main_duracion_L2+0, 0
L__main36:
	BTFSS      STATUS+0, 0
	GOTO       L_main20
	MOVLW      2
	SUBWF      main_duracion_L2+1, 0
	BTFSS      STATUS+0, 2
	GOTO       L__main37
	MOVLW      238
	SUBWF      main_duracion_L2+0, 0
L__main37:
	BTFSC      STATUS+0, 0
	GOTO       L_main20
L__main26:
;Tarea2Pic.c,98 :: 		playLoseSound();
	CALL       _playLoseSound+0
;Tarea2Pic.c,99 :: 		}
	GOTO       L_main21
L_main20:
;Tarea2Pic.c,102 :: 		else if(duracion >= 750) {
	MOVLW      2
	SUBWF      main_duracion_L2+1, 0
	BTFSS      STATUS+0, 2
	GOTO       L__main38
	MOVLW      238
	SUBWF      main_duracion_L2+0, 0
L__main38:
	BTFSS      STATUS+0, 0
	GOTO       L_main22
;Tarea2Pic.c,104 :: 		playWinSound();
	CALL       _playWinSound+0
;Tarea2Pic.c,105 :: 		}
L_main22:
L_main21:
L_main17:
;Tarea2Pic.c,108 :: 		while(SIGNAL_IN == 1);
L_main23:
	BTFSS      RC0_bit+0, BitPos(RC0_bit+0)
	GOTO       L_main24
	GOTO       L_main23
L_main24:
;Tarea2Pic.c,111 :: 		Delay_ms(100);
	MOVLW      2
	MOVWF      R11+0
	MOVLW      4
	MOVWF      R12+0
	MOVLW      186
	MOVWF      R13+0
L_main25:
	DECFSZ     R13+0, 1
	GOTO       L_main25
	DECFSZ     R12+0, 1
	GOTO       L_main25
	DECFSZ     R11+0, 1
	GOTO       L_main25
	NOP
;Tarea2Pic.c,112 :: 		}
L_main13:
;Tarea2Pic.c,113 :: 		}
	GOTO       L_main11
;Tarea2Pic.c,114 :: 		}
L_end_main:
	GOTO       $+0
; end of _main
