
#include <NimBLEDevice.h>

// BLECharacteristics defined for connection with the devices
NimBLECharacteristic *LOCKCharacterstics, *deviceIDCharacteristic, *firmwareCharacterstics, *batteryCharacterstic;
BLEAdvertising *pAdvertising;
#define LOCK_SYSTEM_UUID "0000FFB1-0000-1000-8000-00805F9B34FB"
#define LOCK_SYSTEM_UUID_TX "0000FFB2-0000-1000-8000-00805F9B34FB"
#define DEVICE_ID_SERVICE_UUID "00001510-0000-1000-8000-00805f9b34fc"
#define DEVICE_ID_CHARACTERISTIC_UUID_TX "0000ffc2-0000-1000-8000-00805f9b34fb"
#define ACK_CHARACTERISTIC_UUID_TX "0000ffc1-0000-1000-8000-00805f9b34fb"
#define FIRMWARE_CHARACTERISTIC_UUID_TX "0000ffc1-0000-1000-8000-00805f9b34fb"
#define BATTERY_SERVICE_UUID "0000180F-0000-1000-8000-00805f9b34fb"
#define BATTERY_CHARACTERISTIC_UUID_TX "00002A19-0000-1000-8000-00805f9b34fb"
extern bool deviceConnected;
// nible
int first_ble_clear = 0;
// extern bool print_data;
int first_ble = 0, indication_1 = 0;
extern int firstTime;
bool oldDeviceConnected = false;
BLEServer *pServer = NULL;
// BLECharacteristics defined for connection with the device
//  BLECharacteristic *LOCKCharacterstics, *deviceIDCharacteristic, *firmwareCharacterstics, *batteryCharacterstic;
// BLEAdvertising *pAdvertising;
// Variable used for checking if the device is connected or not
//  bool deviceConnected = false;
// int first_ble = 0;
// For sending different values to the charateristics based on the values that has been written
bool takeECG = false;
bool takeSpo2 = false;
bool takeBP = false; // Bp value taking condition
bool takeTemp = false;
bool resetAll = false;
bool takeBPM = false; // take bpm akash addede
int phonetype = 0;    // 0 = Android, 1 = iPhone
// extern unsigned long system_start;
int ecgdisplay2 = 0;
int first_spo2 = 0;
int last_spo2 = 0;
int final_SpO2 = 70;
int first_temp = 0;
int final_temp = 0;

int isBLEWrite = 0;
class MyServiceCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
    first_ble = 1;
    oldDeviceConnected = true;
    Serial.print("Connected to: Finnix_Loan_Buster");
    // system_start = millis();
  };

  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
  }
};
class LOCKCharacteristicCallbacks : public BLECharacteristicCallbacks
{
  void onRead(BLECharacteristic *pCharacteristic)
  {
  }
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    std::string value = pCharacteristic->getValue();
    if (value.length() > 0)
    {
      // if the instruction has been sent for taking the ECG Android
      if (value[0] == 'a')
      {
        //     Serial.println("ECG Enabled");
        ecgdisplay2 = 1;
        digitalWrite(26, LOW);
        takeECG = true;
        takeSpo2 = false;
        takeBP = false;
        takeTemp = false;
        // //tft.fillRect(//tft_BLACK);
        phonetype = 0; // Android
        isBLEWrite = 1;

        Serial.print("Sanju");
      }
      if (value[0] == 'Z')
      {
        //     Serial.println("ECG Enabled");

                Serial.println("Ending BLE connection Now");
        indication_1 = 1;
      }

      // ECG for iPhone if the instruction has been sent for taking the ECG
      if (value[0] == 'x')
      {
        //     Serial.println("ECG Enabled");
        digitalWrite(26, LOW);
        ecgdisplay2 = 1;
        takeECG = true;
        takeSpo2 = false;
        takeBP = false;
        takeTemp = false;
        // firstTime=0;
        // tft.fillScreen(//tft_BLACK);
        phonetype = 1; // iPhone
        isBLEWrite = 1;
        Serial.write(value[0]); // added by Sheshnath for switchSy UART
      }

      // if the instruction has been sent for taking the Spo2
      if (value[0] == 'i')
      {
        Serial.println("spo2 sent command coming ");
        // firstTime=0;
        // //tft.fillScreen(//tft_BLACK);
        takeECG = false;
        takeSpo2 = true;
        takeBP = false;
        takeTemp = false;
        first_spo2 = 1;
        last_spo2 = 1;
      }
      // if the instruction has been sent for taking the ECG switchcy
      if ((value[0] == 'b') || (value[0] == 'c') || (value[0] == 'd') || (value[0] == 'e') || (value[0] == 'f') || (value[0] == 'g') || (value[0] == 'h'))
      {
        Serial.println("ECG swichy " + value[0]);
        // //tft.fillScreen(//tft_BLACK);
        // firstTime=0;
        digitalWrite(26, LOW);
        takeECG = true;

        Serial.write(value[0]); // added by Sheshnath for switchSy UART
      }

      // if the instruction has been sent for taking the temp
      if (value[0] == 'j')
      {
        //  Serial.println("Temperature Enabled");
        firstTime = 0;
        // tft.fillScreen(//tft_BLACK);
        Serial.print(value[0]);
        takeECG = false;
        takeSpo2 = false;
        takeBP = false;
        takeTemp = true;
        first_temp = 1;
        if (last_spo2 == 1)
        {
          last_spo2 = 0;
        }
      }

      // if the instruction has been sent for taking the Blood Pressure
      if (value[0] == 'k')
      {
        //  Serial.println("Blood Pressure Enabled");
        // firstTime=0;
        //  //tft.fillScreen(//tft_BLACK);
        // Serial.print(value[0]) ;
        // print_data = true;
        takeECG = false;
        takeSpo2 = false;
        takeBP = true;
        takeTemp = false;
      }
      // if the instruction has been sent for taking the reset all param
      if ((value[0] == 'ED') || (value[0] == 'l'))
      {
        //   Serial.println("Reset Enabled for the devices");
        // tft.fillScreen(//tft_BLACK);
        firstTime = 0;
        Serial.print("Reset Command Get from App ");
        resetAll = true;
        takeECG = false;
        takeSpo2 = false;
        takeBP = false;
        takeTemp = false;
        digitalWrite(26, HIGH);
      }
    }
  }
};
void StartBLE()
{

  Serial.println("Start BLE NiMBLE Sequence");
  BLEDevice::init("Finnix_Loan_Buster");

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServiceCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(LOCK_SYSTEM_UUID);
  BLEService *deviceIdService = pServer->createService(DEVICE_ID_SERVICE_UUID);
  BLEService *batteryService = pServer->createService(BATTERY_SERVICE_UUID);

  // Create the BLE ECG Characteristic and define its properties
  LOCKCharacterstics = pService->createCharacteristic(
      LOCK_SYSTEM_UUID,
      NIMBLE_PROPERTY::READ |
          NIMBLE_PROPERTY::WRITE |
          NIMBLE_PROPERTY::NOTIFY);
  LOCKCharacterstics->setCallbacks(new LOCKCharacteristicCallbacks());

  // Create the BLE device Id Characteristic
  deviceIDCharacteristic = deviceIdService->createCharacteristic(
      DEVICE_ID_CHARACTERISTIC_UUID_TX,
      NIMBLE_PROPERTY::READ |
          NIMBLE_PROPERTY::NOTIFY);

  // Create the BLE firmware Characteristic
  firmwareCharacterstics = deviceIdService->createCharacteristic(
      FIRMWARE_CHARACTERISTIC_UUID_TX,
      NIMBLE_PROPERTY::READ |
          NIMBLE_PROPERTY::NOTIFY);

  // Create the BLE battery service Characteristic
  batteryCharacterstic = batteryService->createCharacteristic(
      FIRMWARE_CHARACTERISTIC_UUID_TX,
      NIMBLE_PROPERTY::READ |
          NIMBLE_PROPERTY::NOTIFY);
  LOCKCharacterstics->addDescriptor(new NimBLE2904());
  deviceIDCharacteristic->addDescriptor(new NimBLE2904());
  firmwareCharacterstics->addDescriptor(new NimBLE2904());
  batteryCharacterstic->addDescriptor(new NimBLE2904());
  pService->start();
  deviceIdService->start();
  batteryService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  BLEAdvertisementData adv;
  adv.setName("LB1");
  /*
    String myMACString = getMACinString();
    String myMACString= BLEDevice::getAddress().toString().c_str();
    myMACString.replace(":", "");
    Serial.println(myMACString);
  */
  Serial.println("Debug-1");
  // std::string s = BLEDevice::getAddress().toString().c_str();
  String s = BLEDevice::getAddress().toString().c_str();
  /// String myMACString = getMACinString();
  s.replace(":", "");
  char *char_array = new char[s.length() + 1];
  // make sure that the new string is null terminated
  char_array[s.length()] = '\0';
  for (int i = 0; i < s.length(); i++)
  {
    char_array[i] = s[i];
  }
  s.replace(":", "");
  char manufacturerData[13];
  // Serial.println("myMACString");
  // Serial.println(myMACString);
  int i;
  for (i = 0; i < 13; i++)
  {
    manufacturerData[i] = char_array[i];
    Serial.print("Mac Adrress= ");
    Serial.println(manufacturerData[i]);
  }

  // Serial.println("Manufactordata");
  Serial.println(manufacturerData);
  // const char manufacturerData[] = { 0xFF,  0x01 , 0x01,  0xFF,  0x07,  0x40};
  adv.setManufacturerData(manufacturerData);
  //  adv.setvalue(112345667);
  pAdvertising->addServiceUUID(DEVICE_ID_SERVICE_UUID);
  // adv.setCompleteServices(DEVICE_ID_SERVICE_UUID));
  pAdvertising->setAdvertisementData(adv);
  // pAdvertising->addServiceUUID(DEVICE_ID_SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  pServer->getAdvertising()->start();
  BLEDevice::startAdvertising();
  Serial.println("Wait for the connection");
}
void disable_ble()
{
  BLEDevice::deinit(true);
}