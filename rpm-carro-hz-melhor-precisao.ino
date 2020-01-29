#include <FreqPeriodCounter.h>
//para baixar a lib acima -> http://www.avdweb.nl/arduino/hardware-interfacing/frequency-period-counter.html
//Mas a mesma ja existe para importar pela propria IDE do arduino.
  
const byte counterPin = 3; //Pino D3~ que ira ler o Sinal Pulso (+12v) vindo da centralina do carro.
const byte counterInterrupt = 1; 
int rpm = 0;
int ultimoRPM = 0;
int contador = 0;
float soma = 0.00;


FreqPeriodCounter counter(counterPin, micros);
  
void setup(void)
{ 
  Serial.begin(115200);
  attachInterrupt(counterInterrupt, counterISR, CHANGE);  
  
}
  
void loop(void)
{ 
  int period;
  if(counter.ready()) period = counter.period;
  //Cada 3Hz equivale a 100RPM
  //Entao a regra de tres fica RPM = ((ValorHZ * 100RPM)/3Hz)
  
  //Fazer uma media com 5 contagens.
  if (contador < 5) 
  {
      //Caso a frequencia ultrapasse 255Hz + ou - 8500RPM Zerar as medias e contador e mostrar o ultimoRPM mostrado.
      if (counter.hertz() > 255) {
        contador = 0;
        soma = 0;
        Serial.println(ultimoRPM);
      } else {
        soma  = soma +  counter.hertz();
        contador = contador + 1;
      }
  
  } else  {
  //a soma abaixo esta dividida por 5 devido a usar a media de 5 contagens cfe. codigo acima (contador < 5), caso altere o numero de medias acima alterar aqui tambem.
  rpm = (((soma/5) * 100) / 3);
  ultimoRPM = rpm;
  Serial.println(rpm);
  contador = 0;
  soma = 0;
  delay(10);
  }
}
  
void counterISR()
{ 
  counter.poll();
}
