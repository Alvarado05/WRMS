/*
Wireless Router Monitoring System
Status: Release V1.1
By: Carlos Alvarado, Eliud Perez
*/

//////////////////////////
////Required Libraries////
//////////////////////////

//Built-in Arduino libraries
#include <LiquidCrystal.h>
#include <SPI.h>
#include <EEPROM.h>
#include <MFRC522.h>

//Custom libraries located in WRMS Repo
#include <Buzzer.h>

//Constants
#define RST_PIN   5
#define SS_PIN    53

////////////////
////Settings////
////////////////

//Relay
int relayPin = 2;

//Buttons
int button1 = 22;
int button2 = 23;

//Buzzer
int buzzerPin = 6; //Pin for the buzzer

//LEDs
int R_LED = 24;
int Y_LED = 25;
int G_LED = 26;

//Variables
unsigned long AuthUserSession = 0;
int buttonSt1, buttonSt2;

////////////////////////////
////Class Instantiations////
////////////////////////////

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
MFRC522::MIFARE_Key key;
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
Buzzer buzz = Buzzer(buzzerPin);

/////////////
////Setup////
/////////////

void setup() {

  ///////////////
  ///Pin Modes///
  ///////////////

  //Relay
  pinMode(relayPin, OUTPUT);

  //Buttons
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

  //LEDs
  pinMode(R_LED, OUTPUT);
  pinMode(Y_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);


  ////////////////////
  ///Pin Initiation///
  ////////////////////

  //Relay
  digitalWrite(relayPin, LOW);

  //LEDs
  digitalWrite(R_LED, LOW);
  digitalWrite(Y_LED, LOW);
  digitalWrite(G_LED, LOW);


  ////////////
  ///Begins///
  ////////////

  //LCD
  lcd.begin(16, 2);        // set up the LCD's number of columns and rows

  //Serial
  // Serial.begin(9600);
  Serial1.begin(9600);
  Serial1.println("Serial Ready");
  Serial1.println();
  Serial2.begin(115200);

  //RFID
  SPI.begin();         // Init SPI bus
  mfrc522.PCD_Init();  // Init MFRC522 card
}


/////////////////
////Main Loop////
/////////////////

void loop() {
  // set the cursor to column 0, line 0
  lcd.setCursor(0, 0);
  lcd.print("WRMS on Standby");
  if(AuthUserSession > millis()){
    lcd.setCursor(0, 1);
    lcd.print("Access Granted");
  }
  else{
    lcd.setCursor(0, 1);
    lcd.print("                ");
  }

  checkUser();
  if(Serial1.available()){
    int input = Serial1.parseInt();
  //Every function is explained with details in the declaration of it. 

    if(input==1){           //If the number 1 is introduced in the Serial Monitor, it will check the internet
      checkInternet();
    }
    else if(input==2){     //If the number 2 is introduced in the Serial Monitor, it will reboot the router
      rebootRouter();
    }
    else if(input==3){    //If the number 3 is introduced in the Serial Monitor, it will show the password on the phone or Serial Monitor.
      showPassSerial();
    } 
    else if(input==4){    //If the number 4 is introduced in the Serial Monitor, it will give you the latency of your internet.
      checkLatency();
    }
    else if(input==5){   //If the number 5 is introduced in the Serial Monitor, it will give you the option to change the password on the phone or Serial Monitor.
      changePass();
    }
    else if(input==4020){
      lightShow();
    }
  }
  else if(checkButtons()==1 && AuthUserSession > millis()){
    if(buttonSt1==1){
      checkInternet();
    }
    else if(buttonSt1==2){
      checkLatency();
    }
    else if(buttonSt2==1){
      showPassLCD();
    }
    else if(buttonSt2==2){
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

/////////////////
////Functions////
/////////////////



void rebootRouter(){                // Activates the Relay which cut’s the power to the router for 10 seconds.
  lcd.clear();
  lcd.print("Rebooting...");
  Serial1.print("Rebooting Router...");
  digitalWrite(2, HIGH);
  delay(5000);
  digitalWrite(2, LOW);
  lcd.print("Done");
  Serial1.println("Done");
  delay(2000);
  lcd.clear();
}

void showPassLCD(){                     // Illustrates the password of the router in the LCD panel with the LiquidCrystal.h library 
  lcd.clear();
  lcd.print("Wifi Password:");
  lcd.setCursor(0, 1);                  // Specific row in the LCD panel
  lcd.print(readPass());
  delay(5000);
  lcd.clear();                          // Clear whatever is in the panel at the moment.
}

void showPassSerial(){                       // Demonstrates the password of the router to your mobile phone or Serial Monitor.
  Serial1.println("The Wifi Password is:");
  Serial1.println(readPass());                // Will show us the password that is in stored in the readPass funcion.
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
  Serial1.println("User Authenticated");
  buzz.alert();
  }
  mfrc522.PICC_HaltA();
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
    return;
  }
  return;
}

String readPass(){              // Reads the password of the router from the internal memory of the Aruduino
  String pass;
  char password2[13] = {"0"};     //Reads the password without it the password getting erased. 0 is the address

  for(int i=0 ; i<12 ;i++ ){
    password2[i] = EEPROM.read(i);
  }
  pass = password2;               // Stores the char to a String variable to later return the passoword.

  return pass;
}

void changePass(){    //Works with the EEPROM of the Microcontroller to write and store a new password.
  String nPass;
  char password[13]; //Strings of char for the password. thats stores in eeprom

  Serial1.print("Enter the new password: ");
  while(!Serial1.available()){}    // It will enter when there is nothing and when it does it gets out, simple way of waiting in the Serial.
  nPass = Serial1.readString();
  nPass.toCharArray(password, 13);  //Converts the string to char or copies the String’s characters to the supplied buffer.

  for(int i=0 ; i<12 ;i++ ){    
    EEPROM.write(i,password[i]);    //Loops through all char so that way it can write in the EEPROM.
  }
  
  Serial1.println("");
  Serial1.print("Password: ");     
  Serial1.println(readPass());     //Will show us the password that is in stored in the readPass funcion.
}

int connectInternet()                              //Pings google.com to the router and waits if it responds this
{                                                  // will let us know if there is any internet connection.   
  String response, string_buffer;
  Serial2.println("AT+CIPSTART=\"TCP\",\"google.com\",80");    //connect module to googles server
  response = Serial2.readString();                             //Stores the reading of the ESP.
  delay(100);  
  Serial2.println("AT+CIPCLOSE");                             //close connection
  string_buffer = Serial2.readString();                      //Stores a second response that we don't want to use
  delay(50);
  
  if(response.indexOf("OK") > 0)                        //When you read the OK in the Serial you will get a 1 in return.
  {
    return 1;
  }
    else
  {
    return 0;
  }
}

void checkInternet(){                       //Checks if the Internet is running or the Internet is down for the moment.
  Serial1.print("Checking for Internet...");
  lcd.clear();
  lcd.print("Cheking Wifi...");
  lcd.setCursor(0, 1);
  if(connectInternet() == 1){                 //If the we received a connection from the internet it will say 
    Serial1.println("Internet Up");            // in the serial and in the LCD display the Internet is up.
    lcd.print("Internet Up");
    digitalWrite(G_LED,HIGH);
    delay(2000);
    lcd.clear();
    digitalWrite(G_LED,LOW);
  }
  else{
    Serial1.println("Internet Down");         //When the internet is down or not connected it will say              
      lcd.print("Internet Down");           // in the serial and in the LCD display the Internet is down.
      digitalWrite(R_LED,HIGH);
      delay(2000);
      lcd.clear();
      digitalWrite(R_LED,LOW);
  }
}

void checkLatency(){                        //Pings google.com to the router and takes the average time that it took to respond.
  String response;                                 //Variable that reads the Serial of the ESP.
  Serial1.print("Checking Latency...");
  lcd.clear();
  lcd.print("Latency Check...");
  lcd.setCursor(0, 1);
  Serial2.println("AT+PING=\"www.google.com\"");  //Gives the command to the ESP to get the ping
  response = Serial2.readString();
  delay(100);
  int pingPos = response.indexOf("+",3)+1;        //After the AT+ it starts searching for the next +.

  int ping = atoi(&response[pingPos]);           //Converts an array of characters to an int.
  // Serial1.println(ping);

  if(ping > 0 && ping <= 100){                  //If your ping is lower than 101 you will get
    Serial1.println("Good Internet");          // the message of Good Internet.
    lcd.print("Good Internet");
    digitalWrite(G_LED,HIGH);               //The Green LED will turn on.
    delay(2000);
    lcd.clear();
    digitalWrite(G_LED,LOW);               //The Green LED will turn on.
  }

  else if(ping >= 101 && ping <= 200){       //If your ping is lower than 201 and higher than 101 
    Serial1.println("Potential Problems");   // you will get the message of potential problems.
    lcd.print("Some Problems");
    digitalWrite(Y_LED,HIGH);             //The Yellow LED will turn on.
    delay(2000);
    lcd.clear();
    digitalWrite(Y_LED,LOW);               //The Green LED will turn on.
  }  

  else{
    Serial1.println("Slow Intenet");      //If your ping is higher than 200 you will get this message.
    lcd.print("Slow Internet");
    digitalWrite(R_LED,HIGH);           //The Red LED will turn on.
    delay(2000);
    lcd.clear();
    digitalWrite(R_LED,LOW);               //The Green LED will turn on.
  }  
  
  if(response.indexOf("OK") > 0){       //When you read the OK in the Serial you will get a 1 in return.
    return 1;
  }
  else{
    return 0;
  }
}

void lightShow(){                     //LED's begin to turn on and off and the buzzer sounds of in a fearsome way.
  Serial1.println("Imperial March Now Playing");
  buzz.imperialMarch();
}

/*
Wireless Router Monitoring System
Status: Release V1.1
By: Carlos Alvarado, Eliud Perez
*/