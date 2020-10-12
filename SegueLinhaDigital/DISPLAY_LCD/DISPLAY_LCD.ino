//PARA USAR AS FUNÇOES, BASTA CHAMAR mostraMensagem(M), onde M é uma string a ser mostrada.
 

int strTamanho(char*m) {
  int i = 0;
  while(*(m+i) != '\0')
     i++;
  return(i);
}
 
void mostraMensagem(char* M)
{  
  int p;
  int t = strTamanho(M);
  MENSAGEIRO.clear();
  if(t <= 16) {
    p = (NCol - t)/2;
    MENSAGEIRO.setCursor(p,0);
    MENSAGEIRO.print(M);
    delay(500);
  } else {
    for (p = NCol - 1; p > 0; p--) { // string entrando no display
      MENSAGEIRO.setCursor(p,0);
      MENSAGEIRO.print(M);
      delay(400);
     }
     
    for(p = 0; p < t; p++) { // string saindo do display
      MENSAGEIRO.clear();
      MENSAGEIRO.setCursor(0,0);
      MENSAGEIRO.print(M+p);
      delay(400);
     } MENSAGEIRO.clear();   
  }
}
