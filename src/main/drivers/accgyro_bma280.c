/* 
 * This file is part of RaceFlight. 
 * 
 * RaceFlight is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation, either version 3 of the License, or 
 * (at your option) any later version. 
 * 
 * RaceFlight is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License 
 * along with RaceFlight.  If not, see <http://www.gnu.org/licenses/>.
 * You should have received a copy of the GNU General Public License 
 * along with RaceFlight.  If not, see <http://www.gnu.org/licenses/>.
 */ 
#include <stdbool.h>
#include <stdint.h>
#include <platform.h>
#include "bus_i2c.h"
#include "sensor.h"
#include "accgyro.h"
#include "accgyro_bma280.h"
#ifndef BMA280_I2C_INSTANCE
#define BMA280_I2C_INSTANCE I2C_DEVICE
#endif
#define BMA280_ADDRESS 0x18
#define BMA280_ACC_X_LSB 0x02
#define BMA280_PMU_BW 0x10
#define BMA280_PMU_RANGE 0x0F
static void bma280Init(void);
static bool bma280Read(int16_t *accelData);
bool bma280Detect(acc_t *acc)
{
    bool ack = false;
    uint8_t sig = 0;
    ack = i2cRead(BMA280_I2C_INSTANCE, BMA280_ADDRESS, 0x00, 1, &sig);
    if (!ack || sig != 0xFB)
        return false;
    acc->init = bma280Init;
    acc->read = bma280Read;
    return true;
}
static void bma280Init(void)
{
    i2cWrite(BMA280_I2C_INSTANCE, BMA280_ADDRESS, BMA280_PMU_RANGE, 0x08);
    i2cWrite(BMA280_I2C_INSTANCE, BMA280_ADDRESS, BMA280_PMU_BW, 0x0E);
    acc_1G = 512 * 8;
}
static bool bma280Read(int16_t *accelData)
{
    uint8_t buf[6];
    if (!i2cRead(BMA280_I2C_INSTANCE, BMA280_ADDRESS, BMA280_ACC_X_LSB, 6, buf)) {
        return false;
    }
    accelData[0] = (int16_t)((buf[0] >> 2) + (buf[1] << 8));
    accelData[1] = (int16_t)((buf[2] >> 2) + (buf[3] << 8));
    accelData[2] = (int16_t)((buf[4] >> 2) + (buf[5] << 8));
    return true;
}
