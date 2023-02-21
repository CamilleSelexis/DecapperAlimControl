//Arduino Nano Every Decapper Power Control
//Sketch by Camille Aussems - Selexis SA
//Ver 1.0
//06/02/2023
//Implemented on an arduino nano every with an ENC28J60 based ethernet shield
///!\ Pin 8 of arduino connected to pin 10 of ethernet shield -> cut the feet of D10 on arduino nano every (used by TX0)
//This sketch is used to control the power supply of the decappers
//Future Improvement - Add a pull-up resistor on the SSR so that if the arduino fail, the decappers can still be operated

#include <UIPEthernet.h> // Used for Ethernet
#include <EEPROM.h>

// **** ETHERNET SETTING ****
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x78, 0xEE  };                                       
IPAddress ip(192, 168, 1, 107);                        
EthernetServer server(80); //Standard http port
long counter = 0;
long time_reset;
bool powerOn;
#define PIN_SSR   2 // 0 & 1 are TX/RX

void setup() {
  Serial.begin(115200);
  powerOn = EEPROM.read(0);
  //EEPROM.write(0,powerOn);
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();

  Serial.print("IP Address: ");
  Serial.println(Ethernet.localIP());
  pinMode(PIN_SSR,OUTPUT);
  time_reset = millis();
}

void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  EthernetClient *client_pntr = &client;  
  if(client){ //A client tries to connect 
    Serial.println("new connection");
    unsigned long time_start = millis();
    String currentLine = "";
    int counterLine = 0;
    while(client.connected()) { //Loop while the client is connected
      if(client.available()){
        //time_start = millis(); //reset time_start
        currentLine = ""; //reset currentLine
        //Read the first Line
        char c = client.read();
        while(!(c== '\n' || c == ' ' || c == '/' || c == -1)){
          currentLine += c;
          c = client.read();
        }
        counterLine++;
        if(currentLine == "home"){homePage(client_pntr); break;}
        if(currentLine == "powerOn"){powerOn = true;homePage(client_pntr);EEPROM.write(0,powerOn);Serial.println("power enabled");break;}
        if(currentLine == "powerOff"){powerOn = false;homePage(client_pntr);EEPROM.write(0,powerOn);Serial.println("Power off");break;}
        if(currentLine == "getStatus"){getStatus(client_pntr);Serial.println("Returned status");break;}
        if(currentLine == "reset"){homePage(client_pntr);software_reset();}
        //if(currentLine == "favicon.ico"){homePage(client_pntr);break;}
        if(counterLine > 4){homePage(client_pntr);break;}
        //Serial.println(currentLine);
        //homePage(client_pntr);
      }
      if(millis()-time_start> 5000){
        client.stop();
        Serial.println("Connection closed");
      }
    }//if(client.connected())
    client.stop();
    Serial.println("Connection closed");
  }//if(client)
  if(powerOn){
    digitalWrite(PIN_SSR,HIGH);
  }
  else{
    digitalWrite(PIN_SSR,LOW);
  }
  counter++;
  if(millis()-time_reset > 3600000){ //reset every hour
    software_reset();
  }
}

//Reset Arduino
void software_reset() {
  asm volatile (" jmp 0");  
}
