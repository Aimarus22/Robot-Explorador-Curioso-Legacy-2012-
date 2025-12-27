//**************************************************************************************************
//ROBOT EXPLORADOR CURIOSO (ROBOTIC SOLUTIONS S.L.)
//AUTORES: AXIER ALBÍN, JOKIN MUGIKA, AIMAR ROLLÁN
//LSB ANDOAIN. G2E 2012 - 2013
//**************************************************************************************************

#include <16f876A.h>                          
#fuses 	XT, NOLVP, PUT, NOWDT							//Palabra 1 de configuración
#fuses	NOWRT, NOPROTECT 								//Palabra 2 de configuración
#device ADC=10											//Canal analógico de 10 bits
#use fast_io(A)                                         // configuración rápida de entradas y salidas
#use fast_io(B) 
#use fast_io(C) 
#use delay(clock=4000000)								//Frecuencia de trabajo
#use i2c(Master,sda=PIN_C4,scl=PIN_C3,slow,force_hw)	//Configuración blutooth 
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)			//Habilita las funciones RS232, velocidad a 9600 baudios
int a,b,c,d,e;											//Definición de variables
int16 distancia,dato,resultado,nivel;					
char distancia_L,distancia_H;                       //buffer de recepción de un dispositivo I2C

#int_ext												//Vector de interrupción externa RB0
	void ext_isr()
	{
		printf("G\r");									//Choque frontal
		a=2;
		b=1;
	}

#int_rda												//Vector de interrupción al recibir por el UART
	tratamiento()
	{	
		switch(getc())									//Lee el carácter recibido
		{
			case '0': a=0; b=0;break;					//Si es el comando es '0', Marcha adelante
			case '1': a=1;break;						//Si es el comando es '1', Marcha atras
			case '2': a=2;break;						//Si es el comando es '2', Parado
			case '3': a=3;break;						//Si es el comando es '3', Curva izquierda
			case '4': a=4;break;						//Si es el comando es '4', Curva derecha 
			case '5': a=5;break;						//Si es el comando es '5', Giro 180º
			case '6': a=6;break;						//Si es el comando es '6', 
			case '7': output_high(PIN_C0);break;        //Encender-Apagar linterna si el comando es '7'
			case '8': output_low(PIN_C0);break;         //Encender-Apagar linterna si el comando es '7'
			
	
		}	
	}

void peligro(void)										//Subrutina que alerta de obstáculos situados a menos de 20 cm
{
	if(distancia < 20 )
	{ 
		printf(" H%c%lucm\r",e,distancia);
	}
	else
	{
		printf(" %c%lucm\r",e,distancia);
	}
}

main()													//Programa principal
{   
	SETUP_ADC_PORTS(RA0_ANALOG);						//Puerta A y B Digitales
	setup_adc(ADC_CLOCK_INTERNAL);
	output_b(0x00);
	set_tris_a(0b00000001);								//Configuración de entradas y salidas
	set_tris_b(0b00011000);								//
	set_tris_c(0b10011110);								//RC6/TxD salida de datos y RC7/RxD entrada del EUSART
	putc('\r');											//Transmite CR
	putc('\n');											//Transmite avance de línea
	enable_interrupts(INT_RDA);							//Activa interrupciónes en la recepción
	enable_interrupts(INT_EXT);
	enable_interrupts(global);							//Habilita interrupciones
	a=2;
	b=0;

	while(1)
	{

		while(a==1&&b==0)								//MARCHA ADELANTE
		{	
			i2c_start(); 								//Arrancamos el sonar
			i2c_write(0xe0);							//Dirección de escritura del sonar
			i2c_write(0x00);							//Ubicacción del registro de comando
			i2c_write(0x51);							//Comando de distancia en cm
			i2c_stop();
			delay_ms(1);  					   			//Entre este delay y el de antes de I2c_start debe de haber 100 ms
			set_adc_channel(0);							//Selección del canal 0 (RA0)
			delay_us(10);								//Temporiza 10uS
			resultado=read_adc();						//Inicia la conversión y lee el resultado
			nivel=(resultado-594)*0.503;

			if(nivel>80)
			{
				e="A";									//Batería 5 barras
			}
			else if(nivel>60 && nivel<80)
			{
				e="B";									//Batería 4 barras
			}
			else if(nivel>40 && nivel<60)
			{
				e="C";									//Batería 3 barras
			}
			else if(nivel>20 && nivel<40)
			{
				e="D"; 									//Batería 2 barras
			}
			else if(nivel>10 && nivel<20)
			{
				e="E"; 									//Batería 1 barras
			}
			else if(nivel>=0 && nivel<10)
			{
				e="F"; 									//Batería 0 barras y color rojo
			}

			delay_ms(1);

			for(d=0;d<4;d++)
			{
				output_high(PIN_A2);
				output_high(PIN_A1);
				delay_us(1300);		
				output_low(PIN_A2);
				delay_us(400);	
				output_low(PIN_A1);
				delay_us(20000);
 			}

	   		output_high(PIN_A2);
			output_high(PIN_A1);
			delay_us(1300);		
			output_low(PIN_A2);
			delay_us(400);	
			output_low(PIN_A1);  
			delay_us(8500); 							//Entre este delay y la siguiente orde que active el motor debe haber 20 mS
 
			i2c_start();
			i2c_write(0xe0);							// dirección de escritura del sonar
			i2c_write(0x01);							// configuración para la lectura de luz y distancia
			i2c_stop();
			i2c_start();
			i2c_write(0xe1);							// dirección del sonar en lectura
			distancia_H=i2c_read(1);					// lee el byt alto de distancia
			distancia_L=i2c_read(0);					// lee el byt alto de distancia
			i2c_stop();
			distancia=make16(distancia_H,distancia_L);	// Valor real de distancia
			peligro();
			delay_us(8500);
		}

		while(a==0) 									//MARCHA ATRÁS
		{	
	
			i2c_start(); 								// arrancamos el sonar
			i2c_write(0xe0);							// dirección de escritura del sonar
			i2c_write(0x00);							// Ubicacción del registro de comando
			i2c_write(0x51);							//Comando de distancia en cm
			i2c_stop();
			delay_ms(1);
			set_adc_channel(0);							//Selección del canal 0 (RA0)
			delay_us(10);								//Temporiza 10uS
			resultado=read_adc();						//Inicia la conversión y lee el resultado
			nivel=(resultado-594)*0.503;
			delay_ms(1);

			for(d=0;d<4;d++)
			{
				output_high(PIN_A1);
				output_high(PIN_A2);
				delay_us(1300);		
				output_low(PIN_A1);
				delay_us(400);	
				output_low(PIN_A2);
				delay_us(20000);  
			}

			output_high(PIN_A1);
			output_high(PIN_A2);
			delay_us(1300);		
			output_low(PIN_A1);
			delay_us(400);	
			output_low(PIN_A2);  
			delay_us(8500);  
			i2c_start();
			i2c_write(0xe0);							// dirección de escritura del sonar
			i2c_write(0x01);							// configuración para la lectura de luz y distancia
			i2c_stop();
			i2c_start();
			i2c_write(0xe1);							// dirección del sonar en lectura
			distancia_H=i2c_read(1);					// lee el byt alto de distancia
			distancia_L=i2c_read(0);					// lee el byt alto de distancia
			i2c_stop();
			distancia=make16(distancia_H,distancia_L);	// Valor real de distancia
			peligro();
			delay_us(8500);
		}

		while(a==2) 									//PARADA
		{	

			i2c_start(); 								// arrancamos el sonar
			i2c_write(0xe0);							// dirección de escritura del sonar
			i2c_write(0x00);							// Ubicacción del registro de comando
			i2c_write(0x51);							//Comando de distancia en cm
			i2c_stop();
			delay_ms(1);
			set_adc_channel(0);							//Selección del canal 0 (RA0)
			delay_us(10);								//Temporiza 10uS
			resultado=read_adc();						//Inicia la conversión y lee el resultado
			nivel=(resultado-594)*0.503;
			if(nivel>80)
			{
				e="A";									//Batería 5 barras
			}
			else if(nivel>60 && nivel<80)
			{
				e="B";									//Batería 4 barras
			}
			else if(nivel>40 && nivel<60)
			{
				e="C";									//Batería 3 barras
			}
			else if(nivel>20 && nivel<40)
			{
				e="D"; 									//Batería 2 barras
			}
			else if(nivel>10 && nivel<20)
			{
				e="E"; 									//Batería 1 barras
			}
			else if(nivel>=0 && nivel<10)
			{
				e="F"; 									//Batería 0 barras y color rojo
			}
			delay_ms(1);
			for(d=0;d<4;d++)
			{
				output_high(PIN_A2);
				output_high(PIN_A1);
				delay_us(1500);
				output_low(PIN_A2);		
				output_low(PIN_A1);
				delay_us(20000);
			}
       		output_high(PIN_A2);
			output_high(PIN_A1);
			delay_us(1500);		
			output_low(PIN_A2);
			output_low(PIN_A1);  
			delay_us(9500);  
			i2c_start();
			i2c_write(0xe0);							// dirección de escritura del sonar
			i2c_write(0x01);							// configuración para la lectura de luz y distancia
			i2c_stop();
			i2c_start();
			i2c_write(0xe1);							// dirección del sonar en lectura
			distancia_H=i2c_read(1);					// lee el byt alto de distancia
			distancia_L=i2c_read(0);					// lee el byt alto de distancia
			i2c_stop();
			distancia=make16(distancia_H,distancia_L);	// Valor real de distancia
			peligro();
			delay_us(7500);
		}

		while(a==3&&b==0) 								//CURVA IZQUIERDA
		{
			i2c_start(); 								//Arrancamos el sonar
			i2c_write(0xe0);							//Dirección de escritura del sonar
			i2c_write(0x00);							//Ubicacción del registro de comando
			i2c_write(0x51);							//Comando de distancia en cm
			i2c_stop();
			delay_ms(1);  					   			//Entre este delay y el de antes de I2c_start debe de haber 100 ms
			set_adc_channel(0);							//Selección del canal 0 (RA0)
			delay_us(10);								//Temporiza 10uS
			resultado=read_adc();						//Inicia la conversión y lee el resultado
			nivel=(resultado-594)*0.503;
					if(nivel>80)
			{
				e="A";									//Batería 5 barras
			}
			else if(nivel>60 && nivel<80)
			{
				e="B";									//Batería 4 barras
			}
			else if(nivel>40 && nivel<60)
			{
				e="C";									//Batería 3 barras
			}
			else if(nivel>20 && nivel<40)
			{
				e="D"; 									//Batería 2 barras
			}
			else if(nivel>10 && nivel<20)
			{
				e="E"; 									//Batería 1 barras
			}
			else if(nivel>=0 && nivel<10)
			{
				e="F"; 									//Batería 0 barras y color rojo
			}
			delay_ms(1);
			for(d=0;d<4;d++)
			{
				output_high(PIN_A2);
				delay_us(1300);
				output_low(PIN_A2);		
				delay_us(20000);
   			}
	   		output_high(PIN_A2);
			delay_us(1300);		
			output_low(PIN_A2); 
			delay_us(13500); 							//Entre este delay y la siguiente orde que active el motor debe haber 20 mS
			i2c_start();
			i2c_write(0xe0);							// dirección de escritura del sonar
			i2c_write(0x01);							// configuración para la lectura de luz y distancia
			i2c_stop();
			i2c_start();
			i2c_write(0xe1);							// dirección del sonar en lectura
			distancia_H=i2c_read(1);					// lee el byt alto de distancia
			distancia_L=i2c_read(0);					// lee el byt alto de distancia
			i2c_stop();
			distancia=make16(distancia_H,distancia_L);	// Valor real de distancia
			peligro();
			delay_us(6500);     	
		}

		while(a==4&&b==0) 								//CURVA DERECHA
		{	
			i2c_start(); 								//Arrancamos el sonar
			i2c_write(0xe0);							//Dirección de escritura del sonar
			i2c_write(0x00);							//Ubicacción del registro de comando
			i2c_write(0x51);							//Comando de distancia en cm
			i2c_stop();
			delay_ms(1);  					   			//Entre este delay y el de antes de I2c_start debe de haber 100 ms
			set_adc_channel(0);							//Selección del canal 0 (RA0)
			delay_us(10);								//Temporiza 10uS
			resultado=read_adc();						//Inicia la conversión y lee el resultado
			nivel=(resultado-594)*0.503;
				if(nivel>80)
			{
				e="A";									//Batería 5 barras
			}
			else if(nivel>60 && nivel<80)
			{
				e="B";									//Batería 4 barras
			}
			else if(nivel>40 && nivel<60)
			{
				e="C";									//Batería 3 barras
			}
			else if(nivel>20 && nivel<40)
			{
				e="D"; 									//Batería 2 barras
			}
			else if(nivel>10 && nivel<20)
			{
				e="E"; 									//Batería 1 barras
			}
			else if(nivel>=0 && nivel<10)
			{
				e="F"; 									//Batería 0 barras y color rojo
			}
			delay_ms(1);
			for(d=0;d<4;d++)
			{
				output_high(PIN_A1);
				delay_us(1700);
				output_low(PIN_A1);		
				delay_us(20000);
   			}
	   		output_high(PIN_A1);
			delay_us(1700);		
			output_low(PIN_A1); 
			delay_us(11500); 							//Entre este delay y la siguiente orde que active el motor debe haber 20 mS
			i2c_start();
			i2c_write(0xe0);							// dirección de escritura del sonar
			i2c_write(0x01);							// configuración para la lectura de luz y distancia
			i2c_stop();
			i2c_start();
			i2c_write(0xe1);							// dirección del sonar en lectura
			distancia_H=i2c_read(1);					// lee el byt alto de distancia
			distancia_L=i2c_read(0);					// lee el byt alto de distancia
			i2c_stop();
			distancia=make16(distancia_H,distancia_L);	// Valor real de distancia
			peligro();
			delay_us(8500);  
		}

		while(a==5&&b==0) 								//GIRO 180º
		{	
			for(c=1;c<=122;c++)
			{
				output_high(PIN_A2);
				output_high(PIN_A1);
				delay_us(1700);
				output_low(PIN_A2);		
				output_low(PIN_A1);
				delay_us(20000);
			}

	 		a=2;
		}

	}	
}
