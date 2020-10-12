int temVerde(char lado){

   int i,conte,contd;

   conte = contd = 0;
delay(2000);
for(i = 1; i <= 10; i++){
    contd+= dirTemVerde();
    conte+= esqTemVerde();
}

 if(lado == 'd'){
 if(contd >=3){
    Serial.println("VERDE NA DIREITA");
    return(1);
 }
  else return(0);

 }
if(lado == 'e'){   
if(conte>=1) {Serial.println("VERDE NA ESQUQERDA");
               return(1);
  } 
  else return(0);
  
}
}






//*****************************************************************************************************************************************************************************
int esqTemVerde(){
  int Red, Blue, Green;

digitalWrite(s2E, LOW); digitalWrite(s3E, LOW); //count OUT, pRed, RED

Red = pulseIn(outE, digitalRead(outE) == HIGH ? LOW : HIGH);

digitalWrite(s3E, HIGH); //count OUT, pBLUE, BLUE

Blue = pulseIn(outE, digitalRead(outE) == HIGH ? LOW : HIGH);

digitalWrite(s2E, HIGH); //count OUT, pGreen, GREEN

Green = pulseIn(outE, digitalRead(outE) == HIGH ? LOW : HIGH);
 
return ((Green < Red) && (Green < Blue))?1:0;
}
//*****************************************************************************************************************************************************************************
int dirTemVerde(){
  int Red, Blue, Green;

digitalWrite(s2D, LOW); digitalWrite(s3D, LOW); //count OUT, pRed, RED

Red = pulseIn(outD, digitalRead(outD) == HIGH ? LOW : HIGH);

digitalWrite(s3D, HIGH); //count OUT, pBLUE, BLUE

Blue = pulseIn(outD, digitalRead(outD) == HIGH ? LOW : HIGH);

digitalWrite(s2D, HIGH); //count OUT, pGreen, GREEN

Green = pulseIn(outD, digitalRead(outD) == HIGH ? LOW : HIGH);

return (Green < Red && Green < Blue)?1:0;
}  
