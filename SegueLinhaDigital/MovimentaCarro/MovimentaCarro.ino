//******************************************************************************************************************************************************************************************
int pulsosParaDist(int d) {
  return(d*ppVolta/dpVolta); 
}

//******************************************************************************************************************************************************************************************
void Frente(int vel) {  
  analogWrite(pinMotorDireitaFrente, vel-correcao);
  analogWrite(pinMotorDireitaTras, 0);
  analogWrite(pinMotorEsquerdaFrente, vel);
  analogWrite(pinMotorEsquerdaTras, 0);  
}
//******************************************************************************************************************************************************************************************
void Re(int vel) {
  analogWrite(pinMotorDireitaFrente, 0);
  analogWrite(pinMotorDireitaTras, vel - correcao);
  analogWrite(pinMotorEsquerdaFrente, 0);
  analogWrite(pinMotorEsquerdaTras, vel);  
}
//******************************************************************************************************************************************************************************************
void corrigeReDir(int vel) {
  analogWrite(pinMotorEsquerdaFrente, vel);
  analogWrite(pinMotorEsquerdaTras, 0); 
  analogWrite(pinMotorDireitaFrente, 0);
  analogWrite(pinMotorDireitaTras, vel - correcao); 
}
//******************************************************************************************************************************************************************************************
void corrigeReEsq(int vel) {
  analogWrite(pinMotorEsquerdaFrente, 0);
  analogWrite(pinMotorEsquerdaTras, vel); 
  analogWrite(pinMotorDireitaFrente, vel - correcao);
  analogWrite(pinMotorDireitaTras, 0);  
}
//******************************************************************************************************************************************************************************************
/*void ReNavega(int vel){ 
  //ré seguindo linha... a lógica aqui é que quando o sensor de um lado fica preto, corrige-se para o lado oposto. 
  //EX: sensor esquerdo de dentro fica preto, então devemos acelerar o motor direito para trás e o esquerdo para frente!
  if((digitalRead(pinSensorEsqFora)) && (digitalRead(pinSensorEsqDentro)) && (digitalRead(pinSensorCentral) == 0) && (digitalRead(pinSensorDirDentro)) && (digitalRead(pinSensorDirFora)))
  { //o sensor do centro esta seguido; os dois motores devem estar ativos.
    Re(vel);
  }
  if((digitalRead(pinSensorEsqFora)) && (digitalRead(pinSensorEsqDentro)== 0) && (digitalRead(pinSensorCentral)) && (digitalRead(pinSensorDirDentro)) && (digitalRead(pinSensorDirFora)))
  { //o sensor da esquerda de dentro está pegando, entao o motor direita tem que acelerar para trás
    corrigeReDir(vel);
  } 
  if((digitalRead(pinSensorEsqFora)) && (digitalRead(pinSensorEsqDentro)) && (digitalRead(pinSensorCentral)) && (digitalRead(pinSensorDirDentro)==0) && (digitalRead(pinSensorDirFora)))
  { //o sensor da direita de dentro está pegando, entao o motor direito tem que acelerar para trás
    corrigeReEsq(vel);
  } 
  if((digitalRead(pinSensorEsqFora)== 0)&& (digitalRead(pinSensorEsqDentro)) && (digitalRead(pinSensorCentral)) && (digitalRead(pinSensorDirDentro)) && (digitalRead(pinSensorDirFora)))
  { //o sensor da esquerda de fora está pegando, entao o motor esquerdo tem que acelerar para trás
    corrigeReDir(vel);
  }
  if((digitalRead(pinSensorEsqFora)) && (digitalRead(pinSensorEsqDentro)) && (digitalRead(pinSensorCentral)) && (digitalRead(pinSensorDirDentro)) && (digitalRead(pinSensorDirFora)== 0))
  { //o sensor da direita está pegando, entao o motor direito tem que acelerar para trás
    corrigeReEsq(vel);
  }
}   */
//******************************************************************************************************************************************************************************************
void corrigeDir(int vel) {  
  analogWrite(pinMotorDireitaFrente, 0);
  analogWrite(pinMotorDireitaTras, 0);
  analogWrite(pinMotorEsquerdaFrente, vel);
  analogWrite(pinMotorEsquerdaTras, 0);  
}
//******************************************************************************************************************************************************************************************
void corrigeEsq(int vel) {  
  analogWrite(pinMotorDireitaFrente, (vel - correcao));
  analogWrite(pinMotorDireitaTras, 0);
  analogWrite(pinMotorEsquerdaFrente, 0);
  analogWrite(pinMotorEsquerdaTras, 0);  
}
//******************************************************************************************************************************************************************************************
void corrigeDirRap(int vel) {  
  analogWrite(pinMotorDireitaFrente, 0);
  analogWrite(pinMotorDireitaTras, (vel-correcao));
  analogWrite(pinMotorEsquerdaFrente, vel);
  analogWrite(pinMotorEsquerdaTras, 0);  
}
//******************************************************************************************************************************************************************************************
void corrigeEsqRap(int vel) {  
  analogWrite(pinMotorDireitaFrente, (vel - correcao));
  analogWrite(pinMotorDireitaTras, 0);
  analogWrite(pinMotorEsquerdaFrente, 0);
  analogWrite(pinMotorEsquerdaTras, vel);  
}
//******************************************************************************************************************************************************************************************
void Parar() {  
  analogWrite(pinMotorDireitaFrente, 255);
  analogWrite(pinMotorDireitaTras, 255);
  analogWrite(pinMotorEsquerdaFrente,255);
  analogWrite(pinMotorEsquerdaTras, 255);  
}

//******************************************************************************************************************************************************************************************
int temObstaculoPerto() {  //leitura do ultrasom
  float Tempo;
  digitalWrite(trigFrente, LOW);
  delayMicroseconds(2);
  digitalWrite(trigFrente, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigFrente, LOW);
  delayMicroseconds(2);
  Tempo = pulseIn(echoFrente, HIGH);
  Dist = (Tempo/2) / 29.1;  
  return (Dist <= dMin?1:0);
}
void passosEmFrente(int p) {
    contaPulso = 0;
    Frente(velAjuste);
    while(contaPulso <= p) { delayMicroseconds(1);}
    Parar();
}



//***************************************************************************************************************************************************************************************
void direitaVolver() {
   int pulsosDesejados = pulsosParaDist(dpVoltaCarro/4);
   Serial.print("Pulsos Desejados: ");
   Serial.println(pulsosDesejados);
   delay(2000);
   contaPulso = 0;  
   analogWrite(pinMotorDireitaFrente, 0);
   analogWrite(pinMotorDireitaTras, (velGiro-correcao));
   analogWrite(pinMotorEsquerdaFrente, velGiro);
   analogWrite(pinMotorEsquerdaTras, 0);
   
   while(contaPulso < pulsosDesejados) { 
    delayMicroseconds (1);
    Serial.println(contaPulso);
    delay(200);
   } Parar();
}
//***************************************************************************************************************************************************************************************
void esquerdaVolver() {
  int pulsosDesejados = pulsosParaDist(dpVoltaCarro/4);
  contaPulso = 0;  
  
  analogWrite(pinMotorDireitaFrente, velGiro- correcao);
  analogWrite(pinMotorDireitaTras, 0);
  analogWrite(pinMotorEsquerdaFrente, 0);
  analogWrite(pinMotorEsquerdaTras, velGiro);
  
  while(contaPulso < pulsosDesejados) { 
    delayMicroseconds (1);
  } Parar();
}

//***************************************************************************************************************************************************************************************
void meiaVoltaVolver() {
  int pulsosDesejados = pulsosParaDist(dpVoltaCarro/2);
  contaPulso = 0;  
  
  analogWrite(pinMotorDireitaFrente, velGiro- correcao);
  analogWrite(pinMotorDireitaTras, 0);
  analogWrite(pinMotorEsquerdaFrente, 0);
  analogWrite(pinMotorEsquerdaTras, velGiro);
  
  while(contaPulso < pulsosDesejados) {
    delayMicroseconds (1);
  } Parar();
}  
//***************************************************************************************************************************************************************************************
/*void alinharPelaTesta(){    // esta eh para trabalhar com os dois sensores de dentro e os dois de fora 
while(!(digitalRead(pinSensorEsqDentro) && digitalRead(pinSensorDirDentro))){
  Re(velAjuste);  // marcha a Ré para tentar acertar perpendicularmente com a linha que está sendo seguida.
  while( (digitalRead(pinSensorEsqDentro) == 0) &&  (digitalRead(pinSensorEsqFora == 0)) && (digitalRead(pinSensorDirDentro) == 0)&& (digitalRead(pinSensorDirFora) == 0)){
     delayMicroseconds(1);
  }
  Parar();
  if(digitalRead(pinSensorEsqFora))   //(digitalRead(pinSensorEsqFora == 0)) && (digitalRead(pinSensorDirDentro) == 0)&& (digitalRead(pinSensorDirFora) == 0))
     {
         corrigeDirRap(velAjuste);
     }
  else if(digitalRead(pinSensorDirFora))
        {
            corrigeEsqRap(velAjuste);
        }
}   */

void alinharPelaTesta() {      // esta e para trabalhar somente com os dois sensores de dentro
  while(!(digitalRead(pinSensorEsqDentro) && digitalRead(pinSensorDirDentro))) {
    Re(velAjuste);  // marcha a Ré para tentar acertar perpendicularmente com a linha que está sendo seguida.
    while( (digitalRead(pinSensorEsqDentro) == 0) &&  (digitalRead(pinSensorDirDentro) == 0)) {
       delayMicroseconds(1);
    } Parar();
    while(digitalRead(pinSensorEsqDentro)&& (digitalRead(pinSensorDirDentro) == 0)) {
           corrigeDirRap(velAjuste);
    }
    while(digitalRead(pinSensorDirDentro)&&(digitalRead(pinSensorEsqDentro) == 0)) {
              corrigeEsqRap(velAjuste);
    }
  } Parar();
}
//***************************************************************************************************************************************************************************************
float leDistancia(char lado){
  float Tempo, dist;
  switch(lado) {
    case 'e': digitalWrite(trigEsq, LOW);
              delayMicroseconds(2);
              digitalWrite(trigEsq, HIGH);
              delayMicroseconds(10);
              digitalWrite(trigEsq, LOW);
              delayMicroseconds(2);
              Tempo = pulseIn(echoEsq, HIGH);
              dist = (Tempo/2) / 29.1;
              break;
              
    case 'd': digitalWrite(trigDir, LOW);
              delayMicroseconds(2);
              digitalWrite(trigDir, HIGH);
              delayMicroseconds(10);
              digitalWrite(trigDir, LOW);
              delayMicroseconds(2);
              Tempo = pulseIn(echoDir, HIGH);
              dist = (Tempo/2) / 29.1;
              break;
              
   case 'f':  digitalWrite(trigFrente, LOW);
              delayMicroseconds(2);
              digitalWrite(trigFrente, HIGH);
              delayMicroseconds(10);
              digitalWrite(trigFrente, LOW);
              delayMicroseconds(2);
              Tempo = pulseIn(echoFrente, HIGH);
              dist = (Tempo/2) / 29.1;
              break;
  } return(dist);             
}

//***************************************************************************************************************************************************************************************
//***************************************************************************************************************************************************************************************
void avancarDesvio() {  
  unsigned long Tempo1,Tempo2,Tempo;
  int pulsosDist;  
  Dist = distEsperada;
  pulsosDist = pulsosParaDist(Dist);
  contaPulso = 0;
  Tempo1 = millis();   // tempo de inicio da ida para a direita.
  Frente(velAjuste); 
  while(contaPulso < pulsosDist) {
    delayMicroseconds(1);
  } Parar();
  Tempo2 = millis();
  Tempo = Tempo2 - Tempo1;
  esquerdaVolver();
  contaPulso = 0;
  Frente(velAjuste); 
  while(contaPulso < pulsosDist) {
            delayMicroseconds(1);
  } Parar();
  esquerdaVolver();
  contaPulso = 0;
  Frente(velAjuste); 
  while((contaPulso < pulsosDist)&&((digitalRead(pinSensorEsqDentro) == 0) && (digitalRead(pinSensorDirDentro)==0)&&(digitalRead(pinSensorEsqFora) == 0) && (digitalRead(pinSensorDirFora)==0))) {
            delayMicroseconds(1);
  } Parar();
}  /*
//***************************************************************************************************************************************************************************************
void Ultrapassar(){
  float Tempo;
  float distLimSup = distLateral + distFolga;
  Dist = leDistancia('e');
  if(Dist > distLimSup){   // ainda não ve o obstáculo do lado
        Frente(velAjuste);
        while(Dist > distLimSup){
            Dist = leDistancia('e');
        }
        Parar();    // sensor começa a ver o obstaculo.
        }
        contaPulso = 0;   // zera o contaPulso para ver o tamanho da frente do obstáculo
        Frente(velAjuste);
        while(Dist <= distLimSup){
            Dist = leDistancia('e');
        } 
        Parar();
        contaPulso = 0; // zerando para calcular quanto avanço mais por segurança  
        Frente(velAjuste);
        while(contaPulso < pulsosParaDist(distFolga)){
            delayMicroseconds(1);
        }
        Parar();
    }
    
//***************************************************************************************************************************************************************************************
void recuarDesvio(){ 
while(!(digitalRead(pinSensorEsqDentro) && digitalRead(pinSensorDirDentro))){
  Frente(velAjuste);
  while((digitalRead(pinSensorEsqDentro)==0) && (digitalRead(pinSensorDirDentro)==0))
 {delayMicroseconds(1);}
  Parar(); 
   while(digitalRead(pinSensorEsqDentro)&& (digitalRead(pinSensorDirDentro) == 0))  
     {
         corrigeEsqRap(velAjuste);
     }
  while(digitalRead(pinSensorDirDentro)&&(digitalRead(pinSensorEsqDentro) == 0))
        {
            corrigeDirRap(velAjuste);
        }
 }
Parar();
} 
//***************************************************************************************************************************************************************************************
void esquerdaVolver(){
  float ZAtual,ZFinal,ZVolta;
  MedeAnguloVertical.update();
  ZAtual = MedeAnguloVertical.getAngleZ();
  ZFinal = ZAtual+(90-ajusteDirEsq);
  Serial.println(" ");
  Serial.print("\tZAtual : ");
  Serial.println(ZAtual);
  Serial.print("\tZFinal : ");
  Serial.println(ZFinal);
  ZVolta = MedeAnguloVertical.getAngleZ(); 
   corrigeEsqRap(velGiro);
  while(ZVolta <= ZFinal){ 
            MedeAnguloVertical.update();        
            ZVolta = MedeAnguloVertical.getAngleZ();
            Serial.println(ZVolta);
         }
  Parar();
 }
//***************************************************************************************************************************************************************************************
void direitaVolver(){
  float ZAtual,ZFinal;
  MedeAnguloVertical.update();
  ZAtual = MedeAnguloVertical.getAngleZ();
  ZFinal = ZAtual-(90-ajusteDirEsq);
  Serial.print("\tZFinal : ");
  Serial.println(ZFinal);
  corrigeDirRap(velGiro);
  while(MedeAnguloVertical.getAngleZ() >= ZFinal){
       MedeAnguloVertical.update();
       }
  Parar();
 }
//***************************************************************************************************************************************************************************************
void meiaVoltaVolver(){
  float ZAtual,ZFinal;
  MedeAnguloVertical.update();
  ZAtual = MedeAnguloVertical.getAngleZ();
  ZFinal = ZAtual+180+ajusteMeiaVolta;
  Serial.print("\tZFinal : ");
  Serial.println(ZFinal);
  corrigeEsqRap(velGiro);
  while(MedeAnguloVertical.getAngleZ() <= ZFinal){MedeAnguloVertical.update();
  }
  Parar();
 }
