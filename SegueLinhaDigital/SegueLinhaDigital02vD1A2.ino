/******* DEFINIÇÕES GERAIS *******/
//  ESTA VERSÃO ESTÁ PREPARADA PARA O SEGUIDOR COM 5 SENSORES INDIVIDUAIS ONDE SE LÊ 5V PARA PRETO E 0 VOLTS PARA BRANCO.
// BIBLIOTECAS
#include <Math.h>
#include <LiquidCrystal.h>                //Inclui biblioteca do LCD
#include <Wire.h>                         // para utilizar no giroscópio.
#include <Servo.h>
#include <MPU6050_tockn.h>
#define ajusteDirEsq 5
#define ajusteMeiaVolta 19
// LCD DESAFIO
#define NCol 16
#define NLin 2
#define QtMens 2   // quantidade de mensagens do display.

// SENSORES SEGUE LINHA
#define pinSensorEsqFora 34
#define pinSensorEsqDentro 28
#define pinSensorCentral 22
#define pinSensorDirDentro 8
#define pinSensorDirFora 7

// ENCODER RODA E DEFINIÇÕES
#define encoderEsq 2  // interrupt 0
#define encoderDir 3
#define dCarro 20   // cm    diametro da volta do carro sobre seu eixo
#define dpVoltaCarro 62.8 // cm  distancia percorrida por volta sobre o eixo do carro | 2 * pi * raio do centro do carro ao meio da roda
#define dRoda 6.5  // cm     diametro da roda
#define ppVolta 238         // pulsos por volta
#define dpVolta  20.41    // cm  .. distancia percorrida por volta// interrupt 1
#define qtPulsosPrevista  238  // quantidade de pulsos que se espera sejam dados em um décimo de segundo na velocidade velTur. Usada para verificar se o carro está parado num redutor
// se emitiu menos que esse nr de passos, é porque ou está parado no redutor ou está patinando no redutor. De qq maneira tem que ser acertado.

// SENSOR INDUTIVO
#define sensorIndutivo 50

// SENSOR ULTRASOM
// FRENTE
#define trigFrente 52
#define echoFrente 53
// DIREITA
#define trigDir 35
#define echoDir 36
// ESQUERDA
#define trigEsq 38
#define echoEsq 39
// DEFINIÇÃO DE VALORES AUXILIARES
#define distParada 12
#define distSeguranca 5 // eh o que pode variar a distancia Esperada enquanto avancaDesvio
#define distEsperada (distParada + distSeguranca + 5)  // esperando que ao fazer direita volver, a distancia aumente em "distSeguranca"  + 5 cm  os 5 cm devem ser conferidos no direita volver.
#define distLateral 10  // espera-se passar a 10 cm pelo lado direito do obstaculo

#define distFolga 5    // eh o tanto que se avança apos ultrapassar o obstaculo, para folga na hora de voltar à linha. (direita volver final)

// SERVO MOTOR
#define Servo01 12
#define Servo02 13
//#define Servo03


// SENSOR DE COR
// ESQUERDO
#define s0E 23
#define s1E 26
#define s2E 25
#define s3E 24
#define outE 27
// DIREITO
#define s0D 49
#define s1D 51
#define s2D 43
#define s3D 45
#define outD 33
// DEFINIÇÃO DOS VALORES AUXILIARES
#define dMin 20

// MOTOR
// DIREITA
#define pinMotorDireitaFrente 10
#define pinMotorDireitaTras 11
// ESQUERDA
#define pinMotorEsquerdaFrente 5
#define pinMotorEsquerdaTras 6
// DEFINIÇÕES DOS VALORES AUXILIARES
#define correcao 20 // para corrigir diferencas de velocidade entre os dois motores
#define velTur 150
#define velGiro 130
#define velAjuste 125
#define velTurbo 255 
#define distFugaRedutor 12

// GIROSCOPIO

#define RampaMinima 10 //valor mínimo que se espera para a rampa
//*******************************************************************************************************************************************************************

// VARIAVEIS
MPU6050 MedeAnguloVertical(Wire);
LiquidCrystal MENSAGEIRO(41, 42, 46, 47, 48, 49); // (RS,EN,D4,D5,D6,D7)  O RW DEVE SER COLOCADO EM GND.
Servo AbreFechaGarra;
Servo LevantaAbaixaGarra;
char*Mensagens[QtMens] = {"Desafio Superado!!", "Prova Finalizada"};
float largObstaculo;
int cont = 0;
float Dist;
int contaPulso;
int emBaixo = 1;  // em Baixo passa a se zero (0), quando estiver na rampa ou em cima.
int emCima = 0;   // quando subir totalmente, emCima passa a ser 1
float YBase;
int ilcd = 0; // soh para mostrar a mensagem inicial.
float distaEsq, distaDir,distaFrente;
//********************************************************CÓDIGO DO SETUP  *****************************************************************************************************************
//***************************************************************************************************************************************************************************************
void incremPulso() {
  contaPulso++;
}
//**********************************************************************************************************************SETUP *********************************************************
void setup() {
  Serial.begin(9600);
  // INICIALIZANDO TUDO QUE PRECISA PARA O GIROSCÓPIO
     Wire.begin();
     MedeAnguloVertical.begin();
     MedeAnguloVertical.calcGyroOffsets(true);
     MedeAnguloVertical.update();
     YBase = MedeAnguloVertical.getAngleY();

    /* DETALHES DO  DISPLAY
      MENSAGEIRO.begin(NCol, NLin);                                inicio do LCD.
      MENSAGEIRO.setCursor(0,0);
      MENSAGEIRO.print("  EQUIPO UAISO  ");
      MENSAGEIRO.setCursor(0,1);
      MENSAGEIRO.print(" ABRAZA A TODAS ");    */

  // SENSOR SEGUE LINHA
  pinMode(pinSensorDirDentro, INPUT);
  pinMode(pinSensorDirFora, INPUT);
  pinMode(pinSensorEsqDentro, INPUT);
  pinMode(pinSensorEsqFora, INPUT);
  pinMode(pinSensorCentral, INPUT);

  // ULTRASOM
  // FRENTE
  pinMode(trigFrente, OUTPUT);
  pinMode(echoFrente, INPUT);
  // DIREITO
  pinMode(trigDir, OUTPUT);
  pinMode(echoDir, INPUT);
  // ESQUERDO
  pinMode(trigEsq, OUTPUT);
  pinMode(echoEsq, INPUT);

  // ENCODER
  attachInterrupt(0, incremPulso, FALLING); // PARA CONTAR OS PULSOS DO ENCODER ; interrupt zero - 0

  // SENSOR INDUTIVO DE IDENTIFICAÇAO DE NAVE RESGATAVEL
  pinMode(sensorIndutivo, INPUT);

  // SENSOR DE COR
  // ESQUERDO
  pinMode(s0E, OUTPUT);
  pinMode(s1E, OUTPUT);
  pinMode(s2E, OUTPUT);
  pinMode(s3E, OUTPUT);
  pinMode(outE, INPUT);
  digitalWrite(s0E, LOW); digitalWrite(s1E, HIGH);  // 2% freq saída
  // SENSOR DE COR
  // DIREITO
  pinMode(s0D, OUTPUT);
  pinMode(s1D, OUTPUT);
  pinMode(s2D, OUTPUT);
  pinMode(s3D, OUTPUT);
  pinMode(outD, INPUT);
  digitalWrite(s0D, LOW); digitalWrite(s1D, HIGH);  // 2% freq saída

  //**********************************************************************************************************************************************************************************
  //  INICIALIZAÇÕES DOS SERVOS MOTORES:
  /* AbreFechaGarra.attach(Servo01);
    LevantaAbaixaGarra.attach(Servo02);
    pinMode(Servo02,OUTPUT);
    digitalWrite(Servo02,LOW);  */
}

//***********************************************************CODIGO PRINCIPAL DO LOOP  ***************************************************************************************************
void loop() {
  /* delay(2000);
    mostraMensagem(Mensagens[0]);
    delay(2000);
    mostraMensagem(Mensagens[1]);   */
  int contaPulsoA, contaPulsoN;  //variaveis para verificar se, apesar de mandar andar, está preso no redutor ou outro lugar.
  if (emBaixo) { // AINDA NÃO ENTREI NA RAMPA
    if (digitalRead(pinSensorCentral)) {
      if ((digitalRead(pinSensorEsqDentro == 0)) && (digitalRead(pinSensorDirDentro) == 0))
      { //SOMENTE O SENSOR CENTRAL ESTÁ SEGUINDO LINHA
        Frente(velTur);
        if (temObstaculoPerto()) {
          Parar();
          direitaVolver();
          alinharPelaTesta();
          avancarDesvio();
        /*  esquerdaVolver();
          delay(10);
          Ultrapassar();    // dentro de ultrapassar jah anda um pouco mais após ultrapassar para não correr risco de bater no obstaculo ao fazer direitaVolver para voltar à linha.
          delay(10);
          esquerdaVolver();
          delay(10);
          recuarDesvio();  // dentro do recuarDesvio jah faço o ajustarFrente de deixa de ser necessário.
          delay(10);   */
          direitaVolver();
          Frente(velTur);
        }
        /*  if(estouNaRampa()){
            emBaixo = 0;   //****************************  PASSEI PARA A RAMPA.  ********************************
            Frente(velTurbo);
          }*/
        /*  if(estaParadoNoRedutor())
            tentarSairDoRedutor();     */
     }
      if (digitalRead(pinSensorEsqDentro) && (digitalRead(pinSensorDirDentro) == 0)) { // Cruzamento Frente ou Esquerda
        Parar();
        if (temVerde('e')){
          Serial.println("cruzamento detectado");
          do {
            corrigeEsqRap(velTur);  //esquerdaVolver();
          }while (!digitalRead(pinSensorCentral));
        }
        Frente(velTur);
      }
      if ((digitalRead(pinSensorEsqDentro) == 0) && digitalRead(pinSensorDirDentro)) { // Cruzamento Frente ou Direita
        Parar();
        if (temVerde('d')){
          Serial.println("cruzamento detectado");
          do {
            corrigeDirRap(velTur); //direitaVolver();      // TRABALHANDO COM DO WHILE.
          }while (!digitalRead(pinSensorCentral));
        }
        Frente(velTur);
      }
      if (digitalRead(pinSensorEsqDentro) && digitalRead(pinSensorDirDentro)) { // Cruzamento Frente, esquerda, direita ou retorno
        Parar();
        if (temVerde('e')) {
          Serial.println("cruzamento detectado");
          if (temVerde('d'))
            meiaVoltaVolver();
          else {
            corrigeEsqRap(velTur); //esquerdaVolver();
            while (!digitalRead(pinSensorCentral)) {
              delayMicroseconds(1);
            }
          }
        }
        else if (temVerde('d')) {
          Serial.println("cruzamento detectado");
          corrigeDirRap(velTur); //direitaVolver();
          while (!digitalRead(pinSensorCentral)) {
            delayMicroseconds(1);
          }
        }
        Frente(velTur);        // caso não haja verde de nenhum lado e ao final dos outros casos, sempre se vai para frente.
      }
    }
    else { //   Sensor Central fora da linha  :   ou tenho que corrigir para curva ou cruzamento  esquerda direita ou meia volta ou impossivel

      if (digitalRead(pinSensorEsqDentro) && (digitalRead(pinSensorDirDentro) == 0)) {
        corrigeEsqRap(velTur);
      }

      if ((digitalRead(pinSensorEsqDentro) == 0) && digitalRead(pinSensorDirDentro)) {
        corrigeDirRap(velTur);
      }

      if (digitalRead(pinSensorEsqDentro) && digitalRead(pinSensorDirDentro)) { // Cruzamento esquerda, direita, retorno ou impossivel
        Parar();
        delay(3000);
        if (temVerde('e')) {
          Serial.println("cruzamento detectado");
          if (temVerde('d'))
            meiaVoltaVolver();
          else {
            corrigeEsqRap(velTur); //esquerdaVolver();
            while (!digitalRead(pinSensorCentral)) {
              delayMicroseconds(1);
            }
          }
        }
        else if (temVerde('d')) {
          Serial.println("cruzamento detectado");
          corrigeDirRap(velTur); //direitaVolver();
          while (!digitalRead(pinSensorCentral)) {
            delayMicroseconds(1);
          }
        }
        Frente(velTur);        // caso não haja verde de nenhum lado e ao final dos outros casos, sempre se vai para frente.
      }
    }
  }

  else { //  está na rampa ou em cima
    if (emCima) { // CHEGOU NA PLATAFORMA DE RESGATE.
                   Parar();
                   distaFrente = leDistancia('f');
                   distaEsq = leDistancia('e');
                   distaDir = leDistancia('d');
                   if(distaEsq > distaDir){
                        passosEmFrente(pulsosParaDist((distaFrente-25)/2));
                        esquerdaVolver();
                        passosEmFrente(pulsosParaDist((distaEsq-22)/2));
                        Parar();
                   }
                   else{
                        passosEmFrente(pulsosParaDist((distaFrente-25)/2));
                        direitaVolver();
                        passosEmFrente(pulsosParaDist((distaEsq-22)/2));
                        Parar();
                   }
           }
                         
    else {     // ESTAH NA RAMPA
      while (!andarDeCima()){ 
           if ((digitalRead(pinSensorEsqFora) == 0) && (digitalRead(pinSensorEsqDentro == 0)) && (digitalRead(pinSensorDirDentro) == 0) && (digitalRead(pinSensorDirFora) == 0))
           {
          Frente(velTurbo);
        }
      if ((digitalRead(pinSensorEsqFora) == 0) && digitalRead(pinSensorEsqDentro) && (digitalRead(pinSensorCentral) == 0) && (digitalRead(pinSensorDirDentro) == 0) && (digitalRead(pinSensorDirFora) == 0))
      {
        corrigeEsqRap(velTurbo);
      }
      if ((digitalRead(pinSensorEsqFora) == 0) && (digitalRead(pinSensorEsqDentro) == 0) && (digitalRead(pinSensorCentral) == 0) && digitalRead(pinSensorDirDentro) && (digitalRead(pinSensorDirFora) == 0))
      {
        corrigeDirRap(velTurbo);
      }

      if (digitalRead(pinSensorEsqFora) && (digitalRead(pinSensorEsqDentro) == 0) && (digitalRead(pinSensorCentral) == 0) && (digitalRead(pinSensorDirDentro) == 0) && (digitalRead(pinSensorDirFora) == 0))
      {
        corrigeEsqRap(velTurbo);
      }
      if ((digitalRead(pinSensorEsqFora) == 0) && (digitalRead(pinSensorEsqDentro) == 0) && (digitalRead(pinSensorCentral) == 0) && (digitalRead(pinSensorDirDentro) == 0) && digitalRead(pinSensorDirFora))
      {
        corrigeDirRap(velTurbo);
      }
      /* while(estaParadoNoRedutor())
                      tentarSairDoRedutor();   */
       }
    }
  }
}  
// FINAL DO LOOP  (PROGRAMA)
