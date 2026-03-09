# ESP32-C3 BLE Page Turner

A high-performance, Bluetooth Low Energy (BLE) Page Turner built with **ESP32-C3 SuperMini**. This project allows you to flip pages in PDF readers(like Flexcil) or sheet music apps (like Piascore, forScore) using physical buttons.


## Key Features

- **Advanced Security**: Implements **Passkey Authentication** (6-digit PIN) to prevent unauthorized access and ensure a stable connection with iPadOS/macOS.
- **Precision Control**: Built-in **Hardware Debouncing** and **Auto-Repeat** logic for a smooth user experience.
- **Customizable Shortcuts**: Supports complex key combinations like `Control + Arrow` or `Command + Arrow` to support various apps.
- **NimBLE Stack**: Optimized for ESP32-C3 using the NimBLE-Arduino library for lower memory footprint and better stability.

## 🛠 Hardware Requirements

- **MCU**: ESP32-C3 (SuperMini or DevKit) [Purchase Here (AliExpress)](https://ko.aliexpress.com/item/1005007539612437.html?spm=a2g0o.order_list.order_list_main.152.393b1802yaPrSu&gatewayAdapt=glo2kor)
- **DC-DC Converter**: DC-DC Buck Converter (5V~30V to 5V) [Purchase Here (AliExpress)](https://ko.aliexpress.com/item/1005008257960729.html?spm=a2g0o.order_list.order_list_main.262.393b1802yaPrSu&gatewayAdapt=glo2kor)
- **Function Buttons**: 2x Tactile Push Buttons [Purchase Here (AliExpress)](https://ko.aliexpress.com/item/32873378875.html?spm=a2g0o.order_list.order_list_main.281.393b1802yaPrSu&gatewayAdapt=glo2kor)
- **Enclosure**: Aluminum Stomp Box Enclosure 1590A [Purchase Here (AliExpress)](https://ko.aliexpress.com/item/1005009010223615.html?spm=a2g0o.order_list.order_list_main.252.393b1802yaPrSu&gatewayAdapt=glo2kor)
- **PowerButton**: Push Button Switch Momentary Locking(Blue-Flat-Ring, 3-6V, Latching) [Purchase Here (AliExpress)](https://ko.aliexpress.com/item/4000034288190.html?spm=a2g0o.order_list.order_list_main.5.393b1802yaPrSu&gatewayAdapt=glo2kor)


## 🚀 Getting Started

### Prerequisites
- [PlatformIO](https://platformio.org/) (recommended) or Arduino IDE.
- Libraries:
  - `NimBLE-Arduino`
  - `ESP32-BLE-Keyboard` (NimBLE compatible version)

### Configuration (platformio.ini)
Ensure your `platformio.ini` includes the following build flags for security and USB CDC support:

```ini
build_flags = 
    -D ARDUINO_USB_MODE=1
    -D ARDUINO_USB_CDC_ON_BOOT=1
    -D CONFIG_BT_NIMBLE_MAX_BONDINGS=5
    -D USE_NIMBLE