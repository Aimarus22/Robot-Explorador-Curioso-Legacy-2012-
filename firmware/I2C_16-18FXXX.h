/*****************************************************************************************************
                               					I2C_16FXXX.H
											Autor: Mikel Etxebarria
								(c) Ingeniería de Microsistemas Programados S.L.
										www.microcontroladores.com
												Bilbao 2007

El conjunto de funciones C de propósito general que se presentan a continuación permiten realizar las tareas 
básicas del protocolo I2C mediante un PIC16FXXX. Este fichero se debe incluir en los futuros programas fuente 
mediante la directiva #INCLUDE< >

Leer_I2C()	Lee un nº de bytes del dispositivo I2C. La variable Dir_I2C contiene la dirección
			I2C del dispositivo. La variable I2C_Dir_Ini contiene la dirección interna inicial. La variable
			I2C_N_Bytes contiene el nº de bytes a leer. Los bytes recibidos se depositan en RAM a 
			partir de la posición indicada por I2C_Buffer
Escr_I2C() 	Escribe sobre el dispositivo I2C un nº de bytes. La variable Dir_I2C contiene la
			dirección del dispositivo. La variable I2C_Dir_Ini contiene la dirección interna inicial. La
			variable I2C_N_Bytes contiene el nº de bytes a escribir. Se supone que los bytes a escribir están 
			previamente depositados en un buffer de memoria cuyo inicio está representado por I2C_Buffer */

/********************************************************************************************
Leer_I2C():	Lee un nº de bytes del dispositivo I2C. La variable Dir_I2C contiene la dirección
I2C del dispositivo. La variable I2C_Dir_Ini contiene la dirección interna inicial. La variable
I2C_N_Bytes contiene el nº de bytes a leer. Los bytes recibidos se depositan en RAM a 
partir de la posición indicada por I2C_Buffer*/

Leer_I2C(Dir_I2C,I2C_Dir_Ini,I2C_N_Bytes)
{
	int	indice=0;
	int	contador;
	i2c_start();					//Secuencia de inicio
	i2c_write(Dir_I2C);				//Dirección I2C del dispositivo modo escritura
	i2c_write(I2C_Dir_Ini);			//Envia dirección interna del dispositivo
	i2c_stop();						//Envía secuencia de stop
	i2c_start();					//Envía secuencia de inicio
	i2c_write(Dir_I2C|0b00000001);	//Envía dirección I2C del dispositivo en modo lectura

	For(contador=I2C_N_Bytes-1;contador>=1;contador--)	//Ultimo byte a leer ??
		{
		I2C_Buffer[indice]=i2c_read();	//No, leer el byte, envíar ACK y almacenar en el buffer
		indice++;						//Siguiente posición en el buffer
		}
	I2C_Buffer[indice]=i2c_read(0);		//Si, lee último byte, envía NACK y almacena en el buffer
	i2c_stop();							//Secuencia de stop	
}

/* *******************************************************************************************
Escr_I2C(): Escribe sobre el dispositivo I2C un nº de bytes. La variable Dir_I2C contiene la
dirección del dispositivo. La variable I2C_Dir_Ini contiene la dirección interna inicial. La
variable I2C_N_Bytes contiene el nº de bytes a escribir. Se supone que los bytes a escribir están 
previamente depositados en un buffer de memoria cuyo inicio está representado por I2C_Buffer */

Escr_I2C(Dir_I2C,I2C_Dir_Ini,I2C_N_Bytes)
{
	int	indice=0;
	int	contador;
	i2c_start();					//Secuencia de inicio
	i2c_write(Dir_I2C);				//Dirección I2C del dispositivo modo escritura
	i2c_write(I2C_Dir_Ini);			//Envia dirección interna del dispositivo

	For(contador=I2C_N_Bytes;contador>=1;contador--)	//Ultimo byte a escribir ??
		{
		i2c_write(I2C_Buffer[indice]);	//No, leer byte del buffer y enciarlo al dispositivo I2C
		indice++;						//Siguiente posición del buffer
		}
	i2c_stop();							//Secuencia de stop
}
