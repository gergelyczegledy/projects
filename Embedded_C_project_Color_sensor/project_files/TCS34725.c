#include <stdlib.h>
#include "project.h"
#include <math.h>
#include "TCS34725.h"

//init resz
tcs34725Gain_t _tcs34725Gain = TCS34725_GAIN_1X;
uint8_t _tcs34725IntegrationTime = TCS34725_INTEGRATIONTIME_614MS;

uint8_t I2C_SetRegAddrToRD(uint8_t SLA, uint8_t reg_addr)
{
    uint8_t err;
    err = I2C_MasterSendStart(SLA, 0);
    if (!err)
    {
        I2C_MasterSendStart(SLA, 0);
        I2C_MasterWriteByte(reg_addr);
    }
    I2C_MasterSendStop();
    return (err);
}

void I2C_RegReadBuf(uint8_t SLA, uint8_t reag_addr, uint8_t *buf, uint8_t count)
{
    I2C_SetRegAddrToRD(SLA, reag_addr);
    I2C_MasterClearStatus();
    I2C_MasterReadBuf(SLA, buf, count, I2C_MODE_COMPLETE_XFER);
    do
    {
    }while(!(I2C_MasterStatus()&I2C_MSTAT_RD_CMPLT));
}

/*!
 *  @brief  Reads an 8 bit value over I2C
 *  @param  reg
 *  @return value
 */
uint8_t read8(uint8_t reg)
{
    uint8_t buffer[1] = {(TCS34725_COMMAND_BIT | reg)};
    I2C_RegReadBuf(TCS34725_ADDRESS, buffer[0], &buffer[0], 1);
    return buffer[0];
}

/*!
 *  @brief  Reads a 16 bit values over I2C
 *  @param  reg
 *  @return value
 */
uint16_t read16(uint8_t reg)
{
    uint8_t buffer[2] = {(TCS34725_COMMAND_BIT | reg), 0};
    I2C_RegReadBuf(TCS34725_ADDRESS, buffer[0], &buffer[0], 2);
    return ((uint16_t)buffer[1] << 8) | ((uint16_t)buffer[0] & 0xFF);
}

uint8_t I2C_RegWrite(uint8_t SLA, uint8_t reg_addr, uint8_t data)
{
    volatile uint8_t err;
    err = I2C_MasterSendStart(SLA, 0);
    if (!err)
    {
        I2C_MasterWriteByte(reg_addr);
        I2C_MasterWriteByte(data);
    }
    I2C_MasterSendStop();
    return (err);
}

/*!
 *  @brief  Writes a register and an 8 bit value over I2C
 *  @param  reg
 *  @param  value
 */
void write8(uint8_t reg, uint8_t value)
{
    uint8_t buffer[2] = {(uint8_t)(TCS34725_COMMAND_BIT | reg), value};
    I2C_RegWrite(TCS34725_ADDRESS, buffer[0], value);
}

/*!
 *  @brief  Sets the integration time for the TC34725
 *  @param  it
 *          Integration Time
 */
void setIntegrationTime(uint8_t it)
{
    /* Update the timing register */
    write8(TCS34725_ATIME, it);

    /* Update value placeholders */
    _tcs34725IntegrationTime = it;
}

/*!
 *  @brief  Adjusts the gain on the TCS34725
 *  @param  gain
 *          Gain (sensitivity to light)
 */
void setGain(tcs34725Gain_t gain)
{
    /* Update the timing register */
    write8(TCS34725_CONTROL, gain);

    /* Update value placeholders */
    _tcs34725Gain = gain;
}

/*!
 *  @brief  Enables the device
 */
void enable()
{
    write8(TCS34725_ENABLE, TCS34725_ENABLE_PON);
    CyDelay(3);
    write8(TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);
    /* Set a delay for the integration time.
      This is only necessary in the case where enabling and then
      immediately trying to read values back. This is because setting
      AEN triggers an automatic integration, so if a read RGBC is
      performed too quickly, the data is not yet valid and all 0's are
      returned */
    /* 12/5 = 2.4, add 1 to account for integer truncation */
    CyDelay((256 - _tcs34725IntegrationTime) * 12 / 5 + 1);
}

/*!
 *  @return True if initialization was successful, otherwise false.
 */
_Bool init_TCS34725()
{
    /* Make sure we're actually connected */
    uint8_t x = read8(TCS34725_ID);
    if ((x != 0x4d) && (x != 0x44) && (x != 0x10))
    {
        return 0;
    }

    /* Set default integration time and gain */
    setIntegrationTime(_tcs34725IntegrationTime);
    setGain(_tcs34725Gain);

    /* Note: by default, the device is in power down mode on bootup */
    enable();

    return 1;
}
//

//loop resz

void getRawData(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c)
{
    *c = read16(TCS34725_CDATAL);
    *r = read16(TCS34725_RDATAL);
    *g = read16(TCS34725_GDATAL);
    *b = read16(TCS34725_BDATAL);

    /* Set a delay for the integration time */
    /* 12/5 = 2.4, add 1 to account for integer truncation */
    CyDelay((256 - _tcs34725IntegrationTime) * 12 / 5 + 1);
}

/*!
 *  @brief  Read the RGB color detected by the sensor.
 *  @param  *r
 *          Red value normalized to 0-255
 *  @param  *g
 *          Green value normalized to 0-255
 *  @param  *b
 *          Blue value normalized to 0-255
 */
void getRGB(float *r, float *g, float *b) {
    uint16_t red, green, blue, clear;
    getRawData(&red, &green, &blue, &clear);
    uint32_t sum = clear;

    // Avoid divide by zero errors ... if clear = 0 return black
    if (clear == 0) {
    *r = *g = *b = 0;
    return;
    }

    *r = ((float)red / (float)sum) * 255.0;
    *g = ((float)green / (float)sum) * 255.0;
    *b = ((float)blue / (float)sum) * 255.0;
}

/*!
 *  @brief  Disables the device (putting it in lower power sleep mode)
 */
void disable() {
  /* Turn the device off to save power */
  uint8_t reg = 0;
  reg = read8(TCS34725_ENABLE);
  write8(TCS34725_ENABLE, reg & ~(TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN));
}

//

/* [] END OF FILE */

