# customS3 Board Configuration

This directory contains the custom board configuration for an ESP32-S3-based device running the Bruce firmware. This documentation reflects the exact finalized pin map and settings configured in `boards/customS3/`.

## Hardware Specifications

- **Microcontroller:** ESP32-S3-WROOM-1 N16R8
- **Flash Memory:** 16MB (Quad SPI)
- **PSRAM:** 8MB (Octal SPI / `qio_opi`)
- **Display:** 2.8" ST7789 SPI TFT (240x320)
- **Touch Controller:** XPT2046
- **Physical Interface:** Touchscreen only (No physical directional/select buttons are configured).

## Pinout & Peripherals

The board employs a shared SPI (FSPI) topology for the onboard MicroSD card and the externally available RF modules to save pins and avoid conflicts with ESP32-S3 JTAG and strapping pins.

### Display (ST7789)

*Physically Wired*

| Function | ESP32-S3 Pin |
| :--- | :--- |
| **TFT CS** | GPIO 7 |
| **TFT MOSI** | GPIO 17 |
| **TFT MISO** | GPIO 8 |
| **TFT SCLK** | GPIO 18 |
| **TFT DC** | GPIO 15 |
| **TFT RST** | GPIO 16 |
| **TFT Backlight** | GPIO 6 |

### Touch (XPT2046)

*Physically Wired*

| Function | ESP32-S3 Pin | Notes |
| :--- | :--- | :--- |
| **Touch CS** | GPIO 33 | |
| **Touch MOSI** | GPIO 17 | Shared with TFT |
| **Touch MISO** | GPIO 8 | Shared with TFT |
| **Touch SCLK** | GPIO 18 | Shared with TFT |

*(Note: Touch uses `TFT_eSPI` software implementation defined by `USE_TFT_eSPI_TOUCH` and `XPT2046_TOUCH` flags)*

### Shared FSPI Bus (SD Card & RF Modules)

A single high-performance SPI bus is shared among the MicroSD card, CC1101, and NRF24 modules.

| Function | ESP32-S3 Pin |
| :--- | :--- |
| **Shared SCK** | GPIO 12 |
| **Shared MOSI** | GPIO 11 |
| **Shared MISO** | GPIO 13 |

#### 1. Onboard MicroSD Card

*Physically Wired*

| Function | ESP32-S3 Pin | Notes |
| :--- | :--- | :--- |
| **SD CS** | GPIO 10 | Dedicated Chip Select |

#### 2. CC1101 (Sub-GHz RF)

*Optional / External Module*

| Function | ESP32-S3 Pin | Notes |
| :--- | :--- | :--- |
| **CC1101 CS** | GPIO 38 | Dedicated Chip Select |
| **CC1101 GDO0** | GPIO 9 | Interrupt / Data |
| **CC1101 GDO2** | GPIO 14 | Interrupt / Data |

#### 3. NRF24 (2.4GHz RF)

*Optional / External Module*

| Function | ESP32-S3 Pin | Notes |
| :--- | :--- | :--- |
| **NRF24 CS (SS)** | GPIO 21 | Dedicated Chip Select |
| **NRF24 CE** | GPIO 4 | Chip Enable |

### I2S Microphone

**Status:** Disabled (`-1`)

Due to pin constraints and the need to avoid ESP32-S3 JTAG pins (GPIO 39-42) and default UART pins (GPIO 43-44), the I2S microphone (e.g., INMP441) is left unmapped and completely disabled in the firmware configuration.

### USB / UART

*Physically Wired*

| Function | ESP32-S3 Pin | Notes |
| :--- | :--- | :--- |
| **USB D-** | GPIO 19 | Hardware default (USB Serial/JTAG) |
| **USB D+** | GPIO 20 | Hardware default (USB Serial/JTAG) |
| **UART0 TX** | GPIO 1 | Serial debug |
| **UART0 RX** | GPIO 2 | Serial debug |

*(Note: Hardware USB is configured to support BadUSB via the `-DUSB_as_HID=1` compiler flag)*

### I2C / Grove Port

*Optional / External*

| Function | ESP32-S3 Pin |
| :--- | :--- |
| **SDA** | GPIO 47 |
| **SCL** | GPIO 48 |

### RGB LED (WS2812B)

*Physically Wired*

| Function | ESP32-S3 Pin | Notes |
| :--- | :--- | :--- |
| **RGB Data** | GPIO 45 | 16-LED array |

### Power Management (BQ25896)

*Physically Wired*

Communicates over the I2C bus (SDA: 47, SCL: 48). Configured via `XPOWERS_CHIP_BQ25896`.

## Reserved / Unused Pins & Hardware Notes

* **GPIO 0:** Reserved (Strapping pin for BOOT).
* **GPIO 3:** Reserved (Strapping pin).
* **GPIO 46:** Reserved (Strapping pin).
* **GPIO 39, 40, 41, 42:** Reserved (Default hardware JTAG pins). Avoided to prevent erratic pull-up states at boot and conflicts with native USB/JTAG debugging.
* **GPIO 43, 44:** Reserved (UART0 default pins).
* **GPIO 33, 34:** While GPIO 33 is used for `TOUCH_CS` here, caution must be exercised on S3 modules as these pins are sometimes used internally for Octal SPI flash/PSRAM on certain chip revisions. This mapping assumes 33 is safely exposed on your specific N16R8 variant.

## Build Requirements

To build the firmware for this board, ensure the PlatformIO environment targets the `customS3` configuration:

```bash
pio run -e customS3
```

Memory requirements are strictly enforced by the following flags in `customS3.ini`:
```ini
board_build.arduino.memory_type = qio_opi
board_build.partitions = custom_16Mb.csv
```