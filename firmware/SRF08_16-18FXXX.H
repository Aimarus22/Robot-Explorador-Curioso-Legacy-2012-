/*****************************************************************************************************
                               			SRF08_16-18FXXX.H
									Autor: Mikel Etxebarria
						(c) Ingeniería de Microsistemas Programados S.L.
								www.microcontroladores.com
										Bilbao 2007

El conjunto de rutinas que se presentan a continuación permiten realizar las funciones de control 
del sonar SRF08 de la firma Devantech mediante el interface I2C con un PIC16-18FXXX. Este fichero 
se debe incluir en los futuros programas fuente mediante la directiva #INCLUDE< > 

SRF08_in()		Realiza una medida en pulgadas.El resultado se almacen en I2C_Buffer[0],[1]
SRF08_cm()		Realiza una medida en centímetros.El resultado se almacen en I2C_Buffer[0],[1]
SRF08_us()		Realiza una medida en microsegundos.El resultado se almacen en I2C_Buffer[0],[1]
SRF08_Luz() 	Realiza la medida de luminosidad que recibe el sónar SRF08 a través de la célula LDR. 
				El resultado de 8 bits se deposita almacena en I2C_Buffer[0]
SRF08_Rango()	Ajusta el rango de la distancia a medir: distancia=((rango*43)+43). Para medidas de 
				distancia cortas, un menor rango permite medidas mas rápidas. Por defecto el rango
				es de 0xff (11 m).El nuevo rango se indica en SRF08_New_Rango
SRF08_Firm(): 	Lee la versión del firmware interno del sónar SRF08. El resultado se deposita en 
				I2C_Buffer[0]
SRF08_I2C_Dir()	Cambia la dirección I2C del SRF08. SRF08_New_Dir debe expresar la nueva dirección I2C */

#define	SRF08	0xe0				//Dirección I2C del SRF08

/*************************************************************************************************
SRF08_Ejec():El SRF08 ejecuta un determinado comando. */

SRF08_Ejec(SRF08_Comando)
{
	i2c_start();					//Secuencia de inicio
	i2c_write(SRF08);				//Dirección I2C del SRF08 modo escritura
	i2c_write(0x00);				//Dirección interna 0x00 (registro de comandos)
	i2c_write(SRF08_Comando);		//Enviar comando a ejecutar
	i2c_stop();						//Secuencia de stop
	delay_ms(80);					//Temporización de 80mS necesaria para realizar la ejecución

/*Secuencia para la lectura del resultado medido por el SRF08 */

	Leer_I2C(SRF08,0x2,0x02);		//Realiza la medida del resultado
}

/**********************************************************************************************
SRF08_in(): Realiza una medida en pulgadas.El resultado se almacen en I2C_Buffer[0],[1] */

SRF08_in()
{
	SRF08_Ejec(0x50);				//Ejecuta el comando
}

/**********************************************************************************************
SRF08_cm(): Realiza una medida en centímetros.El resultado se almacen en I2C_Buffer[0],[1] */

SRF08_cm()
{
	SRF08_Ejec(0x51);				//Ejecuta el comando
}

/**********************************************************************************************
SRF08_us(): Realiza una medida en microsegundos.El resultado se almacen en I2C_Buffer[0],[1] */

SRF08_us()
{
	SRF08_Ejec(0x52);				//Ejecuta el comando
}

/***********************************************************************************************************
SRF08_Luz: Realiza la medida de luminosidad que recibe el sónar SRF08 a través de la célula LDR. El resultado
de 8 bits se deposita almacena en I2C_Buffer[0] */

SRF08_Luz()
{
	SRF08_cm();		/*Al ejecutar un comando cualquiera, la medida de luz se actualiza. 
					Se ejecuta el comando de medir distancia en cm */

/*A continuación see el contenido de la posición 1 que contiene el valor procedente de la célula LDR */
	Leer_I2C(SRF08,1,1);	//Lee el registro del sensor de luz
}

/**********************************************************************************************************
SRF08_Rango: Ajusta el rango de la distancia a medir: distancia=((rango*43)+43). Para medidas de distancia
cortas, un menor rango permite medidas mas rápidas. Por defecto el rango es de 0xff (11 m). El nuevo rango
se indica en SRF08_New_Rango.*/

SRF08_Rango(int SRF08_New_Rango)
{
	i2c_start();					//Secuencia de inicio
	i2c_write(SRF08);				//Dirección I2C del SRF08 modo escritura
	i2c_write(0x02);				//Dirección interna 0x02 (registro de rango)
	i2c_write(SRF08_New_Rango);		//Transmite el nuevo rango
	i2c_stop();						//Secuencia de stop
}

/************************************************************************************************************
SRF08_Firm(): Lee la versión del firmware interno del sónar SRF08. El resultado se deposita en I2C_Buffer[0] */

SRF08_Firm()
{
	Leer_I2C(SRF08,0x0,0x01);		//Realiza la lectura del firmware
}

/***********************************************************************************************************
SRF08_I2C_Dir(): Cambia la dirección I2C del SRF08. SRF08_New_Dir debe expresar la nueva dirección I2C */

SRF08_I2C_Dir(int SRF08_New_Dir)

{
	i2c_start();					//Secuencia de inicio
	i2c_write(SRF08);				//Dirección I2C del SRF08 modo escritura
	i2c_write(0x00);				//Dirección interna 0x00 (registro de comandos)
	i2c_write(0xA0);				//1ª secuencia para el cambio de dirección
	i2c_stop();						//Secuencia de stop

	i2c_start();					//Secuencia de inicio
	i2c_write(SRF08);				//Dirección I2C del SRF08 modo escritura
	i2c_write(0x00);				//Dirección interna 0x00 (registro de comandos)
	i2c_write(0xAA);				//2ª secuencia para el cambio de dirección
	i2c_stop();						//Secuencia de stop 
	
	i2c_start();					//Secuencia de inicio
	i2c_write(SRF08);				//Dirección I2C del SRF08 modo escritura
	i2c_write(0x00);				//Dirección interna 0x00 (registro de comandos)
	i2c_write(0xA5);				//3ª secuencia para el cambio de dirección
	i2c_stop();						//Secuencia de stop 

	i2c_start();					//Secuencia de inicio
	i2c_write(SRF08);				//Dirección I2C del SRF08 modo escritura
	i2c_write(0x00);				//Dirección interna 0x00 (registro de comandos)
	i2c_write(SRF08_New_Dir);		//Nueva dirección I2C
	i2c_stop();						//Secuencia de stop 
}
