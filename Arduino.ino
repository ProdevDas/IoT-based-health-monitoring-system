#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>
#define ARDUINO_TX 2   // Connect this to the RX pin of the ESP8266
#define ARDUINO_RX 3   // Connect this to the TX pin of the ESP8266
SoftwareSerial esp(ARDUINO_RX, ARDUINO_TX);
LiquidCrystal_I2C lcd(0x27,16,2); 
#define ONE_WIRE_BUS 9
#define TEMPERATURE_PRECISION 12
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress tempDeviceAddress;
int numberOfDevices, temp, buzzer = 8;
const int PulseWire = A0;
int myBPM, Threshold = 550;
PulseSensorPlayground pulseSensor;
unsigned long previousMillis = 0;
const long interval = 5000;
void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.init();         
  lcd.backlight();
  esp.begin(9600);
  sensors.begin();
  numberOfDevices = sensors.getDeviceCount();
  pulseSensor.analogInput(PulseWire);
  pulseSensor.setThreshold(Threshold);
  pulseSensor.begin();
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);
  lcd.setCursor(0, 0);
  lcd.print("  IoT Patient");
  lcd.setCursor(0, 1);
  lcd.print(" Monitor System");
  delay(1500);
  digitalWrite(buzzer, LOW);
  lcd.clear();
}

void loop()
{
  int myBPM = pulseSensor.getBeatsPerMinute();
  if (pulseSensor.sawStartOfBeat())
  {
    
    beep();
    lcd.setCursor(0, 1);
    lcd.print("HEART:");
    lcd.print(myBPM);
    lcd.setCursor(9, 1);
    lcd.print(" BPM");
    delay(20);
  }
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);

  // Check if reading was successful
  if(temp != DEVICE_DISCONNECTED_C) 
  {
    lcd.setCursor(0, 0);
    lcd.print("BODY:");
    lcd.print(temp);
    lcd.print(" *C");
  } 
  else
  {
    Serial.println("Error: Could not read temperature data");
  }
  
  
  Serial.print(myBPM);
  Serial.print(",");
  Serial.print(temp);
  Serial.println();
  delay(1000);
}

int printTemperature(DeviceAddress deviceAddress)
{
  int temp = sensors.getTempC(deviceAddress);
  return temp;
}

void beep()
{
  digitalWrite(buzzer, HIGH);
  delay(150);
  digitalWrite(buzzer, LOW);
}

