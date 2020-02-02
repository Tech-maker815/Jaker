#include<SoftwareSerial.h>
#include<TinyGPS.h>
SoftwareSerial mySerial(2,3); //make RX arduino line is pin 2, make TX arduino line is pin 3.
#include<LiquidCrystal.h>
LiquidCrystal lcd(4, 5, 6, 7, 8, 9); 
void updateSerial();

const int xInput = A3;
const int yInput = A2;
const int zInput = A1;


// initialize minimum and maximum Raw Ranges for each axis
int RawMin = 290;
int RawMax = 370;



float lat, lon;
SoftwareSerial gpsSerial(10,11);
TinyGPS gps;

void setup() {
  
  // put your setup code here, to run once:
   //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  
 gpsSerial.begin(9600);
  mySerial.begin(9600);
  Serial.begin(9600);
  
  lcd.begin(16,2);  
  lcd.print("Accident Alert  ");
  lcd.setCursor(0,1);
  lcd.print("     System     ");
  delay(2000);
  lcd.clear();
  lcd.print("Initializing");
  lcd.setCursor(0,1);
  lcd.print("Please Wait...");
  delay(1000);
  

  
  //Begin serial communication with Arduino and SIM800L
  

  Serial.println("Initializing..."); 
  delay(1000);

  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();

  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CMGS=\"+919061980727\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  
  Serial.println("Initialized Successfully");
  lcd.clear();
  lcd.print("Initialized");
  lcd.setCursor(0,1);
  lcd.print("Successfully");
  delay(2000);
  lcd.clear();
  lcd.print("Callibrating ");
  lcd.setCursor(0,1);
  lcd.print("Acceleromiter");
  lat=10.055706;
  lon=76.354057;

   lcd.clear();
  lcd.print("Waiting For GPS");
  lcd.setCursor(0,1);
  lcd.print("     Signal    ");
  delay(2000);
  while(gpsSerial.available()){
    if(gps.encode(gpsSerial.read())){
      gps.f_get_position(&lat,&lon);
      Serial.print("Position: ");
      Serial.print("lat: "); Serial.print(lat,6); Serial.print(" ");
      Serial.print("lon: "); Serial.println(lon,6); 
    }
  }
  delay(2000);
  lcd.clear();
  lcd.print("GPS is Ready");
  delay(1000);
  lcd.clear();
  lcd.print("System Ready");
  Serial.println("System Ready..");
  

}

void loop() 
{   
  
   int value1=analogRead(xInput);
    int value2=analogRead(yInput);
    int value3=analogRead(zInput);
    
    
    
    Serial.print("x=");
    Serial.println(value1);
    Serial.print("y=");
    Serial.println(value2);
    Serial.print("z=");
    Serial.println(value3);
   // delay(1000);
     if(value1 < RawMin || value1 > RawMax  || value2 < RawMin || value2 > RawMax )     {
      
     
      
      
      
      Serial.print("Position: "); lcd.print("Position: ");
      Serial.print("lat: "); Serial.print(lat,6); Serial.print(" ");
      Serial.print("lon: "); Serial.println(lon,6); 
      lcd.print("lat: "); lcd.print(lat,6); lcd.print(" ");
      lcd.print("lon: "); lcd.println(lon,6); 
        lcd.clear();
    
      lcd.print("Sending SMS ");
      Serial.println("Sending SMS");
      mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();

  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CMGS=\"+919061980727\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
   mySerial.print("Latitude:");
   
   mySerial.println(lat,6);
   updateSerial();
   delay(500); 
   mySerial.print("Longitude:");
   
   mySerial.println(lon,6);
   updateSerial();
   
   delay(500); 
   mySerial.print("http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=");
   
   mySerial.print(lat,6);
   mySerial.print("+");              //28.612953, 77.231545   //28.612953,77.2293563
   mySerial.print(lon,6);
    mySerial.write(26);
   delay(2000);
   updateSerial();
 
  delay(2000);
  Serial.println("SMS Sent");
      delay(2000);
      lcd.clear();
      lcd.print("System Ready");
    } 
    

        
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
