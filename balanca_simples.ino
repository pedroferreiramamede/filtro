#include <HX711.h> 
#include <Wire.h>

#define RELE 4
#define PESO_CHEIO 14
#define PESO_SEM_AGUA 8
#define ANTI_TRANSBORDER 200 // 30 segundos, considerando 300 milisec no loop encher_filtro


const int PINO_DT = 3;
const int PINO_SCK = 2;
const int TEMPO_ESPERA = 1000;
HX711 escala; 
float FATOR_CALIBRACAO = -87500;
float peso = 0;

void encher_filtro(){
    float peso_check = escala.get_units();
    int anti_transbordador = 0;
     
    while((peso_check < PESO_CHEIO) && (peso_check > PESO_SEM_AGUA)){
        Serial.println("Enchendo...");
        digitalWrite(RELE, LOW);//aciona rele
        peso_check = escala.get_units();
        Serial.print("peso_check: "); Serial.print(peso_check, 1); Serial.println(" kg");
        delay(300);
        anti_transbordador ++;
        if(anti_transbordador == ANTI_TRANSBORDER){
           Serial.println("ANTI TRANSBORDAMENTO ATIVADO");
        }
    }    
    digitalWrite(RELE, HIGH);
}
 
void setup ()
{
  pinMode(RELE, OUTPUT);
  digitalWrite(RELE, HIGH);
  Serial.begin(9600);
  escala.begin (PINO_DT, PINO_SCK); 
  //escala.tare(); 
  escala.set_scale(FATOR_CALIBRACAO);
  
}

void loop ()
{
  if (escala.is_ready())
  {
      peso = escala.get_units();
      Serial.print("Peso: "); Serial.print(peso, 1); Serial.println(" kg");

      if((peso < (PESO_CHEIO - 0.3)) && (peso > PESO_SEM_AGUA)){
          Serial.println("INICIANDO ENCHIMENTO");     
          encher_filtro();
      }
      
  } else {
    Serial.print("Aguarde, sensor não preparado...");
  }
  delay(TEMPO_ESPERA); //intervalo de espera para leitura
}
