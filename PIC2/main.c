#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "DEF_ATMEGA328P.h"
/* DEF_ATMEGA328P.h>
	------ Estructura de datos para cada puerto y pin, posteriormente utilizados en cada componente para su configuraci�n.
*/
#include "LCD.h"
/* LCD.h>
	------ void LCD_instanciar(void)> Inicializa LCD seg�n configuraci�n de puertos y pines del Atmega328 que se conectan con la LCD.
	------ void LCD_goto(uint8_t x, uint8_t y)> Posiciona mediante el comando 0x80 comprendido en la LCD, para imprimir el pr�ximo d�gito en la posici�n indicada por el argumento (x e y).
	------ void LCD_print(char *)> Imprime el char ingresado por argumento, enviando nible por nible.
	------ void LCD_num(int)> Imprime el valor de tipo int ingresado por argumetno, convierte el valor y utiliza LCD_print().
	------ void LCD_clear(void)> Vac�a la pantalla mediante el comando 0x01 comprendido en la LCD.
*/
#include "PAD.h"
/*	PAD.h>
	------ char keys_PAD[16]> Array de char que representa a cada key del PAD.
	------ void PAD_init(void)> Configura los puertos y pines del Atmega328 para conectar el PAD.
	------ uint8_t COLUMNA_scan(void)> Escanea columna por columna detectando si alguna tecla fue presionada. En caso afirmativo, retorna 1 y deja la variable global columna en la posici�n seleccionada.
	------ void FILA_init(uint8_t fila)> Activa todas las filas y deja en 0 la fila indicada por el argumento.
	------ uint8_t PAD_leer(void)> Realiza la lectura completa del teclado, recorriendo cada fila y escaneando las columnas. Si detecta una tecla presionada, retorna el car�cter correspondiente seg�n keys_PAD.
*/
#include "DATA_menu.h"
#include "BTH.h"

bool decenasCargadas = false;
bool modo_espera = false;

int main(void){
	int modo_debug = 0;
	LCD_instanciar();
	DATA_PTL_cargaXY();
	BTH_init();
	PAD_init();
  
  LCD_clear();
	LCD_goto(1,1);
	LCD_print("  Inicializando LCD");
	_delay_ms(3000);
  
	menu_estadoActual = opc_bth;
	LCD_menu[menu_estadoActual]();
  
  // RECIBIR Y PROCESAR NUEVO STOCK
  BTH_envia("AT$C\0");
  _delay_ms(2000); 
  BTH_recibe();
  DATA_PTL_cargarStock();
  
	while (1){
		uint8_t key = PAD_leer(); // Si se presiona el pad, guarda la key presionada
    
		if (key != 0){
			switch (menu_estadoActual){
        case opc_agregar:
          switch (key){
          case '0':
            menu_estadoActual = opc_agregar_interno;
            break;
          case '>':
            menu_estadoActual = opc_bth;
            break;
          }
        break;
        
        case opc_bth:
          switch (key){
          case '0':
            menu_estadoActual = opc_bth_interno;
            break;
          case '<':
            menu_estadoActual = opc_agregar;
            break;
          case '>':
            menu_estadoActual = opc_quitar;
            break;
          }
        break;
        
        case opc_quitar:
          switch (key){
          case '0':
            menu_estadoActual = opc_quitar_interno;
            break;
          case '<':
            menu_estadoActual = opc_bth;
            break;
          }
        break;
        
          /*
                  |--------------------------------------------------------|
                  |  / \                                            / \    |
                  | < # >     Opcion para agregar objetos al PTL   < # >   |
                  |  \ /                                            \ /    |
                  |--------------------------------------------------------|
          */
        case opc_agregar_interno:
          switch (key){
            case 'A':
              LCD_enviando();
              strcat(PTL_datos.buffer_envio,  "AT$A");
              DATA_PTL_generarEnvio(PTL_datos);
              
              menu_estadoActual = manipulacion_espera;
            break;
            
            case 'B':
              LCD_stock();
              _delay_ms(3000);
            break;
            
            case 'C':
              LCD_vaciar();
              DATA_PTL_vaciarCeldas();
              _delay_ms(3000);
            break;
            
            case 'D':
              menu_estadoActual = opc_agregar;
            break;
            
            case '<':
              decenasCargadas = false;
              if (indiceCelda == 0)
                indiceCelda = 8;
              else
                indiceCelda--;
            break;
            
            case '>':
              decenasCargadas = false;
              if (indiceCelda == 8)
                indiceCelda = 0;
              else
                indiceCelda++;
            break;
            
            default:
              key_numero = key - 48; // Resta 48 porque los numeros en ASCII comienzan en el indice 48 [ASCII(48) = '0'].
              
              if (decenasCargadas){
                celdas[indiceCelda].valor += key_numero;
                indiceCelda++; // Luego de cargar las unidades el indice de carga avanza.
              }else{
                celdas[indiceCelda].valor = 0; // Si se vuelve a cargar, se reestablece.
                celdas[indiceCelda].valor += (10 * key_numero);
              }
              
              decenasCargadas = !decenasCargadas;
            break;
          }
          break;
          /*
                  |--------------------------------------------------------|
                  |  / \                                            / \    |
                  | < # >           Opcion de bth interno		     < # >   |
                  |  \ /                                            \ /    |
                  |--------------------------------------------------------|
          */
        case opc_bth_interno:
          switch (key){
          case 'A':
            menu_estadoActual = opc_bth_vincular;
            break;
          case 'B':
            menu_estadoActual = opc_bth_modificar;
            break;
          case 'D':
            menu_estadoActual = opc_bth;
            break;
          }
        break;
          /*
                  |--------------------------------------------------------|
                  |  / \                                            / \    |
                  | < # >          Opcion de bth vincular          < # >   |
                  |  \ /                                            \ /    |
                  |--------------------------------------------------------|
          */
        case opc_bth_vincular:
          switch (key){
          case 'A':
            // LCD_vinculando();
            _delay_ms(200);
            break;
          case 'B':
            // LCD_vinculando();
            _delay_ms(200);
            break;
          case '1':
            LCD_clear();
            LCD_goto(1, 1);
            LCD_print("1: ");
            // BTH_envia("H1");
            _delay_ms(300);
            break;
          case '2':
            LCD_clear();
            LCD_goto(1, 1);
            LCD_print("2: ");
            // BTH_envia("H2");
            _delay_ms(300);
            break;
          case '3':
            LCD_clear();
            LCD_goto(1, 1);
            LCD_print("3: ");
            // BTH_envia("H3");
            _delay_ms(300);
            break;
          case 'D':
            menu_estadoActual = opc_bth_interno;
            break;
          }
          break;
          /*
                  |--------------------------------------------------------|
                  |  / \                                            / \    |
                  | < # >          Opcion de bth modificar         < # >   |
                  |  \ /                                            \ /    |
                  |--------------------------------------------------------|
          */
        case opc_bth_modificar:
          switch (key){
          case 'A':
            // LCD_modificar();
            _delay_ms(200);
            break;
          case 'B':
            // LCD_modificar();
            _delay_ms(200);
            break;
          case 'D':
            menu_estadoActual = opc_bth_interno;
            break;
          }
        break;
          /*
                  |--------------------------------------------------------|
                  |  / \                                            / \    |
                  | < # >     Opcion para quitar objetos del PTL   < # >   |
                  |  \ /                                            \ /    |
                  |--------------------------------------------------------|
          */
        case opc_quitar_interno:
          switch (key){
          case 'A':
            LCD_enviando();
            strcat(PTL_datos.buffer_envio,  "AT$B");
            DATA_PTL_generarEnvio(PTL_datos);
            
            menu_estadoActual = manipulacion_espera;
          break;
          case 'B': // VER STOCK
            LCD_stock(); // MUESTRA STOCK ALMACENADO
            _delay_ms(3000);
          break;
          case 'C': // VACIA CELDAS
            LCD_vaciar();
            DATA_PTL_vaciarCeldas();
            _delay_ms(200);
          break;
          case 'D': // SALIR
            menu_estadoActual = opc_quitar;
          break;
          case '<':
            decenasCargadas = false;
            if (indiceCelda == 0)
              indiceCelda = 8;
            else
              indiceCelda--;
          break;
          case '>':
            decenasCargadas = false;
            if (indiceCelda == 8)
              indiceCelda = 0;
            else
              indiceCelda++;
          break;
          
          default:
            key_numero = key - 48; // Se sustrae 48, porque los n�meros representados en ASCII comienzan en el caracter 48 [ASCII(48) = '0'].
            
            if (decenasCargadas){
              celdas[indiceCelda].valor += key_numero;
              indiceCelda++; // Luego de cargar las unidades el �ndice de carga avanza.
            } else{
              celdas[indiceCelda].valor = 0; // Si se vuelve a cargar, se reestablece.
              celdas[indiceCelda].valor += (10 * key_numero);
            }
            
            decenasCargadas = !decenasCargadas;
            break;
          }
        break;
          /*
                  |--------------------------------------------------------|
                  |  / \                                            / \    |
                  | < # >     Opcion para quitar objetos del PTL   < # >   |
                  |  \ /                                            \ /    |
                  |--------------------------------------------------------|
          */
        
        case manipulacion_espera:
          modo_espera = true;
          
          switch(key){  
            case '9':
              PTL_datos.buffer_envio[0] = 'O';
              PTL_datos.buffer_envio[1] = 'K';
            break;
          }
        break;
      }
      
			// Muestra en pantalla luego de presionar el pad
			_delay_ms(150);
			LCD_menu[menu_estadoActual]();
		}
    
    if (modo_espera && (PTL_datos.buffer_envio[0] == 'O' && PTL_datos.buffer_envio[1] == 'K')){
        // MUESTRA MENSAJE DE ENVIO EXITOSO
        LCD_envio_ok();    
        
        // RECIBIR Y PROCESAR NUEVO STOCK
        BTH_envia("AT$C\0");
        _delay_ms(2000); 
        BTH_recibe();
        DATA_PTL_cargarStock();
        
        // Se limpia el buffer de envio y vuelve a 0 el indice.
        memset(PTL_datos.buffer_envio, 0, sizeof(PTL_datos.buffer_envio));
        DATA_PTL_vaciarCeldas();
        indiceCelda = 0;
        
        // Vuelve al estado opc_bth para mostrar el menu principal.
        menu_estadoActual = opc_bth;
        LCD_bth();
        modo_espera = false;
    }
	}
}
