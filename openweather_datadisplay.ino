#include <LiquidCrystal.h>
#include<ESP8266WiFi.h>
#include<ArduinoJson.h>

const char* ssid="StayOff";
const char* pass="72supdal72";
//const char* ssid="Harleen's iPhone";
//const char* pass="123456789";

const char* server="api.openweathermap.org";
//LiquidCrystal_I2C lcd(0x27,16,2);
LiquidCrystal lcd(12,13,16,5,4,0);    //rst,en,D4,D3,D2,D1

WiFiClient client;
int counter=60;
String cityID="1273294";
String APIKEY="d9dfacdf3ef2dad5ea64e073cb257358";
String result;
String weatherDescription="";
String weatherLocation="";
float  temperature,humidity,pressure;
String Country="";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid,pass);
  lcd.begin(16,2);
  Serial.println("connecting to wifi");
  lcd.setCursor(1,0);
  lcd.print("connecting");
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    lcd.print(".");
    delay(100);
    }
    Serial.println("WIFI CONNECTED");
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("connected");
}

void loop() {
   if(counter==60)
   {
     counter=0;
     lcd.clear();
     lcd.print("GETTING DATA");
     delay(1000);
     getWeatherData();                      //Send req to openweathermap after 1 min to get weather information
    }
  else
  {
    counter++;
    displayWeather(weatherLocation,weatherDescription);   // to display the location and description on lcd
    delay(5000);
    displayConditions(temperature,humidity,pressure);     // to display temp,humidity and pressure conditions
    delay(5000);
  }  
}

void getWeatherData()
{
  if(client.connect(server,80))               //connect to server
  {
    Serial.println("ESP CONNECTED TO SERVER SUCCESSFULLY");
    client.println("GET /data/2.5/weather?id="+cityID+"&units=metric&APPID="+APIKEY);   //units=metric is for temp in kelvin 
    client.println("HOST:api.openweathermap.org");
    client.println("User-Agent:ArduinoWiFi/1.1");
    client.println("Connection:close");
    client.println();
    }
  else
  {
    Serial.println("CONNECTION FAILED");
    }
  while(client.available())
  {
    result=client.readStringUntil('\r');
    Serial.print(result);
    }
    client.stop();
    result.replace('[',' ');    //making the complex response into simple json format with key value pairs and removing array structs
    result.replace(']',' ');
    Serial.println(result);
    char jsonArray[result.length()+1];   // converting string response to char array since that is required for json parser
    result.toCharArray(jsonArray,sizeof(jsonArray));
    jsonArray[result.length()+1]='\0';

    StaticJsonBuffer<1024> json_buf;   // json_buf is the json document responsible for reserving and releasing memory used in arduinoJson
    JsonObject &root=json_buf.parseObject(jsonArray); //this will create a json object reference(the final parsing changes will be done in original) and store the parsed output          

    if(!root.success())   //to verify if parsing succedes
   {
    Serial.println("parse object failed!!");
    }
    String description=root["weather"]["description"];   //everything in [] is json variant it is reference of value stored in a json document
    String location   =root["name"];
    String country    =root["sys"]["country"];
    float temp        =root["main"]["temp"];
    float humid       =root["main"]["humidity"];
    float pres        =root["main"]["pressure"];
    String weather    =root["weather"]["main"]; 

    weatherDescription=description;
    weatherLocation=location;
    Country=country;
    temperature=temp;
    humidity=humid;
    pressure=pres;
  }

 void displayWeather(String location,String description)
 {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(location);
  lcd.print(", ");
  lcd.print(Country);
  lcd.setCursor(0,1);
  lcd.print(description);
  }

  void displayConditions(float Temp,float Humid,float Pre)
  {
    lcd.clear();
    lcd.print("T:");
    lcd.print(Temp,1);   //temp upto 1 decimal place
    lcd.print((char)223); // ascii value for degree symbol
    lcd.print("c");
    lcd.print("H:");
    lcd.print(Humid,0);  //without decimal
    lcd.print("%");
    lcd.setCursor(0,1);
    lcd.print("P:");
    lcd.print(Pre,1);
    lcd.print("hPa");
    
  }
