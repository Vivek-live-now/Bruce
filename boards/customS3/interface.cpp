#include "core/bus_HAL.h"
#include "core/powerSave.h"
#include "core/utils.h"

/***************************************************************************************
** Function name: _setup_gpio()
** Location: main.cpp
** Description:   initial setup for the device
***************************************************************************************/

// Power handler for battery detection
#ifdef XPOWERS_CHIP_BQ25896
#include <Wire.h>
#include <XPowersLib.h>
XPowersPPM PPM;
#endif

void _setup_gpio() {

    pinMode(TFT_CS, OUTPUT);
    digitalWrite(TFT_CS, HIGH);
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    pinMode(CC1101_SS_PIN, OUTPUT);
    pinMode(NRF24_SS_PIN, OUTPUT);

    digitalWrite(CC1101_SS_PIN, HIGH);
    digitalWrite(NRF24_SS_PIN, HIGH);
    // Starts SPI instance for CC1101 and NRF24 with CS pins blocking communication at start

    bruceConfigPins.rfModule = CC1101_SPI_MODULE;
    bruceConfigPins.irRx = -1; // RXLED was repurposed
    setSysI2CBus(&Wire); // PMU lives on the default Wire object
    Wire.setPins(SYS_I2C_SDA, SYS_I2C_SCL);
    // Wire.begin();
    bool pmu_ret = false;
    Wire.begin(SYS_I2C_SDA, SYS_I2C_SCL);
    pmu_ret = PPM.init(Wire, SYS_I2C_SDA, SYS_I2C_SCL, BQ25896_SLAVE_ADDRESS);
    if (pmu_ret) {
        PPM.setSysPowerDownVoltage(3300);
        PPM.setInputCurrentLimit(3250);
        Serial.printf("getInputCurrentLimit: %d mA\n", PPM.getInputCurrentLimit());
        PPM.disableCurrentLimitPin();
        PPM.setChargeTargetVoltage(4208);
        PPM.setPrechargeCurr(64);
        PPM.setChargerConstantCurr(832);
        PPM.getChargerConstantCurr();
        Serial.printf("getChargerConstantCurr: %d mA\n", PPM.getChargerConstantCurr());
        PPM.enableMeasure(PowersBQ25896::CONTINUOUS);
        PPM.disableOTG();
        PPM.enableCharge();
    }
}
bool isCharging() {
    // PPM.disableBatterPowerPath();
    return PPM.isCharging();
}

int getBattery() {
    int voltage = PPM.getBattVoltage();
    int percent = (voltage - 3300) * 100 / (float)(4150 - 3350);

    if (percent < 0) return 1;
    if (percent > 100) percent = 100;

    if (PPM.isCharging() && percent >= 97) {
        PPM.disableBatLoad();
        percent = 95; // estimate still charging
    }

    if (PPM.isChargeDone()) { percent = 100; }

    return percent;
}

/*********************************************************************
** Function: setBrightness
** location: settings.cpp
** set brightness value
**********************************************************************/
void _setBrightness(uint8_t brightval) {
    if (brightval == 0) {
        analogWrite(TFT_BL, brightval);
    } else {
        int bl = MINBRIGHT + round(((255 - MINBRIGHT) * brightval / 100));
        analogWrite(TFT_BL, bl);
    }
}

/***************************************************************************************
** Function name: _post_setup_gpio()
** Location: main.cpp
** Description:   second stage gpio setup to make a few functions work
***************************************************************************************/
void _post_setup_gpio() {
#ifdef HAS_TOUCH
    pinMode(TOUCH_CS, OUTPUT);
    uint16_t calData[5];
    File caldata = LittleFS.open("/calData", "r");

    if (!caldata) {
        tft.setRotation(ROTATION);
        tft.calibrateTouch(calData, TFT_WHITE, TFT_BLACK, 10);

        caldata = LittleFS.open("/calData", "w");
        if (caldata) {
            caldata.printf(
                "%d\n%d\n%d\n%d\n%d\n", calData[0], calData[1], calData[2], calData[3], calData[4]
            );
            caldata.close();
        }
    } else {
        Serial.print("\ntft Calibration data: ");
        for (int i = 0; i < 5; i++) {
            String line = caldata.readStringUntil('\n');
            calData[i] = line.toInt();
            Serial.printf("%d, ", calData[i]);
        }
        Serial.println();
        caldata.close();
    }
    tft.setTouch(calData);
#endif
}

/*********************************************************************
** Function: InputHandler
** Handles the variables PrevPress, NextPress, SelPress, AnyKeyPress and EscPress
**********************************************************************/
void InputHandler(void) {
    static unsigned long tm = 0;
    if (millis() - tm < 200 && !LongPress) return;
#ifdef HAS_TOUCH
    TouchPoint t;
    checkPowerSaveTime();
    bool _IH_touched = tft.getTouch(&t.x, &t.y);
    if (_IH_touched) {
        NextPress = false;
        PrevPress = false;
        UpPress = false;
        DownPress = false;
        SelPress = false;
        EscPress = false;
        AnyKeyPress = false;
        NextPagePress = false;
        PrevPagePress = false;
        touchPoint.pressed = false;
        _IH_touched = false;

        if (bruceConfigPins.rotation == 3) {
            t.y = (tftHeight + 20) - t.y;
            t.x = tftWidth - t.x;
        }
        if (bruceConfigPins.rotation == 0) {
            uint16_t tmp = t.x;
            t.x = map((tftHeight + 20) - t.y, 0, 320, 0, 240);
            t.y = map(tmp, 0, 240, 0, 320);
        }
        if (bruceConfigPins.rotation == 2) {
            uint16_t tmp = t.x;
            t.x = map(t.y, 0, 320, 0, 240);
            t.y = map(tftWidth - tmp, 0, 240, 0, 320);
        }

        if (!wakeUpScreen()) AnyKeyPress = true;
        else return;

        // Touch point global variable
        touchPoint.x = t.x;
        touchPoint.y = t.y;
        touchPoint.pressed = true;
        touchHeatMap(touchPoint);
        tm = millis();
    }
#endif
}

/*********************************************************************
** Function: powerOff
** location: mykeyboard.cpp
** Turns off the device (or try to)
**********************************************************************/
void powerOff() {
    esp_deep_sleep_start();
}

/*********************************************************************
** Function: checkReboot
** location: mykeyboard.cpp
** Btn logic to turn off the device (name is odd btw)
**********************************************************************/
void checkReboot() {
    // No physical buttons to trigger reboot
}
