#include <Arduino.h>
#include <bluefruit.h>

// For testing with this sketch, you can use nRF Beacon app
#define MANUFACTURER_ID   0xffff

const char ARTIFACT_NAME[] = "ARTIFACT";
const int TX_POWER_DBM = 4;            // 0..8 typical; test for battery life/range
const uint16_t ADV_INTERVAL_MS = 300;

const int LED_PIN = PIN_015;

// "nRF Connect" app can be used to detect beacon
uint8_t beaconUuid[16] = {
  0xff, 0xff, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78,
  0x89, 0x9a, 0xab, 0xbc, 0xcd, 0xde, 0xef, 0xff
};

// A valid Beacon packet consists of the following information:
// UUID, Major, Minor, RSSI @ 1M
BLEBeacon beacon(beaconUuid, 1, 2, -54);


void startAdv(void)
{  
  Bluefruit.Advertising.setBeacon(beacon);

  Bluefruit.ScanResponse.addName();
  
  Bluefruit.Advertising.setType(BLE_GAP_ADV_TYPE_NONCONNECTABLE_SCANNABLE_UNDIRECTED);
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(160, 80);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  
}

void setup() {
  pinMode(PIN_015, OUTPUT);


  Bluefruit.begin();
  Bluefruit.setTxPower(TX_POWER_DBM);
  Bluefruit.setName(ARTIFACT_NAME);

  // Manufacturer ID is required for Manufacturer Specific Data
  beacon.setManufacturer(MANUFACTURER_ID);

  startAdv();
  digitalWrite(PIN_015, HIGH);

  suspendLoop();
}



void loop() {
              // n o
}

// python .\Adafruit_nRF52_Bootloader\lib\uf2\utils\uf2conv.py .\Arftefact\.pio\build\nicenano\firmware.hex -c -f 0xADA52840