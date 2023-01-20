/**********************************************************************************
 *  TITLE: Blynk + Manual Switch control Relays using NodeMCU (Real time feedback + no WiFi control)
 *  Click on the following links to learn more. 
 *  YouTube Video: https://youtu.be/fRCVx6yKoYw
 *  Related Blog : https://easyelectronicsproject.com/esp32-projects/
 *  by Tech StudyCell
 *  Preferences--> Aditional boards Manager URLs : 
 *  https://dl.espressif.com/dl/package_esp32_index.json, http://arduino.esp8266.com/stable/package_esp8266com_index.json
 *  
 *  Download Board ESP8266 NodeMCU : https://github.com/esp8266/Arduino
 *  Download the libraries
 *  https://github.com/blynkkk/blynk-library/releases/download/v0.6.1/Blynk_Release_v0.6.1.zip
 **********************************************************************************/

//#define BLYNK_PRINT Serial            
#include <BlynkSimpleEsp8266.h> 

// define the GPIO connected with Relays and switches
#define RelayPin1 13  //D1


#define SwitchPin1 15  //SD3


#define wifiLed   16   //D0

#define VPIN_BUTTON_1    V1 

int toggleState_1 = 0; //Define integer to remember the toggle state for relay 1


int wifiFlag = 0;

#define AUTH "AUTH TOKEN"                 // You should get Auth Token in the Blynk App.  
#define WIFI_SSID "WIFI NAME"             //Enter Wifi Name
#define WIFI_PASS "WIFI PASSWORD"         //Enter wifi Password

BlynkTimer timer;

void relayOnOff(int relay){
  
             if(relay == 1){
              digitalWrite(RelayPin1, HIGH); // turn on relay 1
              toggleState_1 = 1;
              Serial.println("Device1 ON");
              }
             else{
              digitalWrite(RelayPin1, LOW); // turn off relay 1
              toggleState_1 = 0;
              Serial.println("Device1 OFF");
              }
             delay(100);
      
  
}

void with_internet(){
    //Manual Switch Control
    if (digitalRead(SwitchPin1) == LOW){
      delay(200);
      relayOnOff(0); 
      Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);   // Update Button Widget  
    }
    
}
void without_internet(){
    //Manual Switch Control
    if (digitalRead(SwitchPin1) == HIGH){
      delay(200);
      relayOnOff(1);      
    }else{
      delay(200);
      relayOnOff(1);
    }
}

BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(VPIN_BUTTON_1);
}

// When App button is pushed - switch the state

BLYNK_WRITE(VPIN_BUTTON_1) {
  toggleState_1 = param.asInt();
  digitalWrite(RelayPin1, toggleState_1);
}


void checkBlynkStatus() { // called every 3 seconds by SimpleTimer

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    wifiFlag = 1;
    digitalWrite(wifiLed, HIGH); //Turn off WiFi LED
  }
  if (isconnected == true) {
    wifiFlag = 0;
    digitalWrite(wifiLed, LOW); //Turn on WiFi LED
  }
}
void setup()
{
  Serial.begin(9600);

  pinMode(RelayPin1, OUTPUT);

  pinMode(wifiLed, OUTPUT);

  pinMode(SwitchPin1, INPUT);


  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, toggleState_1);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  timer.setInterval(3000L, checkBlynkStatus); // check if Blynk server is connected every 3 seconds
  Blynk.config(AUTH);
}

void loop()
{  
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi Not Connected");
  }
  else
  {
    Serial.println("WiFi Connected");
    Blynk.run();
  }

  timer.run(); // Initiates SimpleTimer
  if (wifiFlag == 0)
    with_internet();
  else
    without_internet();
}
