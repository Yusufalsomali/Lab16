/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/USER/Desktop/Lab16/src/Lab16.ino"
void setup();
void loop();
#line 1 "c:/Users/USER/Desktop/Lab16/src/Lab16.ino"
SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

#include "oled-wing-adafruit.h"
OledWingAdafruit display;

const size_t UART_TX_BUF_SIZE = 20;

void onDataReceived(const uint8_t *data, size_t len, const BlePeerDevice &peer, void *context);

void onDataReceived(const uint8_t *data, size_t len, const BlePeerDevice &peer, void *context)
{
  for (size_t i = 0; i < len; i++)
  {
    Serial.write(data[i]);

    if (data[i] == '1')
    {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.print("You typed 1");
      display.display();
    }
    if (data[i] == '0')
    {
      BLE.disconnect();
    }
  }
}

const BleUuid serviceUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid rxUuid("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid txUuid("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");
BleCharacteristic txCharacteristic("tx", BleCharacteristicProperty::NOTIFY, txUuid, serviceUuid);
BleCharacteristic rxCharacteristic("rx", BleCharacteristicProperty::WRITE_WO_RSP, rxUuid, serviceUuid, onDataReceived, NULL);

void setup()
{
  Serial.begin(9600);
  BLE.on();
  BLE.addCharacteristic(txCharacteristic);
  BLE.addCharacteristic(rxCharacteristic);

  BleAdvertisingData data;
  data.appendServiceUUID(serviceUuid);
  BLE.advertise(&data);

  display.setup();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  RGB.control(true);
}

void loop()
{
  if (BLE.connected())
  {
    RGB.color(0, 0, 255);
    uint8_t txBuf[UART_TX_BUF_SIZE];
    String message = "Message from Argon \n";
    message.toCharArray((char *)txBuf, message.length() + 1);
    txCharacteristic.setValue(txBuf, message.length() + 1);
    delay(2000);
  }
  else if (!BLE.connected())
  {
    RGB.color(255, 100, 0);
  }
}