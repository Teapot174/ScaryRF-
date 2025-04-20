<div align="center">
  <h1>📡 ScaryRF-Plus | <a href="#ru">Русский</a> | <a href="#en">English</a></h1>
  <img src="https://github.com/user-attachments/assets/b9ef9adc-ac90-4940-b71d-091447080114" alt="ScaryRF+ Device" width="400">
</div>

<div id="ru">
  
## 🚀 О проекте ScaryRF-Plus
ScaryRF+ — мощный мультитул на базе ESP32 для работы с радиочастотами и беспроводными интерфейсами. 🔥  
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
| Модуль CC1101 | [AliExpress](https://aliexpress.ru/item/1005008544032996.html) |
| Дисплей + кнопки | [AliExpress](https://aliexpress.ru/item/1005006322355552.html) |
| Макетная плата | [AliExpress](https://aliexpress.ru/item/1005008466693134.html) |
| Провода-перемычки | [AliExpress](https://aliexpress.ru/item/1005007553381854.html) |
| *Опционально*: Аккумулятор + контроллер заряда | |

### 🔌 Схема подключения
![Схема](https://github.com/user-attachments/assets/26730497-8100-4cc1-8361-187221489662)

### 📸 Финальная сборка
![ScaryRF+ Device](https://github.com/user-attachments/assets/b9ef9adc-ac90-4940-b71d-091447080114)

</div>

<div id="en" hidden>

## 🚀 About ScaryRF-Plus
ScaryRF+ is a powerful ESP32-based multi-tool for radio frequency and wireless interfaces. 🔥  
This is an improved version of the original [ScaryRF](https://github.com/FernandoHansen666/ScaryRF-315-433mhz) project.  
*Project is under active development, some features may work unstable*

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
| CC1101 Module | [AliExpress](https://aliexpress.ru/item/1005008544032996.html) |
| Display + buttons | [AliExpress](https://aliexpress.ru/item/1005006322355552.html) |
| Breadboard | [AliExpress](https://aliexpress.ru/item/1005008466693134.html) |
| Jumper wires | [AliExpress](https://aliexpress.ru/item/1005007553381854.html) |
| *Optional*: Battery + charge controller | |

### 🔌 Connection Scheme
![Scheme](https://github.com/user-attachments/assets/26730497-8100-4cc1-8361-187221489662)

### 📸 Final Assembly
![ScaryRF+ Device](https://github.com/user-attachments/assets/b9ef9adc-ac90-4940-b71d-091447080114)

</div>

<script>
  // Simple language switcher
  document.querySelectorAll('a[href^="#"]').forEach(anchor => {
    anchor.addEventListener('click', function (e) {
      e.preventDefault();
      const lang = this.getAttribute('href').substring(1);
      document.getElementById('ru').hidden = lang !== 'ru';
      document.getElementById('en').hidden = lang !== 'en';
    });
  });
</script>
