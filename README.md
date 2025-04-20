<div align="left">
  <h1>📡 ScaryRF-Plus   <a href="#ru">Русский</a> | <a href="#en">English</a></h1>

![ScaryRF-Plus_Board](https://github.com/user-attachments/assets/baec7610-9770-4be8-9382-811defc3bb06)

</div>

<div id="ru">
  
## 🚀 О проекте ScaryRF+
ScaryRF+ —  крутой проект который дает возможность показать на что способна ESP32🔥  
Это улучшенная версия оригинального проекта [ScaryRF](https://github.com/FernandoHansen666/ScaryRF-315-433mhz).  
*Проект в активной разработке, некоторые функции могут работать нестабильно*

## ⚡ Возможности
### 📶 SubGHz (315/433 МГц)
- 🎭 Копирование сигналов (в разработке)  
- 📊 Спектральный анализ  
- 🔍 Анализатор сигналов  
- 🎲 Генератор случайных сигналов  

### 📡 WiFi
- 🕸️ EvilPortal (в разработке)  
- 📶 Деаутентификация (частично работает)  
- 💣 WiFi-Спам  

### 📱 Bluetooth
- 🔄 BLE Spam  

## 🛠️ Сборка
### 🔧 Необходимые компоненты
| Компонент | Ссылка |
|-----------|--------|
| ESP32-WROOM | [AliExpress](https://aliexpress.ru/item/1005004605399313.html) |
| CC1101-Модуль | [AliExpress](https://aliexpress.ru/item/1005008544032996.html) |
| Дисплей&кнопки | [AliExpress](https://aliexpress.ru/item/1005006322355552.html) |
| Макетная плата | [AliExpress](https://aliexpress.ru/item/1005008466693134.html) |
| Провода-перемычки | [AliExpress](https://aliexpress.ru/item/1005007553381854.html) |
| *Необязательно*: Аккумулятор + контроллер заряда | |

### 🔌 Схема подключения
![Схема](https://github.com/user-attachments/assets/26730497-8100-4cc1-8361-187221489662)
|Module|Pin 1|Pin 2|Pin 3|Pin 4|Pin 5|Pin 6| Pin 7|
|--------|--------|--------|--------|--------|--------|--------|--------|
|**📺 Display**|VCC → 3V3|GND → GND|SCL → G22|SDA → G21|-|-|-|
|**🔘 Buttons**|K1 → G27|K2 → G26|K3 → G33|K4 → G32|-|-|-|
|**📡 CC1101**|1 → GND|2 → 3V3|3 → G2|4 → G5|5 → G18|6 → G23 |7 → G19|

### 📸 Финальный результат
![ScaryRF+ Device](https://github.com/user-attachments/assets/b9ef9adc-ac90-4940-b71d-091447080114)

</div>

<div id="en" hidden>

## 🚀 About ScaryRF+
ScaryRF+ is a cool project that gives you the opportunity to show what ESP32 is capable of🔥 
This is an improved version of the original [ScaryRF](https://github.com/FernandoHansen666/ScaryRF-315-433mhz). 
*The project is under active development, some functions may be unstable*

## ⚡ Features
### 📶 SubGHz (315/433 MHz)
- 🎭 Signal copying (in development)  
- 📊 Spectrum analysis  
- 🔍 Signal analyzer  
- 🎲 Random signal generator  

### 📡 WiFi
- 🕸️ EvilPortal (in development)  
- 📶 Deauthentication (partially working)  
- 💣 WiFi Spam  

### 📱 Bluetooth
- 🔄 BLE Spam  

## 🛠️ Building
### 🔧 Required Components
| Component | Link |
|-----------|------|
| ESP32-WROOM | [AliExpress](https://aliexpress.ru/item/1005004605399313.html) |
| CC1101-Module | [AliExpress](https://aliexpress.ru/item/1005008544032996.html) |
| Display&buttons | [AliExpress](https://aliexpress.ru/item/1005006322355552.html) |
| Breadboard | [AliExpress](https://aliexpress.ru/item/1005008466693134.html) |
| Jumper wires | [AliExpress](https://aliexpress.ru/item/1005007553381854.html) |
| *Optional*: Battery + charge controller | |

### 🔌 Connection Scheme
![Scheme](https://github.com/user-attachments/assets/26730497-8100-4cc1-8361-187221489662)
|Module|Pin 1|Pin 2|Pin 3|Pin 4|Pin 5|Pin 6| Pin 7|
|--------|--------|--------|--------|--------|--------|--------|--------|
|**📺 Display**|VCC → 3V3|GND → GND|SCL → G22|SDA → G21|-|-|-|
|**🔘 Buttons**|K1 → G27|K2 → G26|K3 → G33|K4 → G32|-|-|-|
|**📡 CC1101**|1 → GND|2 → 3V3|3 → G2|4 → G5|5 → G18|6 → G23 |7 → G19|

### 📸 The final result
![ScaryRF+ Device](https://github.com/user-attachments/assets/b9ef9adc-ac90-4940-b71d-091447080114)

</div>
