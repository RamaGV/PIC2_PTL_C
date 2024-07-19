#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

void BTH_init() {
	// Configura USART como UART
	UCSR0C &=~ (1<<UMSEL00) &~ (1<<UMSEL01);
	
	// Desactiva paridad
	UCSR0C &=~ (1<<UPM00) &~ (1<<UPM01);
	
	// Bit de parada = 1
	UCSR0C &=~ (1<<USBS0);
	
	// Configura 8-bits para datos
	UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
	UCSR0B &=~ (1<<UCSZ02);
	
	// Calcula baudrate
	UCSR0A |= (1<<U2X0);
	UBRR0 = (F_CPU / 8 / 9600) - 1;
	
	// Habilita transmisión y recepción
	UCSR0B |= (1<<TXEN0) | (1<<RXEN0);
}

void BTH_recibe() {
	// Se limpia el buffer de envio.
	memset(PTL_datos.buffer_envio, 0, sizeof(PTL_datos.buffer_envio));
	int i = 0;
	
	while (i < sizeof(PTL_datos.buffer_envio) - 1) {
		while (!(UCSR0A & (1<<RXC0))); // Espera a que haya datos recibidos o se agote el tiempo
		
		PTL_datos.buffer_envio[i] = UDR0;
		if (PTL_datos.buffer_envio[i] == '\n') { // Espera el salto de línea
			break;
		}
		i++;
	}
	PTL_datos.buffer_envio[i] = '\0'; // Termina la cadena
	BTH_envia("OK\n");
}	

void BTH_envia(char *data){
	int i = 0;
	while(data[i] != '\0'){
		while (!(UCSR0A & (1<<UDRE0))); // Espera a que el buffer esté libre
		UDR0 = data[i];
		i++;
	}
}
