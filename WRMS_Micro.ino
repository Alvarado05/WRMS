// Test visual
// /*
// Router Monitoring System
// Status: Alpha
// By: Carlos Alvarado, Eliud Perez
// */

// include the library code:
#include <LiquidCrystal.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN   5     // Configurable, see typical pin layout above
#define SS_PIN    53   // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
MFRC522::MIFARE_Key key;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
int buzzer = 6; //Pin for the buzzer
int button1 = 22;
int button2 = 23;
int buttonSt1, buttonSt2;

String wifiPass = "microProject";
unsigned long AuthUserSession = 0;

void setup() {
  pinMode(2, OUTPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  Serial.begin(9600);
  Serial.println("Serial Ready");
  Serial.println();
  Serial1.begin(9600);
  Serial2.begin(115200);
  digitalWrite(2, LOW);

// Buttons
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

// RFID
  SPI.begin();         // Init SPI bus
  mfrc522.PCD_Init();  // Init MFRC522 card
  
// initialiaze buzzer pin as output
  pinMode(buzzer,OUTPUT);
}

void loop() {
  // set the cursor to column 0, line 0
  lcd.setCursor(0, 0);
  lcd.print("RMS on Standby");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  if(AuthUserSession > millis()){
    lcd.setCursor(0, 1);
    lcd.print("Access Granted");
  }

  checkUser();
  if(Serial1.available()){
    int input = Serial1.parseInt();
  
    if(input==1){
      checkInternet();
    }
    else if(input==2){
      rebootRouter();
    }
    else if(input==3){
      showPassSerial();
    }
    else if(input==4){
      checkLatency();
    }
    else if(input==5){
      changePass();
    }
    else if(input==4020){
      lightShow();
    }
  }
  else if(checkButtons()==1){
    if(buttonSt1==1 && AuthUserSession > millis()){
      checkInternet();
    }
    else if(buttonSt1==2 && AuthUserSession > millis()){
      checkLatency();
    }
    else if(buttonSt2==1 && AuthUserSession > millis()){
      showPassLCD();
    }
    else if(buttonSt2==2 && AuthUserSession > millis()){
      rebootRouter();
    }
  }
}

int checkButtons(){
  unsigned long pressedTime  = 0;
  unsigned long releasedTime = 0;
  const int SHORT_PRESS_TIME = 3000; // 500 milliseconds
  int currentState;     // the current reading from the input pin
  int lastState = HIGH;  // the previous state from the input pin

  if(digitalRead(button1) == LOW){
    currentState = digitalRead(button1);
    
    while(digitalRead(button1) == LOW){
      if(lastState == HIGH && currentState == LOW)        // button is pressed
        pressedTime = millis();
      
      lastState = currentState;
    }
    releasedTime = millis();
    long pressDuration = releasedTime - pressedTime;
    if( pressDuration < SHORT_PRESS_TIME ){
      buttonSt1 = 1;
      return 1;
    }
    else{
      buttonSt1 = 2; 
      return 1;
    } 
  }
  else if(digitalRead(button2) == LOW){
    currentState = digitalRead(button2);
    
    while(digitalRead(button2) == LOW){
      if(lastState == HIGH && currentState == LOW)        // button is pressed
        pressedTime = millis();
      
      lastState = currentState;
    }
    releasedTime = millis();
    long pressDuration = releasedTime - pressedTime;
    if( pressDuration < SHORT_PRESS_TIME ){
      buttonSt2 = 1;
      return 1;
    }
    else{
      buttonSt2 = 2;
      return 1;
    }  
  }
  else{
    buttonSt1 = 0;
    buttonSt2 = 0;
    return 0;
  }
}


void rebootRouter(){
  lcd.clear();
  lcd.print("Rebooting...");
  Serial.print("Rebooting Router...");
  digitalWrite(2, HIGH);
  delay(5000);
  digitalWrite(2, LOW);
  lcd.print("Done");
  Serial.println("Done");
  delay(2000);
  lcd.clear();
}

void showPassLCD(){
  lcd.clear();
  lcd.print("Wifi Password:");
  lcd.setCursor(0, 1);
  lcd.print(wifiPass);
  delay(5000);
  lcd.clear();
}

void showPassSerial(){
  Serial.println("The Wifi Password is:");
  Serial.println(wifiPass);
}

void buzz(){
  tone(buzzer, 2000); // Send 1KHz sound signal...
  delay(400);        // ...for .4 sec
  noTone(buzzer);     // Stop sound...
//  delay(1000);        // ...for 1sec
}

void checkUser(){
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
    delay(50);
    return;
  }
  if(mfrc522.uid.uidByte[0] == 167 &&
     mfrc522.uid.uidByte[1] == 35 &&
     mfrc522.uid.uidByte[2] == 156 &&
     mfrc522.uid.uidByte[3] == 181){
  AuthUserSession = millis() + 60000;
  Serial.println("User Authenticated");
  buzz();
  }
  mfrc522.PICC_HaltA();
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
    return;
  }
  return;
}

void changePass(){
  
}

int connectInternet()
{
  String response, string_buffer;
  Serial2.println("AT+CIPSTART=\"TCP\",\"google.com\",80");    //connect module to googles server
  response = Serial2.readString();
  delay(100);  
  Serial2.println("AT+CIPCLOSE");   //close connection
  string_buffer = Serial2.readString();
  delay(50);
  
  if(response.indexOf("OK") > 0)
  {
    return 1;
  }
    else
  {
    return 0;
  }
}

void checkInternet(){
  Serial.print("Checking for Internet...");
  lcd.clear();
  lcd.print("Cheking Wifi...");
  if(connectInternet() == 1){
    Serial.println("Internet Up");
    lcd.setCursor(0, 1);
    lcd.print("Internet Up");
    delay(2000);
    lcd.clear();
  }
  else{
    Serial.println("Internet Down");
      lcd.setCursor(0, 1);
      lcd.print("Internet Down");
      delay(2000);
      lcd.clear();
  }
}

void checkLatency(){
  
}

void lightShow(){
  
}
