//								LCD4bitsPIC16.h
//						Autor: Mikel Etxebarria
//			(c) Ingeniería de Microsistemas Programados S.L.
//						www.microcontroladores.com
//								Bilbao 2009
//
/*Este conjunto de rutinas diseñadas para PIC16 que permiten realizar las tareas 
básicas de control del módulo de visualización LCD, mediante un interface de 4 bits. 
Este fichero se debe incluir en los futuros programas fuente mediante la directiva INCLUDE

Las rutinas están adaptadas a las velocidades más lentas de algunos módulos LCD, en las cuales 
el tiempo de activación (Tc) entre una señal ENABLE y la siguiente es de unos 4 uS. La 
velocidad de trabajo se supone de 4MHz. */

/*lcd_enviar(LCD_COMANDO/LCD_DATO,valor)

				Permite enviar un comando (LCD_COMANDO) o un dato (LCD_DATO)
				valor representa el valor hex. a enviar

lcd_init()   	Debe ser llamada antes que las otras funciones.

lcd_putc(c)  	Visualiza c en la siguiente posición del display.
               	Caracteres especiales de control:
                      \f  Borrar display
                      \n  Saltar a la segunda linea
                      \b  Retroceder una posición.

lcd_gotoxy(x,y) Selecciona la nueva posición de escritura en el display.
                (la esquina superior izquierda es 1,1)

lcd_getc(x,y)   Devuelve el caracter de la posición x,y del display. */

/*Se supone un interface de 4 bits. RB-RB3 es la puerta de datos. RA1-RA3 son las señales de control. 
Estas conexiones se pueden modificar según se necesite */

#bit  lcd_enable = 0x05.1		//RA1 --> Enable
#bit  lcd_rw     = 0x05.2		//RA2 --> R/W
#bit  lcd_rs     = 0x05.3		//RA3 --> RS
#byte lcd_a		=0x05			//Puerta A
#byte tris_a	=0x85			//Reg. TRIS A
#byte lcd_b 	= 0x06			//Puerta B
#byte tris_b	= 0x86			//Reg. TRIS B

#define LCD_LINEA2 0x40    		// Dirección de memoria para la segunda línea
#define LCD_DATO    1			// Modo dato
#define LCD_COMANDO 0			// Modo comando

//Definición de los códigos de los comandos mas frecuentes

#define LCD_CLEAR       		0x01
#define LCD_HOME	 			0x02

#define LCD_DISPLAY_OFF  		0x08
#define LCD_DISPLAY_ON   		0x0C
#define LCD_CURSOR_ON    		0x0E
#define LCD_CURSOR_BLINK 		0x0F

#define LCD_CURSOR_SHIFT_RIGHT  0x10
#define LCD_CURSOR_SHIFT_LEFT   0x14
#define LCD_DISPLAY_SHIFT_RIGHT 0x18
#define LCD_DISPLAY_SHIFT_LEFT  0x1C

//Realiza un ciclo de lectura de la pantalla LCD

int lcd_leer()	
{
	int valor, temp;
	int	temp_tris_a,temp_tris_b;
	temp_tris_a=tris_a;
	temp_tris_b=tris_b;
	set_tris_a(0b11110001);			//Configura RA1-RA3 como salidas
  	set_tris_b(0Xff);				//Configura RB0-RB3 como entradas
	lcd_rs = 0;						//Modo comando
  	lcd_rw = 1;						//Modo lectura
  	delay_cycles(1);
  	lcd_enable = 1;					//Activación del LCD
  	delay_cycles(1);
	
	temp=lcd_b;
	swap(temp);
	temp=temp&0xf0;					//Lectura de los bits de mas peso

  	lcd_enable = 0;					//Desactivación del LCD
  	delay_cycles(1);
	lcd_enable = 1;					//Activa el LCD
	delay_cycles(1);

	valor=(lcd_b&0x0f)|temp;		//Lectura de los bits de menos peso

	delay_cycles(1);
	lcd_enable = 0;					//Desactiva el LCD
	set_tris_a(temp_tris_a);
	set_tris_b(temp_tris_b);		//Reconfigura las puertas A y B
  	return valor;
}

//Envia a la pantalla un comando (dir=1) o un dato (dir=0)

void lcd_enviar(int dir, int valor)
{
	int	temp;
	int temp_tris_a,temp_tris_b;
	temp_tris_a=tris_a;
	temp_tris_b=tris_b;
  	set_tris_b(0xf0);				//Configura RB0-RB3 como salidas
  	lcd_rs = 0;						//Modo comando
	lcd_rw = 1;						//Modo lectura

  	while( bit_test(lcd_leer(),7) );	// Lectura del bit bussy del LCD para saber si está libre
  	lcd_rs = dir;					//Establece el modo (comando/dato)
  	delay_cycles(1);
  	lcd_rw = 0;						//Modo escritura
  	delay_cycles(1);
  	lcd_enable = 0;

	temp=valor;
	lcd_b=lcd_b&0xf0;
	swap(temp);
	temp=temp&0x0f;
	lcd_b=temp|lcd_b;				//Envia los 4 bits de mas peso

  	delay_cycles(1);
  	lcd_enable = 1;					//Habilita el LCD
  	delay_us(2);
  	lcd_enable = 0;					//Desactiva el LCD
	lcd_b=lcd_b&0xf0;

	lcd_b = (valor&0x0f)|lcd_b;		//Envía los 4 bits de menos peso

  	delay_cycles(1);
  	lcd_enable = 1;					//Habilita el LCD
  	delay_us(2);
  	lcd_enable = 0;					//Desactiva el LCD
	set_tris_a(temp_tris_a);		//Repone puerto A
	set_tris_b(temp_tris_b);		//Repone puerto B
}
 
//Función para inicio de la pantalla LCD según especificaciones del fabricante

void lcd_init()
{
	set_tris_a(0b11110001);			//RA1-RA3 salidas
  	set_tris_b(0xf0);				//RB3-RB0 salidas
  	lcd_enable = 0;					//Desactiva LCD
  	lcd_rw = 0;						//Modo escritura
  	lcd_rs = 0;						//Modo comando
  	delay_ms(15);					//Temporiza 15mS tras el power on del LCD

	lcd_b = (lcd_b&0xf0)|0x03;		//Comando FUNCTION SET sin chequeo de busy
	lcd_enable = 1;					//Activa LCD
	delay_uS(2);
	lcd_enable =0;					//Desactiva LCD
	delay_ms(5);					//Temporiza 5 ms

	lcd_b = (lcd_b&0xf0)|0x03;		//Comando FUNCTION SET sin chequeo de busy
	lcd_enable = 1;					//Activa LCD
	delay_uS(2);
	lcd_enable =0;					//Desactiva LCD
	delay_ms(1);					//Temporiza 1 ms

	lcd_b = (lcd_b&0xf0)|0x02;		//Comando FUNCTION SET sin chequeo de busy e interface 4 bits
	lcd_enable = 1;					//Activa LCD
	delay_uS(2);
	lcd_enable =0;					//Desactiva LCD

 	lcd_enviar(LCD_COMANDO,LCD_DISPLAY_ON);	//Comando Display ON, cursor OFF y blink OFF
  	lcd_enviar(LCD_COMANDO,0x06);			//Comando para modo incremento del cursor
  	lcd_enviar(LCD_COMANDO,LCD_CLEAR);		//Comando para borrar el LCD y colocar cursor en 1ª posición
}

//Función para situar el cursor

void lcd_gotoxy( byte x, byte y) 
{
   	byte dir=0x80;
	switch (y)
	{	case 1: dir=0x80+x-1;break;
		case 2: dir=0xc0+x-1;break;
		case 3: dir=0x94+x-1;break;
		case 4: dir=0xd4+x-1;break;
	}
	lcd_enviar(LCD_COMANDO,dir);	
}

//Visualiza un caracter

void lcd_putc( char c) {
   switch (c) {
     case '\f'   : lcd_enviar(LCD_COMANDO,0x01);
                   delay_ms(2);
                   break;
     case '\n'   : lcd_gotoxy(1,2);               break;
     case '\b'   : lcd_enviar(LCD_COMANDO,0x10);  break;
     default     : lcd_enviar(LCD_DATO,c);        break;
   }
}

//Devuelve el caracter

char lcd_getc( int x, int y) {
   char valor;

    lcd_gotoxy(x,y);

    lcd_rs = 1;
    valor = lcd_leer();
    lcd_rs = 0;

    return valor;
}

