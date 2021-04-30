// Define Pins
byte BLUE=3;
byte GREEN=4;
byte RED=5;

// define variables
int pinLed [] = {RED,BLUE,GREEN};
int buttonPin = 6;
uint8_t pinValue []  = {255,255,255};
int delayTime=100; // fading time between colors
int delayEndTime=5000; // ending time when a color is full
int mode= 0;
int nbEclairage =10;
int8_t hbdelta = 8;
//le mode d'eclairement
//0 parcours du rouge vers bleu vers le vert puis au rouge en mélangeant les teintes
//1 rouge s'allume puis s'eteinds puis bleu s'allume et s'eteinds puis vert s'allume puis s'eteinds
void setup()
{
  mode = 0;
  pinMode(buttonPin, INPUT);  
for (int i = 0; i < 3; i++) {
  pinMode(pinLed[i], OUTPUT);  
  analogWrite(pinLed[i], pinValue[i]);
  
}
delay(delayEndTime);
}

boolean recevoirCommande(int attente)
{
  unsigned long finAttente= millis()+attente;
  while(millis() < finAttente)
  {
  if (digitalRead(buttonPin) == HIGH) {
    eteindre();
    delay(1000);
    return true;
  }
  }
  
  return false;
}

void melangerCouleurs()
{

  for (int p=0;p<3; p++){
    pinValue[p]=255;
    pinValue[(p+1)%3]=0;
    for(int i = 0; i < 255; i++){
      analogWrite(pinLed[p], pinValue[p]);
      analogWrite(pinLed[(p+1)%3], pinValue[(p+1)%3]);      
      pinValue[p]--;
      pinValue[(p+1)%3]++;
      if (recevoirCommande(delayTime))
      {
        mode = 1;
        return;
      }
     }
    analogWrite(pinLed[p], pinValue[p]);
    analogWrite(pinLed[(p+1)%3], pinValue[(p+1)%3]);
    if (recevoirCommande(delayEndTime))
    {
       mode = 1;
       return;
    }
  }
}

void separerCouleur()
{
  for (int p=0;p<3 && mode == 1 ; p++ ){
    battreCoeur(p);
  }
}

void battreCoeur(int couleurPin)
{
  // un cycle = allumage progressif puis extinction progressive
  int cycle =0;
  while ( cycle < nbEclairage )
  {
    if (pinValue[couleurPin] > 192)
    {
      pinValue[couleurPin]=192;
      hbdelta = -hbdelta;
    }
    if (pinValue[couleurPin] < 32)
    {
      pinValue[couleurPin]=32;
      hbdelta = -hbdelta;
      cycle++;
    }
    pinValue[couleurPin] += hbdelta;
    analogWrite(pinLed[couleurPin], pinValue[couleurPin]);
    if (recevoirCommande(delayTime))
    {
      mode = 0;
      return;
    }        
  }
  pinValue[couleurPin]=0;
  analogWrite(pinLed[couleurPin], pinValue[couleurPin]);
}
// main loop
void loop()
{
//on démarre éteinds
  eteindre();
  switch(mode)
        {
          case 0:
            melangerCouleurs();
          break;
          case 1:
            separerCouleur();
          break;
        }
}

void eteindre()
{
    for (int p=0;p<3 ; p +=1 ){
      pinValue[p]=0;
      analogWrite(pinLed[p], pinValue[p]);
  }
}
