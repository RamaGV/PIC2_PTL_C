#define COLUMNA1	PINCbits.PINC0_
#define COLUMNA2	PINCbits.PINC1_
#define COLUMNA3	PINCbits.PINC2_
#define COLUMNA4	PINCbits.PINC3_

#define FILA1		PORTBbits.PB0_
#define FILA2		PORTBbits.PB1_
#define FILA3		PORTBbits.PB2_
#define FILA4		PORTBbits.PB3_

uint8_t key;
int key_numero = 0;
unsigned char keys_PAD[16] = {
	'1', '2', '3', 'A',
	'4', '5', '6', 'B',
	'7', '8', '9', 'C',
'<', '0', '>', 'D'   };

void PAD_init(void){
	DDRBbits.DDRB0 = 1;
	DDRBbits.DDRB1 = 1;
	DDRBbits.DDRB2 = 1;
	DDRBbits.DDRB3 = 1;
	
	DDRCbits.DDRC0 = 0;
	DDRCbits.DDRC1 = 0;
	DDRCbits.DDRC2 = 0;
	DDRCbits.DDRC3 = 0;
	
	PORTCbits.PC0_ = 1;
	PORTCbits.PC1_ = 1;
	PORTCbits.PC2_ = 1;
	PORTCbits.PC3_ = 1;
}

uint8_t COLUMNA_barido(void){
	if(COLUMNA1 == 0){
		while(COLUMNA1 == 0 || COLUMNA2 == 0 || COLUMNA3 == 0 || COLUMNA4 == 0);
		return 1;
	}
	key++;
	
	if(COLUMNA2 == 0){
		while(COLUMNA1 == 0 || COLUMNA2 == 0 || COLUMNA3 == 0 || COLUMNA4 == 0);
		return 1;
	}
	key++;
	
	if(COLUMNA3 == 0){
		while(COLUMNA1 == 0 || COLUMNA2 == 0 || COLUMNA3 == 0 || COLUMNA4 == 0);
		return 1;
	}
	key++;
	
	if(COLUMNA4 == 0){
		while(COLUMNA1 == 0 || COLUMNA2 == 0 || COLUMNA3 == 0 || COLUMNA4 == 0);
		return 1;
	}
	key++;
	
	return 0;
}

void FILA_init(uint8_t fila){
	FILA1 = 1;
	FILA2 = 1;
	FILA3 = 1;
	FILA4 = 1;
	
	switch(fila){
		case 1: FILA1 = 0; break;
		case 2: FILA2 = 0; break;
		case 3: FILA3 = 0; break;
		case 4: FILA4 = 0; break;
	}
}

uint8_t PAD_leer(void){
	key = 0;
	
	for (int fila = 1; fila <= 4; fila++){
		FILA_init(fila);
		if(COLUMNA_barido()){
			return keys_PAD[key];
		}
	}
	
	return 0;
}
