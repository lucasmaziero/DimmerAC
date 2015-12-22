/**************************************************************************
	LIBRARY FOR DIMMER AC 110V/220V(CONTROLE DE ANGULO DE DISPARO)                 
***************************************************************************
AUTOR: LUCAS MAZIERO - Eletric engineer                                                   
E-MAIL: lucas.mazie.ro@hotmail.com 									  
CIDADE: Santa Maria - Rio Grande do Sul - Brasil                                       
***************************************************************************
Versão: 1.0                                                             
Data: 01/03/2013  
Versão: 2.0                                                        
Modificado: 19/05/2015                                                                                                                          
**************************************************************************
BASE DO CODIGO:
//https://community.particle.io/t/trying-to-port-an-ac-dimmer-circuit-from-arduino-to-sparkcore/10968
//http://arduinotehniq.blogspot.com.br/2014/10/ac-light-dimmer-with-arduino.html
**************************************************************************/
#ifndef DimmerAC_h
#define DimmerAC_h

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include <TimerOne.h>	// Avaiable from http://www.arduino.cc/playground/Code/Timer1

volatile int dimCounter = 0;                // Variável a ser usado como um contador
volatile boolean zero_cross = 0;  // Booleana para armazenar um "switch" para nos dizer se temos atravessado de zero
int dim = 0; 					 // Dimming level (0-255)  0 = on, 255 = 0ff
int dimRangeMax = 256;			// Dimming level (0-255)  0 = on, 255 = 0ff
int dimACpin;                  // Output to Opto Triac
int dimFreq;

void zero_cross_detect(void)
{
  zero_cross = true;		// Definir o booleano como true para dizer o nosso
							// função de dimming que um cruzamento de zero ocorreu
  digitalWrite(dimACpin, LOW);// Desliga TRIAC (e AC)
  dimCounter=0;
}

void dim_check() 
{                   
  if(zero_cross == true) 
  {              
    if(dimCounter>=((dimRangeMax-1)-dim)) 
    {                     
      digitalWrite(dimACpin, HIGH); // Ligar a luz                            
      zero_cross = false; //reset na detecção de cruzamento por zero
      dimCounter=0;  // reset contador  
    } 
    else 
    {
      dimCounter++; // incrementa o contador                    
    }                                
  }                                  
} 
 
void DimmerAC_begin(int _dimACpin,int pinZeroCross,int _dimFreq)
{
 dimACpin =_dimACpin;
 dimFreq = _dimFreq;
 
 int dimFreqStep = ((1e6/(dimFreq*2))/(dimRangeMax-1))+1;    // This is the delay-per-brightness step in microseconds.
 // Para 60 Hz deve ser 65
 // Ele é calculado com base na freqüência de seu Tensão de alimentação (50Hz ou 60Hz)
 // Eo número de etapas de brilho que você deseja.
 //
 // Perceber que existem dois zerocrossing por ciclo. isso significa
 // Cruzamento zero acontece em 120Hz para uma fonte de 60Hz ou 100Hz para uma 50Hz.
 // Para calcular dimFreqStep dividir o comprimento completo de uma meia onda da energia
 // Ciclo (em microssegundos) ao número de passos de brilho.
 // (120 Hz = 8333uS) / 256 passos de brilho = 65 uS / passo brilho

  pinMode(dimACpin, OUTPUT);       // Definir o pino como saída Triac
  attachInterrupt((pinZeroCross-2), zero_cross_detect, RISING); // Anexar uma Interrupção no Pin 2 ou 3 
  // (interromper 0 ou 1) para detecção Cruz Zero
  Timer1.initialize(dimFreqStep); // Biblioteca Inicializar TimerOne para a freq precisamos
  Timer1.attachInterrupt(dim_check, dimFreqStep);      
  // Use a Biblioteca TimerOne para anexar uma interrupção
  // Para a função que usamos para verificar para ver se ele é
  // O momento certo para disparar o triac. esta função
  // Vai agora correr todos os dimFreqStep em microssegundos.          
}
 
void DimmerAC_Dimming(int val_dimmer)
{ 
	dim = val_dimmer ;
}
#endif