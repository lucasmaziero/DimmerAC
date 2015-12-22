/**************************************************************************
	    VARIACAO DE LIMINOSIDADE DE 0 A 100% AUTOMATICA(FULL RANGE)             
***************************************************************************
AUTOR: LUCAS MAZIERO - Eletric engineer                                                   
E-MAIL: lucas.mazie.ro@hotmail.com 									  
CIDADE: Santa Maria - Rio Grande do Sul - Brasil                                       
***************************************************************************
Versão: 1.0                                                             
Data: 01/03/2013  
Versão: 2.0                                                        
Modificado: 19/05/2015                                                                                                                                                                                                                                                 
**************************************************************************/
#include <TimerOne.h>	// Avaiable from http://www.arduino.cc/playground/Code/Timer1
#include <DimmerAC.h>

#define PIN_DIMMER 10 // pino de saida TRIAC
#define PIN_ZEROCROSS 2 //pinos 2 ou 3 que sao os pinos da interupcao externa do Arduino uno

int flag = 0;
int Dimmer = 0;

void setup () 
{   
  //begin(int _AC_pin,int pinZeroCross,int freq)
  DimmerAC_begin(PIN_DIMMER,PIN_ZEROCROSS,60);//intervalo de 0 a 255 
} 

void loop ()
{    
  if(flag==0)
  {
    Dimmer++;
    if(Dimmer == 255)
    {
      flag=1;
    }
    delay(10);
  }
  
  if(flag==1)
  {
    Dimmer--;
    if(Dimmer == 0)
    {
      flag=0;
    }
    delay(10);
  }
  
  DimmerAC_Dimming(Dimmer);
}