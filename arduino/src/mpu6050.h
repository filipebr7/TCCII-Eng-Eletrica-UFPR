////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  File: mpu6050.h                                                                                                     //
//  Date: 5/12/2020                                                                                                  //
//  Copyright (c) 1989-2020 Altair Engineering Inc. All Rights Reserved. Contains trade secrets of Altair Engineering Inc.          //
//  Copyright notice does not imply publication. Decompilation or disassembly of this software is strictly prohibited.//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#include <helper_3dmath.h>
//#include <MPU6050.h>
#include <MPU6050_6Axis_MotionApps20.h>
//#include <MPU6050_9Axis_MotionApps41.h>
//#define INTERRUPT_PIN 2 
static MPU6050 accelgyro;
static int16_t quant[4];
static int16_t accel[3];
static uint8_t mpuIntStatus;
static uint8_t devStatus;
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
volatile bool _mpuDataReady=false;

class ActMPU6050
{
private:
    static void mpu6050DataReady()
    {
        _mpuDataReady=true;
    };

public:

    static void init(uint8_t interruptpin, int16_t gyroOffset[], int16_t accelOffset[])
    {
        // join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        TWBR = 24;
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
#endif
        accelgyro.initialize();
        pinMode(interruptpin, INPUT);
        accelgyro.testConnection();
        devStatus = accelgyro.dmpInitialize();
        if (devStatus == 0) {
            accelgyro.setDMPEnabled(true);
            attachInterrupt(digitalPinToInterrupt(interruptpin), mpu6050DataReady, RISING);
            mpuIntStatus = accelgyro.getIntStatus();
            packetSize = accelgyro.dmpGetFIFOPacketSize();
            //accelgyro.getMotion6(&accel[0], &accel[1], &accel[2], &gyro[0], &gyro[1], &gyro[2]);
            accelgyro.setXGyroOffset(gyroOffset[0]);
            accelgyro.setYGyroOffset(gyroOffset[1]);
            accelgyro.setZGyroOffset(gyroOffset[2]);
            accelgyro.setXAccelOffset(accelOffset[0]); 
            accelgyro.setYAccelOffset(accelOffset[1]); 
            accelgyro.setZAccelOffset(accelOffset[2]); // 1688 factory default for my test chip
        }
    };
    
    static void read(int16_t q[], int16_t a[]) {
        // read raw accel/gyro measurements from device
        uint8_t fifoBuffer[64]; // FIFO storage buffer
        if (_mpuDataReady) {
            mpuIntStatus = accelgyro.getIntStatus();
            fifoCount = accelgyro.getFIFOCount();
            if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
                // reset so we can continue cleanly
                accelgyro.resetFIFO();
            } else if (mpuIntStatus & 0x02) {
                while (fifoCount < packetSize) fifoCount = accelgyro.getFIFOCount();
                accelgyro.getFIFOBytes(fifoBuffer, packetSize);
                fifoCount -= packetSize;
                accelgyro.dmpGetQuaternion(quant,fifoBuffer);
                accelgyro.dmpGetAccel(accel, fifoBuffer);
            }
            _mpuDataReady=false;
        }
        memcpy(a,accel,3*sizeof(int16_t));
        memcpy(q,quant,4*sizeof(int16_t));
    };

    static void terminate(uint8_t interruptpin)
    {
        detachInterrupt(digitalPinToInterrupt(interruptpin));
    };

#ifdef ACT_SIMULATION_MODE
private:

    static void treat_mpu6050_init()
    {
        int16_t gyro[3];
        int16_t accel[3];
        int pin = get_pin_number();
        gyro[0] = read_uint16(); 
        gyro[1] = read_uint16(); 
        gyro[2] = read_uint16(); 
        accel[0] = read_uint16(); 
        accel[1] = read_uint16(); 
        accel[2] = read_uint16(); 
        ActMPU6050::init((uint8_t)pin,gyro,accel);
    };

    static void treat_mpu6050_run()
    {
        int16_t out[7];
        int16_t *q=out;
        int16_t *a=out+4;
        ActMPU6050::read(q,a);
        write_uint16ptr((uint16_t *)out,7);
    };

    static void treat_mpu6050_detach()
    {
        int pin = get_pin_number();
        ActMPU6050::terminate((uint8_t)pin);
    };

public:
    static void loop()
    {
        int val = read_uint8();
        switch (val) {
        case INIT_N : { 
            treat_mpu6050_init();
            break;
        }
        case TERMINATE_N : { 
            treat_mpu6050_detach();
            break;
        }
        case READ_N : { 
            treat_mpu6050_run();
            break;
        }
        }
    };
    
#endif
};

#ifndef ACT_SIMULATION_MODE
static void arduinompu6050init(const uint8_t pin[], const uint16_t gyro[], const uint16_t accel[])
{
    ActMPU6050::init(*pin,gyro,accel);
}

static void arduinompu6050read(int16_t q[], int16_t a[])
{
    ActMPU6050::read(q,a);
}

static void arduinompu6050finish(const uint8_t pin[])
{
    ActMPU6050::terminate(*pin);
}

#endif
