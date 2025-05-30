#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX
#define Soil_sensor  A3
#define LDR_sensor A2
#define AirQ_sensor  A0
#define Rain_sensor  13
#include <Servo.h>
Servo myservo;
int pos = 0;
#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define DHTTYPE DHT11
#define Dht_dpin A1

DHT dht(Dht_dpin, DHTTYPE);

#define Soil_Relay 4
#define Exhaust_Fan 5
#define Artificial_LightsP 6

namespace
{
const char * USER_AGENT = "IOTGreenHouse"; // Assgin the user agent
const char * VERSION =  "1.0"; // Assign the version
const char * METHOD = "POST"; // Set the method
const char * TOKEN = "BBFF-OcDZi8xPS0DaEo02T2u141pJSfBkys"; // Assign your Ubidots TOKEN
const char * DEVICE_LABEL = "IOTGreenHouse"; // Assign the device label
const char * VARIABLE_LABEL1 = "Soil_Moisture"; // Assign the variable label
const char * VARIABLE_LABEL2 = "Light_Intensity"; // Assign the variable label
const char * VARIABLE_LABEL3 = "Temperature"; // Assign the variable label
const char * VARIABLE_LABEL4 = "Humidity"; // Assign the variable label
const char * VARIABLE_LABEL5 = "Air_Quality_Index"; // Assign the variable label
const char * VARIABLE_LABEL6 = "Rain_Fall"; // Assign the variable label
}
char telemetry_unit[100];
char str_sensor1[10];
char str_sensor2[10];
char str_sensor3[10];
char str_sensor4[10];
char str_sensor5[10];
char str_sensor6[10];
void setup()
{
  myservo.attach(10);
  pinMode(AirQ_sensor, INPUT);
  pinMode(Soil_sensor, INPUT);
  pinMode(Rain_sensor, INPUT);
  pinMode(Soil_Relay, OUTPUT);
  pinMode(LDR_sensor, INPUT);
  pinMode(Artificial_LightsP, OUTPUT);
  pinMode(Dht_dpin, INPUT);
  pinMode(Exhaust_Fan, OUTPUT);
  digitalWrite(Exhaust_Fan, HIGH);
  digitalWrite(Soil_Relay, HIGH);
  digitalWrite(Artificial_LightsP, HIGH);
  delay(1000);
  Serial.begin(115200);
  mySerial.begin(115200);
  dht.begin();
  lcd.begin();
  //  lcd.init();
  // lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("  GREEN HOUSE    ");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print(" MONITORING IOT   ");
  delay(1000);
}

void loop()
{
  //*******************************************************************************
  // soil sensor condition loop
  int soil_value = analogRead(Soil_sensor);
  soil_value = map(soil_value, 0, 1023, 100, 0);
  Serial.print("soil value");
  Serial.println(soil_value);
  delay(100);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Soil Mois: ");
  delay(500);
  lcd.setCursor(11, 0);
  lcd.print(soil_value );
  lcd.setCursor(15, 0);
  lcd.print("%");
  delay(1000);
  if (soil_value > 20)
  {
    digitalWrite(Soil_Relay, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("SOIL is WET       ");
    delay(500);
    lcd.setCursor(0, 1);
    lcd.print("MOTOR OFF         ");
    delay(1000);
  }
  else
  {
    digitalWrite(Soil_Relay, LOW);
    lcd.setCursor(0, 0);
    lcd.print("SOIL is DRY       ");
    delay(500);
    lcd.setCursor(0, 1);
    lcd.print("MOTOR ON             ");
    delay(1000);
  }
  //*******************************************************************************
  //LDR_sensor condition loop
  int light_value = digitalRead(LDR_sensor);
  lcd.clear();
  if (light_value == 0)
  {
    digitalWrite(Artificial_LightsP, LOW);
    //   digitalWrite(Artificial_LightsN, LOW);
    lcd.setCursor(0, 0);
    lcd.print("SUN Light OK     ");
    delay(500);
    lcd.setCursor(0, 1);
    lcd.print("Lights OFF         ");
    delay(1000);
  }
  else
  {
    digitalWrite(Artificial_LightsP, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("SUN Light LOW    ");
    delay(500);
    lcd.setCursor(0, 1);
    lcd.print("Lights ON          ");
    delay(1000);
  }

  //*******************************************************************************
  //Rain_sensor condition loop
  int Rain_value = digitalRead(Rain_sensor);
  lcd.clear();
  if (Rain_value == 0)
  {
    lcd.setCursor(0, 0);
    lcd.print("Rain Fall: YES   ");
    myservo.write(90);              // tell servo to go to position in variable 'pos'
    delay(1000);
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("Rain Fall: NO    ");
    myservo.write(0);
    delay(1000);
  }
  //*******************************************************************************
  // Temp sensor  condition loop
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp & Humidity   ");
  lcd.setCursor(0, 1);
  lcd.print("IN");
  lcd.setCursor(4, 1);
  lcd.print("T:");
  lcd.setCursor(6, 1);
  lcd.print(t);
  lcd.setCursor(8, 1);
  lcd.print(char(223));
  lcd.setCursor(9, 1);
  lcd.print("C ");
  delay(500);
  lcd.setCursor(11, 1);
  lcd.print("H:");
  lcd.setCursor(13, 1);
  lcd.print(h);
  lcd.setCursor(15, 1);
  lcd.print("%");
  delay(1000);
  //*******************************************
  if (t <= 35)
  {
    digitalWrite(Exhaust_Fan, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Temp Normal        ");
    delay(500);
    lcd.setCursor(0, 1);
    lcd.print("Exhaust FAN OFF   ");
    delay(1000);
  }
  else
  {
    digitalWrite(Exhaust_Fan, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Temp HIGH         ");
    delay(500);
    lcd.setCursor(0, 1);
    lcd.print("Exhaust FAN ON    ");
    delay(1000);
  }
  //********************************************************************************
  // Air sensor loop
  int air_value = analogRead(AirQ_sensor);
  air_value = 0.5 * air_value;

  delay(100);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Air value: ");
  delay(500);
  lcd.setCursor(10, 0);
  lcd.print( air_value );
  lcd.setCursor(13, 0);
  lcd.print("AQI");
  delay(500);
  if (air_value <= 150)
  {
    lcd.setCursor(0, 1);
    lcd.print("Fresh Air             ");
    delay(1000);
  }
  else if (air_value > 300)
  {
    lcd.setCursor(0, 1);
    lcd.print("Polluted Air            ");
    delay(1000);
  }

  //*******************************************************************************
  //********************************************************
  lcd.setCursor(0, 0);
  lcd.print("Now Sending      ");
  lcd.setCursor(0, 1);
  lcd.print("Data to IOT      ");
  //*********************************************************
  mySerial.listen();
  delay(1000);
  char* command1 = (char *) malloc(sizeof(char) * 128);
  float sensor1 = soil_value;
  dtostrf(sensor1, 4, 2, str_sensor1);
  sprintf(command1, "init#");
  sprintf(command1, "%s%s/%s|%s|%s|", command1, USER_AGENT, VERSION, METHOD, TOKEN);
  sprintf(command1, "%s%s=>", command1, DEVICE_LABEL);
  sprintf(command1, "%s%s:%s", command1, VARIABLE_LABEL1, str_sensor1);
  sprintf(command1, "%s|end#final", command1);
  Serial.println(command1);// uncomment this line to print the command
  mySerial.print(command1);
  free(command1);
  delay(2000);
  //****************************************************************************************************
  char* command2 = (char *) malloc(sizeof(char) * 128);
  float sensor2 = light_value;
  dtostrf(sensor2, 4, 2, str_sensor2);
  sprintf(command2, "init#");
  sprintf(command2, "%s%s/%s|%s|%s|", command2, USER_AGENT, VERSION, METHOD, TOKEN);
  sprintf(command2, "%s%s=>", command2, DEVICE_LABEL);
  sprintf(command2, "%s%s:%s", command2, VARIABLE_LABEL2, str_sensor2);
  sprintf(command2, "%s|end#final", command2);
  Serial.println(command2);
  mySerial.print(command2);
  free(command2);
  delay(2000);
  //****************************************************************************************************
  char* command3 = (char *) malloc(sizeof(char) * 128);
  float sensor3 = t;
  dtostrf(sensor3, 4, 2, str_sensor3);
  sprintf(command3, "init#");
  sprintf(command3, "%s%s/%s|%s|%s|", command3, USER_AGENT, VERSION, METHOD, TOKEN);
  sprintf(command3, "%s%s=>", command3, DEVICE_LABEL);
  sprintf(command3, "%s%s:%s", command3, VARIABLE_LABEL3, str_sensor3);
  sprintf(command3, "%s|end#final", command3);
  Serial.println(command3);
  mySerial.print(command3);
  free(command3);
  delay(2000);
  //****************************************************************************************************
  char* command4 = (char *) malloc(sizeof(char) * 128);
  float sensor4 = h;
  dtostrf(sensor4, 4, 2, str_sensor4);
  sprintf(command4, "init#");
  sprintf(command4, "%s%s/%s|%s|%s|", command4, USER_AGENT, VERSION, METHOD, TOKEN);
  sprintf(command4, "%s%s=>", command4, DEVICE_LABEL);
  sprintf(command4, "%s%s:%s", command4, VARIABLE_LABEL4, str_sensor4);
  sprintf(command4, "%s|end#final", command4);
  Serial.println(command4);
  mySerial.print(command4);
  free(command4);
  delay(2000);
  //****************************************************************************************************
  char* command5 = (char *) malloc(sizeof(char) * 128);
  float sensor5 = air_value;
  dtostrf(sensor5, 4, 2, str_sensor5);
  sprintf(command5, "init#");
  sprintf(command5, "%s%s/%s|%s|%s|", command5, USER_AGENT, VERSION, METHOD, TOKEN);
  sprintf(command5, "%s%s=>", command5, DEVICE_LABEL);
  sprintf(command5, "%s%s:%s", command5, VARIABLE_LABEL5, str_sensor5);
  sprintf(command5, "%s|end#final", command5);
  Serial.println(command5);
  mySerial.print(command5);
  free(command5);
  mySerial.end();
  delay(2000);
  //****************************************************************************************************
  char* command6 = (char *) malloc(sizeof(char) * 128);
  float sensor6 = Rain_value;
  dtostrf(sensor6, 4, 2, str_sensor6);
  sprintf(command6, "init#");
  sprintf(command6, "%s%s/%s|%s|%s|", command6, USER_AGENT, VERSION, METHOD, TOKEN);
  sprintf(command6, "%s%s=>", command6, DEVICE_LABEL);
  sprintf(command6, "%s%s:%s", command6, VARIABLE_LABEL6, str_sensor6);
  sprintf(command6, "%s|end#final", command6);
  Serial.println(command6);
  mySerial.print(command6);
  free(command6);
  mySerial.end();
  delay(2000);
  //****************************************************************************************************

}
