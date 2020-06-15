
// HIH6130 

//     uint8_t _address;
//     uint8_t _humidity_lo, _humidity_hi;
//     uint8_t _temp_lo, _temp_hi;
//     uint8_t _status;


//     ________________________________________________________________________________________________________________

//     Wire.beginTransmission(_address);
// 	Wire.endTransmission();

// 	Wire.requestFrom( (int) _address, (int) 4);
// 	while (Wire.available() == 0);

// 	_humidity_hi = Wire.read();
// 	_humidity_lo = Wire.read();
// 	_temp_hi = Wire.read();
// 	_temp_lo = Wire.read();

// 	Wire.endTransmission();

// 	// Get the status (first two bits of _humidity_hi_)
// 	_status = (_humidity_hi >> 6);

// 	// Calculate Relative Humidity
// 	humidity = (double)(((unsigned int) (_humidity_hi & 0x3f) << 8) | _humidity_lo) * 100 / (pow(2,14) - 1);

//     //Simlper:

//     	humidity = (double)(((unsigned int) (_humidity_hi & 0x3f) << 8) 

// 	// Calculate Temperature
// 	temperature = (double) (((unsigned int) (_temp_hi << 6) + (_temp_lo >> 2)) / (pow(2, 14) - 1) * 165 - 40);

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// LPS331

//     #define LPS331_SA0_LOW  0
//     #define LPS331_SA0_HIGH 1
//     #define LPS331_SA0_AUTO 2

//     // register addresses
//     // Note: Some of the register names in the datasheet are inconsistent
//     // between Table 14 in section 6 and the register descriptions in
//     // section 7. Where they differ, the names from section 7 have been
//     // used here.

//     #define LPS331_REF_P_XL       0x08
//     #define LPS331_REF_P_L        0x09
//     #define LPS331_REF_P_H        0x0A

//     #define LPS331_WHO_AM_I       0x0F

//     #define LPS331_RES_CONF       0x10

//     #define LPS331_CTRL_REG1      0x20
//     #define LPS331_CTRL_REG2      0x21
//     #define LPS331_CTRL_REG3      0x22
//     #define LPS331_INTERRUPT_CFG  0x23
//     #define LPS331_INT_SOURCE     0x24
//     #define LPS331_THS_P_L        0x25
//     #define LPS331_THS_P_H        0x26
//     #define LPS331_STATUS_REG     0x27

//     #define LPS331_PRESS_OUT_XL   0x28
//     #define LPS331_PRESS_OUT_L    0x29
//     #define LPS331_PRESS_OUT_H    0x2A

//     #define LPS331_TEMP_OUT_L     0x2B
//     #define LPS331_TEMP_OUT_H     0x2C

//     #define LPS331_AMP_CTRL       0x30

//     #define LPS331_DELTA_PRESS_XL 0x3C
//     #define LPS331_DELTA_PRESS_L  0x3D
//     #define LPS331_DELTA_PRESS_H  0x3E

//     class LPS331
//     {
//     public:
//         LPS331(void);

//         bool init(byte sa0 = LPS331_SA0_AUTO);

//         void enableDefault(void);

//         void writeReg(byte reg, byte value);
//         byte readReg(byte reg);

//         float readPressureMillibars(void);
//         float readPressureInchesHg(void);
//         int32_t readPressureRaw(void);
//         float readTemperatureC(void);
//         float readTemperatureF(void);
//         int16_t readTemperatureRaw(void);

//         static float pressureToAltitudeMeters(float pressure_mbar, float altimeter_setting_mbar = 1013.25);
//         static float pressureToAltitudeFeet(float pressure_inHg, float altimeter_setting_inHg = 29.9213);

//     private:
//         byte address;

//         bool autoDetectAddress(void);
//         bool testWhoAmI(void);
//     };

// ________________________________________________________________________________________________________________

//     #define LPS331AP_ADDRESS_SA0_LOW  0b1011100
//     #define LPS331AP_ADDRESS_SA0_HIGH 0b1011101

//     // Constructors //////////////////////////////////////////////////////

//     LPS331::LPS331(void)
//     {
//     // Pololu board pulls SA0 high, so default assumption is that it is
//     // high
//     address = LPS331AP_ADDRESS_SA0_HIGH;
//     }

//     // Public Methods ////////////////////////////////////////////////////

//     // sets or detects slave address; returns bool indicating success
//     bool LPS331::init(byte sa0)
//     {
//     switch(sa0)
//     {
//         case LPS331_SA0_LOW:
//         address = LPS331AP_ADDRESS_SA0_LOW;
//         return testWhoAmI();

//         case LPS331_SA0_HIGH:
//         address = LPS331AP_ADDRESS_SA0_HIGH;
//         return testWhoAmI();

//         default:
//         return autoDetectAddress();
//     }
//     }

//     // turns on sensor and enables continuous output
//     void LPS331::enableDefault(void)
//     {
//     // active mode, 12.5 Hz output data rate
//     writeReg(LPS331_CTRL_REG1, 0b11100000);
//     }

//     // writes register
//     void LPS331::writeReg(byte reg, byte value)
//     {
//     Wire.beginTransmission(address);
//     Wire.write(reg);
//     Wire.write(value);
//     Wire.endTransmission();
//     }

//     // reads register
//     byte LPS331::readReg(byte reg)
//     {
//     byte value;

//     Wire.beginTransmission(address);
//     Wire.write(reg);
//     Wire.endTransmission(false); // restart
//     Wire.requestFrom(address, (byte)1);
//     value = Wire.read();
//     Wire.endTransmission();

//     return value;
//     }

//     // reads pressure in millibars (mbar)/hectopascals (hPa)
//     float LPS331::readPressureMillibars(void)
//     {
//     return (float)readPressureRaw() / 4096;
//     }

//     // reads pressure in inches of mercury (inHg)
//     float LPS331::readPressureInchesHg(void)
//     {
//     return (float)readPressureRaw() / 138706.5;
// //     }

// //     // reads pressure and returns raw 24-bit sensor output
//     int32_t LPS331::readPressureRaw(void)
//     {
//     Wire.beginTransmission(address);
//     // assert MSB to enable register address auto-increment
//     Wire.write(LPS331_PRESS_OUT_XL | (1 << 7));
//     Wire.endTransmission();
//     Wire.requestFrom(address, (byte)3);

//     while (Wire.available() < 3);

//     uint8_t pxl = Wire.read();
//     uint8_t pl = Wire.read();
//     uint8_t ph = Wire.read();

//     // combine bytes
//     return (int32_t)(int8_t)ph << 16 | (uint16_t)pl << 8 | pxl;
//     }

//     // reads temperature in degrees C
//     float LPS331::readTemperatureC(void)
//     {
//     return 42.5 + (float)readTemperatureRaw() / 480;
//     }

//     // reads temperature in degrees F
//     float LPS331::readTemperatureF(void)
//     {
//     return 108.5 + (float)readTemperatureRaw() / 480 * 1.8;
//     }

//     // reads temperature and returns raw 16-bit sensor output
//     int16_t LPS331::readTemperatureRaw(void)
//     {
//     Wire.beginTransmission(address);
//     // assert MSB to enable register address auto-increment
//     Wire.write(LPS331_TEMP_OUT_L | (1 << 7));
//     Wire.endTransmission();
//     Wire.requestFrom(address, (byte)2);

//     while (Wire.available() < 2);

//     uint8_t tl = Wire.read();
//     uint8_t th = Wire.read();

//     // combine bytes
//     return (int16_t)(th << 8 | tl);
//     }

//     // converts pressure in mbar to altitude in meters, using 1976 US
//     // Standard Atmosphere model (note that this formula only applies to a
//     // height of 11 km, or about 36000 ft)
//     //  If altimeter setting (QNH, barometric pressure adjusted to sea
//     //  level) is given, this function returns an indicated altitude
//     //  compensated for actual regional pressure; otherwise, it returns
//     //  the pressure altitude above the standard pressure level of 1013.25
//     //  mbar or 29.9213 inHg
//     float LPS331::pressureToAltitudeMeters(float pressure_mbar, float altimeter_setting_mbar)
//     {
//     return (1 - pow(pressure_mbar / altimeter_setting_mbar, 0.190263)) * 44330.8;
//     }

//     // converts pressure in inHg to altitude in feet; see notes above
//     float LPS331::pressureToAltitudeFeet(float pressure_inHg, float altimeter_setting_inHg)
//     {
//     return (1 - pow(pressure_inHg / altimeter_setting_inHg, 0.190263)) * 145442;
//     }

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++