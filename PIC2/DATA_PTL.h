#include <string.h>

struct celda {
	int valor;
	int LCD_X;
	int LCD_Y;
};
struct celda celdas[9];
int indiceCelda = 0;

struct datosPTL{
	char* addr_bth;
	char buffer_envio[35];
	// Almacena stock.
	int indiceCeldaStock ;
	struct celda celdas_stock[9];
};
struct datosPTL PTL_datos;

void DATA_PTL_cargarStock() {
	// Puntero para recorrer el comando
	const char* ptr = PTL_datos.buffer_envio + 4; // Salta "AT$C"
	int i = 0;
	
	while (*ptr != '\0' && i < 9) {
		// Busca el siguiente numero
		int valor = strtol(ptr, (char**)&ptr, 10);
		// Almacena el numero en la estructura
		PTL_datos.celdas_stock[i].valor = valor;
		i++;
		// Salta el caructer '*'
		if (*ptr == '*') {
			ptr++;
		}
	}
	
	// Actualiza el undice de la celda
	PTL_datos.indiceCeldaStock = i;
}


void DATA_PTL_generarEnvio(struct datosPTL nuevoPTL) {
	for (int i = 0; i < 9; i++) {
		char cantidad_celda[3]; 						// Almacena la cantidad de la celda
		sprintf(cantidad_celda, "%d", celdas[i].valor); // Pasa de int a char[]		
		strcat(nuevoPTL.buffer_envio, cantidad_celda);  // Concatena la cantidad al buffer de envio
		if(i<8){
			strcat(nuevoPTL.buffer_envio, "*"); 		// Agrega * al final de cada celda menos la última
		}	
	}
	strcat(nuevoPTL.buffer_envio, "\n"); // Agrega \n al final del buffer
	// CREOQ UE HAY QUE SACAR EL DE ABAJO
	BTH_envia(nuevoPTL.buffer_envio);
}

void DATA_PTL_mostrarEnvio(struct datosPTL nuevoPTL){
	LCD_goto(1,1); LCD_print("12:00      Agr. 100%");
	LCD_goto(1,2); LCD_print("-   Buffer envio   -");
	LCD_goto(1,3); LCD_print("| ");
	// AQUI MOSTRAR.
	LCD_print(" |");
	LCD_goto(1,4); LCD_print("--------------------");
}

void DATA_PTL_vaciarCeldas(){
	for(int i=0; i<9; i++){
		celdas[i].valor = 0;
	}
}

void DATA_PTL_cargaXY(){
	celdas[0].LCD_X = 4;
	celdas[0].LCD_Y = 2;

	celdas[1].LCD_X = 11;
	celdas[1].LCD_Y = 2;
	
	celdas[2].LCD_X = 18;
	celdas[2].LCD_Y = 2;
	
	celdas[3].LCD_X = 4;
	celdas[3].LCD_Y = 3;

	celdas[4].LCD_X = 11;
	celdas[4].LCD_Y = 3;
	
	celdas[5].LCD_X = 18;
	celdas[5].LCD_Y = 3;
	
	celdas[6].LCD_X = 4;
	celdas[6].LCD_Y = 4;

	celdas[7].LCD_X = 11;
	celdas[7].LCD_Y = 4;
	
	celdas[8].LCD_X = 18;
	celdas[8].LCD_Y = 4;
}

