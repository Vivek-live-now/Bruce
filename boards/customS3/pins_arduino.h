#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include "soc/soc_caps.h"
#include <stdint.h>

static const uint8_t TX = 1;
static const uint8_t RX = 2;

static const uint8_t SDA = 47;
static const uint8_t SCL = 48;

// Modified elsewhere
static const uint8_t SS = 3;
static const uint8_t MOSI = 17;
static const uint8_t MISO = 8;
static const uint8_t SCK = 18;

#define SERIAL_RX 2
#define SERIAL_TX 1
#define BAD_RX SERIAL_RX
#define BAD_TX SERIAL_TX
#define GPS_SERIAL_TX SERIAL_TX
#define GPS_SERIAL_RX SERIAL_RX
#define USB_as_HID 1

#define TXLED 5
#define LED_ON HIGH
#define LED_OFF LOW

#define USE_CC1101_VIA_SPI
#define CC1101_GDO0_PIN 9
#define CC1101_GDO2_PIN 14
#define CC1101_SS_PIN 38
#define CC1101_MOSI_PIN 11
#define CC1101_SCK_PIN 12
#define CC1101_MISO_PIN 13

#define USE_NRF24_VIA_SPI
#define NRF24_CE_PIN 4
#define NRF24_SS_PIN 21
#define NRF24_MOSI_PIN 11
#define NRF24_SCK_PIN 12
#define NRF24_MISO_PIN 13

#define FP 1
#define FM 2
#define FG 3

#define HAS_SCREEN 1
#define ROTATION 1
#define MINBRIGHT (uint8_t)1

#define USER_SETUP_LOADED 1
#define ST7789_DRIVER 1
#define TFT_RGB_ORDER 0
#define TFT_WIDTH 240
#define TFT_HEIGHT 320
#define TFT_BACKLIGHT_ON 1
#define TFT_BL 6
#define TFT_RST 16
#define TFT_DC 15
#define TFT_MISO 8
#define TFT_MOSI 17
#define TFT_SCLK 18
#define TFT_CS 7
#define USE_TFT_eSPI_TOUCH 1
#define XPT2046_TOUCH 1
#define TOUCH_CS 33
#define TOUCH_MOSI TFT_MOSI
#define TOUCH_MISO TFT_MISO
#define TOUCH_CLK TFT_SCLK
#define SMOOTH_FONT 1
#define SPI_FREQUENCY 20000000
#define SPI_READ_FREQUENCY 20000000
#define SPI_TOUCH_FREQUENCY 2500000

#define SDCARD_CS 10
#define SDCARD_SCK 12
#define SDCARD_MISO 13
#define SDCARD_MOSI 11

#define GROVE_SDA 47
#define GROVE_SCL 48
#define SYS_I2C_SDA 47
#define SYS_I2C_SCL 48

// Secondary SPI bus removed/reassigned

// RGB LED

#define HAS_RGB_LED 1
#define RGB_LED 45
#define LED_TYPE WS2812B
#define LED_ORDER GRB
#define LED_TYPE_IS_RGBW 0
#define LED_COUNT 16

#define LED_COLOR_STEP 15

#define XPOWERS_CHIP_BQ25896

// USE BOOST ENABLE PMIC 5V OUTPUT
#define USE_BOOST

// Microphone removed

// IO EXPANDER
// Removed due to conflict / not requested
#endif /* Pins_Arduino_h */
#define SPI_SS_PIN -1
#define SPI_MOSI_PIN 11
#define SPI_MISO_PIN 13
#define SPI_SCK_PIN 12
#define PIN_WS -1
#define PIN_CLK -1
#define PIN_DATA -1
