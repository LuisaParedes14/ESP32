//SE USARA UN SENSOR PIR DE PRESENCIA Y UN LED, EN DONDE EL SENSOR DE PRESENCIA AL DETECTAR ALGO
//HARÁ QUE EL LED SE ENCIENDA Y CUANDO DEJE DE DETECTAR SE APAGUE

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#define SERVICE_UUID "3feb1e8a-3981-4045-ad39-b225135013a0"
#define CONTROL_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define LED 23
int ledPin = 12; // declaramos el led en el pin 12 de la ESP32
int sensorPin=14; // declaramos el sensor PIR en el pin 14
int val = 0; //variable para asignar la lectura del sensor PIR
String MessagePir ;
char ledStatus = 48; // 0 in ASCII

// New characteristic with object to manage it
BLECharacteristic controlCharacteristic(
CONTROL_CHARACTERISTIC_UUID,BLECharacteristic::PROPERTY_READ |BLECharacteristic::PROPERTY_WRITE);
BLECharacteristic *nameCharacteristic;
void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE!");
  Serial.println("Initializing device");
  BLEDevice::init("Detectar Presencia"); // Initializing the device with its name
  Serial.println("Creating server");
  BLEServer *pServer = BLEDevice::createServer(); // Create the server
  Serial.println("Adding service UUID");
  BLEService *pService = pServer->createService(SERVICE_UUID); // Creating a new service into server
  // Adding a characteristic with the object name (official UUID), without object (this characteristic will not change)
  Serial.println("Adding name characteristic");
  nameCharacteristic = pService->createCharacteristic(
  BLEUUID((uint16_t)0x2A00),
  BLECharacteristic::PROPERTY_READ);
  nameCharacteristic->setValue(MessagePir.c_str());
  // Adding a characteristic to control the led with 0 and 1
  Serial.println("Adding control characteristic");
  pService->addCharacteristic(&controlCharacteristic);
  controlCharacteristic.setValue(&ledStatus); // Value uint8_t with length 1
  Serial.println("Starting...");
  pService->start();
  Serial.println("Creating advertising");
  // BLEAdvertising *pAdvertising = pServer->getAdvertising(); // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
  pinMode (LED, OUTPUT); // Set the LED pin as OUTPUT
  pinMode(ledPin, OUTPUT); //El pin 12 del arduino lo asignamos como salida para el led
  pinMode(sensorPin, INPUT);//El pin 11 lo asignamos como entrada para la señal del sensor
  for(int i = 0; i > 30; i++) //Utilizamos un for para calibrar el sensor depende del tipo de sensor que utilicemos va a cambiar el tiempo de calibración
  {
  delay(1000);
  }
  delay(50);
}
void loop() {
  std::string controlValue = controlCharacteristic.getValue();
  if (controlValue[0] != ledStatus) {
  Serial.print("Value changed... new value: ");
  Serial.println(controlValue[0]);
  ledStatus = controlValue[0];
  if (ledStatus == 48) {
  digitalWrite(LED, LOW); // LED Off
  }
  else if (ledStatus == 49) {
  digitalWrite(LED, HIGH); // LED On
  }
}
val = digitalRead(sensorPin); //Lee el valor de la variable (val)
if (val == HIGH) //Si detecta que hay movimiento manda activar el led que hay conectado en el pin 12 del arduino
{
  MessagePir = "Detected"; // Se ha detectado presencia
  digitalWrite(ledPin, HIGH); 
}
else //Si la condición anterior no se cumple manda apagar el led
{
   MessagePir = "No Detected"; //No se ha detectado presencia
  digitalWrite(ledPin, LOW);
}
nameCharacteristic->setValue(MessagePir.c_str());
//Serial.println(MessagePir);
//Serial.println(MessagePir.c_str());
//nameCharacteristic->setValue((uint8_t*)&MessagePir,11);
delay(1000);

}
