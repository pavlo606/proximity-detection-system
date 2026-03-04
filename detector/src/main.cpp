#include <bluefruit.h>

#define RESET_DELAY_TIMEOUT 2000

const float maxRSSI = -35.0f;
const float minRSSI = -90.0f;
const float maxDelay = 2000.0f;
const float minDelay = 50.0f;

uint8_t beaconUuid[16] = {
    0xff, 0xff, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78,
    0x89, 0x9a, 0xab, 0xbc, 0xcd, 0xde, 0xef, 0xff};

void scan_callback(ble_gap_evt_adv_report_t *report);

int blink_interval_ms = 0;
uint64_t timer = 0;

void setup()
{
  pinMode(PIN_017, OUTPUT);
  pinMode(PIN_020, OUTPUT);

  Bluefruit.begin(0, 1);
  Bluefruit.setTxPower(4);
  Bluefruit.setName("Bluefruit52");

  Bluefruit.setConnLedInterval(250);
  Bluefruit.Scanner.setRxCallback(scan_callback);
  Bluefruit.Scanner.start(0);

  tone(PIN_020, 2109, 100);
}

int update_blink_from_rssi(int rssi)
{
  const int RSSI_MIN = -100;
  const int RSSI_MAX = -35;
  const int DELAY_MIN = 50;
  const int DELAY_MAX = 1000;

  if (rssi < RSSI_MIN)
    rssi = RSSI_MIN;
  if (rssi > RSSI_MAX)
    rssi = RSSI_MAX;

  float t = float(rssi - RSSI_MIN) / float(RSSI_MAX - RSSI_MIN);
  float delay_f = DELAY_MAX - t * (DELAY_MAX - DELAY_MIN);
  blink_interval_ms = int(delay_f + 0.5f);

  return blink_interval_ms;
}

void scan_callback(ble_gap_evt_adv_report_t *report)
{
  int uuidOffset = 9;

  if (report->data.len >= uuidOffset + 16 &&
      memcmp(report->data.p_data + uuidOffset, beaconUuid, 16) == 0)
  {
    update_blink_from_rssi(report->rssi);
    timer = millis();
  }

  Bluefruit.Scanner.resume();
}

void loop()
{
  if (millis() - timer < RESET_DELAY_TIMEOUT)
  {
    tone(PIN_020, 2109, 90);
    digitalWrite(PIN_017, HIGH);
    delay(blink_interval_ms);
    digitalWrite(PIN_017, LOW);
    delay(blink_interval_ms);
  } else {
    digitalWrite(PIN_017, LOW);
  }
}