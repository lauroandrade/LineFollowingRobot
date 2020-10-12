 
 int estouNaRampa(){
  float YAtual,AngRampa;
  MedeAnguloVertical.update();
  YAtual = MedeAnguloVertical.getAngleY();
  AngRampa = abs(YAtual - YBase);
  Serial.print("\tangleY : ");
  Serial.println(AngRampa);
  return(AngRampa >= RampaMinima);
 }

 int andarDeCima(){
  float YAtual,AngCima;
  MedeAnguloVertical.update();
  YAtual = MedeAnguloVertical.getAngleY();
  AngCima = abs(YAtual - YBase);
  Serial.print("\tangleY : ");
  Serial.println(AngCima);
  return(AngCima <= (RampaMinima/2));
 }
