
// Include ESP8266WiFi.h
#include <ESP8266WiFi.h>

// Include Firebase ESP8266 library (this library)
#include <FirebaseESP8266.h>

#define WIFI_SSID ""
#define WIFI_PASSWORD ""

// Insert Firebase project API Key
#define API_KEY ""

// Insert RTDB URLefine the RTDB URL
#define DATABASE_URL ""

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

#define FIREBASE_HOST ""
#define FIREBASE_AUTH "c4aGbLMAvlu7Y0pdvE1cqcvnmFMXeKV8BjmZLWqf"

#define RED_LED_PIN   4
#define YELLOW_LED_PIN   5
#define GREEN_LED_PIN 6

// Database absolute paths
String pathRedLedState = "/led/red/light_on";
String pathYellowLedState = "/led/yellow/light_on";
String pathGreenLedState = "/led/green/light_on";


void setup() 
{
  Serial.begin(9600);
  initWiFi();
  // Configura pinos como saída
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  
  // Assign the api key (required)
  config.api_key = API_KEY;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

   // Initialize the library with the Firebase authen and config
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 

  Firebase.reconnectWiFi(true);

}


bool isRedLedOn = false;
bool isYellowLedOn = false;
bool isGreenLedOn = false;

bool firstTime = true;

void loop()
{
  if (firstTime) {
    lightsInit();
    firstTime = false;
  }

  isRedLedOn = changeLedStateToFirebaseFieldValue( RED_LED_PIN,  pathRedLedState,  isRedLedOn );
               
  isYellowLedOn = changeLedStateToFirebaseFieldValue( YELLOW_LED_PIN,  pathYellowLedState,  isYellowLedOn);

  isGreenLedOn = changeLedStateToFirebaseFieldValue( GREEN_LED_PIN,  pathGreenLedState,  isGreenLedOn);

}

  
bool changeLedStateToFirebaseFieldValue ( int ledPin, String firebaseFieldPath, bool previousState)
{
  
  if ( getDataFromFirebaseField( firebaseFieldPath ) != previousState ) {
    
    bool isLedOn = getDataFromFirebaseField( firebaseFieldPath );    

    switchLed(ledPin, isLedOn );
    
    return isLedOn;
    
  } 
  
  return previousState;
}


void switchLed(int ledPin, bool state) 
{
  if (state) {
     digitalWrite(ledPin, HIGH);
  } else {
     digitalWrite(ledPin, LOW);
  }
}


bool getDataFromFirebaseField(String path)
{
  bool b = false;
  
  if (Firebase.getBool(fbdo, path) ) {
    // On successful Read operation, function returns 1  
    if (fbdo.dataType() == "boolean") {
      // print read data if it is integer
      b = fbdo.boolData();
      Serial.println(b);
    }

  } else {
    Serial.println(fbdo.errorReason());
  }
  return b;
}


void initWiFi()
{
// Select the same baud rate if you want to see the datas on Serial Monitor
  Serial.println("Serial communication started\n\n");  
           
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);

  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  //print local IP address
  Serial.println(WiFi.localIP());                                            
 
  delay(1000);
  
};

void lightsInit(){
  int i = 0;
  for (i ; i < 9; i++) {
   
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(YELLOW_LED_PIN, HIGH);

    if (i%3 == 1)
    {
      digitalWrite(GREEN_LED_PIN, LOW);
      digitalWrite(YELLOW_LED_PIN, LOW);
      digitalWrite(RED_LED_PIN, HIGH);

    } else if (i%3 == 2) {
      digitalWrite(GREEN_LED_PIN, LOW);
      digitalWrite(YELLOW_LED_PIN, HIGH);
      digitalWrite(RED_LED_PIN, LOW);

    } else {
      digitalWrite(GREEN_LED_PIN, HIGH);
      digitalWrite(RED_LED_PIN, LOW);
      digitalWrite(YELLOW_LED_PIN, LOW);
    }
    delay(100);     
  }
}
