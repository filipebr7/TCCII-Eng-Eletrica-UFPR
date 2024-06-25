////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  File: analog.h                                                                                                     //
//  Date: 5/12/2020                                                                                                  //
//  Copyright (c) 1989-2020 Altair Engineering Inc. All Rights Reserved. Contains trade secrets of Altair Engineering Inc.          //
//  Copyright notice does not imply publication. Decompilation or disassembly of this software is strictly prohibited.//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ActAnalog
{
public:
    static uint16_t read(uint8_t pin)
    {
        return analogRead(pin);
    };

    static void write (uint8_t pin, uint8_t val)
    {
        analogWrite(pin,val);
    }
    
#ifdef ACT_SIMULATION_MODE
    static void loop ()
    {
        int val = read_uint8();
        int pin = get_pin_number();
        switch (val) {
        case READ_N: {
            uint16_t v = read(pin);
            write_uint16(v);
            break;
        }
        case WRITE_N: {
            uint8_t v = read_uint8();
            write(pin,v);
            break;
        }
        }
    };
#endif
};

#ifndef ACT_SIMULATION_MODE

static void arduinoarpin (const uint8_t *code, uint16_t y[]) {
  int pin = code_to_pin(*code);
  y[0]=ActAnalog::read(pin);
}

static void arduinoawpin (const uint8_t *code, const uint8_t u[]) {
  int pin = code_to_pin(*code);
  ActAnalog::write(pin,u[0]);
}

#endif

