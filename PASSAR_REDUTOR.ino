/*
//***************************************************************************************************************************************************************************************
int estaParadoNoRedutor(){
     int pulsoAnt,pulsosEmitidos;
     contaPulso = 0;
     pulsoAnt = contaPulso;
     delay(100);
     pulsosEmitidos = contaPulso - pulsoAnt;
     return(pulsosEmitidos < qtPulsosPrevista);   /// testar o valor real esperado
   }




   
void tentarSairDoRedutor(){
  int aumento = 2;
  do{
    Parar();
    contaPulso = 0;
    ReNavega(velAjuste);
    while(contaPulso < pulsosParaDist(distFugaRedutor + aumento)){
       delayMicroseconds(1);
    }
    Parar();
    contaPulso = 0;
    Frente(velTurbo);
    aumento+=2;
    delay(100);
}while((estaParadoNoRedutor())&& (aumento <= 10));
}
*/
