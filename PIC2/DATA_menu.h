#include <stdbool.h>
#include "DATA_PTL.h"

void LCD_agregar();
void LCD_bth();
void LCD_quitar();
void LCD_agregar_interno();
void LCD_quitar_interno();
void LCD_bth_interno();
void LCD_bth_vincular();
void LCD_bth_modificar();
void LCD_espera();
void LCD_envio_ok();

enum menu{
	// Menu principal
	opc_bth,
	opc_agregar,
	opc_quitar,
	
	// Menus internos.
	opc_agregar_interno,
	opc_quitar_interno,
	opc_bth_interno,
	opc_bth_vincular,
	opc_bth_modificar,
	manipulacion_espera	
};
enum menu menu_estadoActual = opc_bth;
typedef void (*Menu_FSM)();

Menu_FSM LCD_menu[] = {LCD_bth, LCD_agregar, LCD_quitar, LCD_agregar_interno, LCD_quitar_interno, LCD_bth_interno, LCD_bth_vincular, LCD_bth_modificar, LCD_espera, LCD_envio_ok};

void LCD_agregar(){
	LCD_clear();
	LCD_goto(1,1); LCD_print("12:00      Bat. 100%");
	LCD_goto(1,2); LCD_print("--------------------");
	LCD_goto(1,3); LCD_print("|      Agregar  >  |");
	LCD_goto(1,4); LCD_print("--------------------");
}
void LCD_bth(){
	LCD_clear();
	LCD_goto(1,1); LCD_print("12:00      Bat. 100%");
	LCD_goto(1,2); LCD_print("--------------------");
	LCD_goto(1,3); LCD_print("| <  Bluetooth  >  |");
	LCD_goto(1,4); LCD_print("--------------------");
}
void LCD_quitar(){
	LCD_clear();
	LCD_goto(1,1); LCD_print("12:00      Bat. 100%");
	LCD_goto(1,2); LCD_print("--------------------");
	LCD_goto(1,3); LCD_print("|  <   Quitar      |");
	LCD_goto(1,4); LCD_print("--------------------");
}

void LCD_celdas(){ // M�todo de muestreo que reutiliza c�digo en opci�n agregar y quitar
	// Carga datos que se van almacenando en la estructura de celdas, an�logo a la solicitud por enviar
	for(int i=0; i<9; i++){
		LCD_goto(celdas[i].LCD_X, celdas[i].LCD_Y);
		LCD_num(celdas[i].valor/10);
		LCD_goto(celdas[i].LCD_X+1, celdas[i].LCD_Y);
		LCD_num(celdas[i].valor%10);
	}
	
	// Primero se dirige a la posici�n que representa al n�mero de casilla/box seleccionada y muestra una X
	LCD_goto(celdas[indiceCelda].LCD_X - 2, celdas[indiceCelda].LCD_Y);
	LCD_print("X");
}
void LCD_agregar_interno(){
	LCD_clear();
	LCD_goto(1,1); LCD_print("Agregar    Bat. 100%");
	LCD_goto(1,2); LCD_print("|1:00 | 2:00 | 3:00|");
	LCD_goto(1,3); LCD_print("|4:00 | 5:00 | 6:00|");
	LCD_goto(1,4); LCD_print("|7:00 | 8:00 | 9:00|");
	
	LCD_celdas();
}
void LCD_quitar_interno(){
	LCD_clear();
	LCD_goto(1,1); LCD_print("Quitar     Bat. 100%");
	LCD_goto(1,2); LCD_print("|1:00 | 2:00 | 3:00|");
	LCD_goto(1,3); LCD_print("|4:00 | 5:00 | 6:00|");
	LCD_goto(1,4); LCD_print("|7:00 | 8:00 | 9:00|");
	
	LCD_celdas();
}

void LCD_bth_interno(){
	LCD_clear();
	LCD_goto(1,1); LCD_print("Bth.       Bat. 100%");
	LCD_goto(1,2); LCD_print("|   A - Vincular   |");
	LCD_goto(1,3); LCD_print("|   B - Modificar  |");
	LCD_goto(1,4); LCD_print("|   D - Volver     |");
}
void LCD_bth_vincular()
{
	LCD_clear();
	LCD_goto(1,1); LCD_print("Bth         Vincular");
	LCD_goto(1,2); LCD_print("| A- Resistencias  |");
	LCD_goto(1,3); LCD_print("| B- Tornillos     |");
	LCD_goto(1,4); LCD_print("| D- Volver        |");
}
void LCD_bth_modificar(){
	LCD_clear();
	LCD_goto(1,1); LCD_print("Bth        Modificar");
	LCD_goto(1,2); LCD_print("| A- Resistencias  |");
	LCD_goto(1,3); LCD_print("| B- Tornillos     |");
	LCD_goto(1,4); LCD_print("| D- Volver        |");
}
void LCD_stock(){
	LCD_clear();
	LCD_goto(1,1); LCD_print("Stock      Bat. 100%");
	LCD_goto(1,2); LCD_print("|1:00 | 2:00 | 3:00|");
	LCD_goto(1,3); LCD_print("|4:00 | 5:00 | 6:00|");
	LCD_goto(1,4); LCD_print("|7:00 | 8:00 | 9:00|");
	
	// Carga datos que se van almacenando en la estructura de celdas, an�logo a la solicitud por enviar
	for(int i=0; i<9; i++){
		LCD_goto(celdas[i].LCD_X, celdas[i].LCD_Y);
		LCD_num(PTL_datos.celdas_stock[i].valor/10);
		LCD_goto(celdas[i].LCD_X+1, celdas[i].LCD_Y);
		LCD_num(PTL_datos.celdas_stock[i].valor%10);
	}
}
void LCD_enviando(){
	LCD_clear();
	LCD_goto(1,1); LCD_print("|                  |");
	LCD_goto(1,2); LCD_print("|     Enviando     |");
	LCD_goto(1,3); LCD_print("|     Solicitud    |");
	LCD_goto(1,4); LCD_print("|                  |");
}
void LCD_vaciar(){
	LCD_clear();
	LCD_goto(1,1); LCD_print("|                  |");
	LCD_goto(1,2); LCD_print("|     Vaciando..   |");
	LCD_goto(1,3); LCD_print("|                  |");
	LCD_goto(1,4); LCD_print("|                  |");
}
void LCD_vinculando(){
	LCD_clear();
	LCD_goto(1,1); LCD_print("|                  |");
	LCD_goto(1,2); LCD_print("|   Vinculando..  |");
	LCD_goto(1,3); LCD_print("|                  |");
	LCD_goto(1,4); LCD_print("|                  |");
}

void LCD_modificar(){
	LCD_clear();
	LCD_goto(1,1); LCD_print("|                  |");
	LCD_goto(1,2); LCD_print("|    Modificar..   |");
	LCD_goto(1,3); LCD_print("|                  |");
	LCD_goto(1,4); LCD_print("|                  |");
}

void LCD_espera(){
	LCD_clear();
	LCD_goto(1,1); LCD_print("|                  |");
	LCD_goto(1,2); LCD_print("| Manipulacion  de |");
	LCD_goto(1,3); LCD_print("| items en proceso |");
	LCD_goto(1,4); LCD_print("|                  |");
}

void LCD_envio_ok(){
	LCD_clear();
	LCD_goto(1,1); LCD_print("|                  |");
	LCD_goto(1,2); LCD_print("|     Solicitud    |");
	LCD_goto(1,3); LCD_print("|     finalizada   |");
	LCD_goto(1,4); LCD_print("|                  |");
}


