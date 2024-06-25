////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  File: servo.h                                                                                                     //
//  Date: 5/12/2020                                                                                                  //
//  Copyright (c) 1989-2020 Altair Engineering Inc. All Rights Reserved. Contains trade secrets of Altair Engineering Inc.          //
//  Copyright notice does not imply publication. Decompilation or disassembly of this software is strictly prohibited.//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Servo.h>

/* create and initialize servos                              */


class XServo : public Servo
{
public:
    int pin;
};

#define MAX_SERVO_NB NUM_DIGITAL_PINS

XServo servos[MAX_SERVO_NB];

class ActServo
{
public:
    static void init(int pin)
        {
            int i=0;
            for (int j = 0; j < MAX_SERVO_NB; j++) {
                if (servos[j].pin == pin && servos[j].attached())
                    return;
            }
            while (i < MAX_SERVO_NB && servos[i].attached()) {
                i++;
            }
            if (i < MAX_SERVO_NB) {
                servos[i].attach(pin);
                servos[i].pin=pin;
                servos[i].write(0);
            }
        };

    static void terminate(int pin)
        {
            for (int i=0; i < MAX_SERVO_NB; i++) {
                if (servos[i].pin == pin) {
                    if (servos[i].attached()) {
                        servos[i].detach();
                    }
                    servos[i].pin=0;
                    break;
                }
            }
        };

    static void write(int pin, int val)
        {
            //val=min(max(val,0),180);
            for (int i=0; i < MAX_SERVO_NB; i++) {
                if (servos[i].pin == pin) {
                    servos[i].write(val);
                    break;
                }
            }
        };

#ifdef ACT_SIMULATION_MODE
    static void loop()
        {
            uint8_t val = read_uint8();
            int pin = get_pin_number();
            switch (val) {
            case INIT_N: {
                init(pin);
                break;
            }
            case TERMINATE_N: {
                terminate(pin);
                break;
            }
            case WRITE_N: {
                uint8_t val = read_uint8();
                write(pin,val);
                break;
            }
            }
        };
#endif
};


#ifndef ACT_SIMULATION_MODE

static void arduinoservomotorinit (const uint8_t *code) {
  int pin = code_to_pin(*code);
  ActServo::init(pin);
}

static void arduinoservomotor (const uint8_t *code, const uint8_t *u) {
  int val = *u;
  int pin = code_to_pin(*code);
  ActServo::write(pin,val);
}

#endif
