////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  File: digital.h                                                                                                     //
//  Date: 5/12/2020                                                                                                  //
//  Copyright (c) 1989-2020 Altair Engineering Inc. All Rights Reserved. Contains trade secrets of Altair Engineering Inc.          //
//  Copyright notice does not imply publication. Decompilation or disassembly of this software is strictly prohibited.//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ActDigital {
public: 
    static void init(int pin, uint8_t mode)
    {
        if (mode==0){//0 -> input
            pinMode(pin,INPUT);
        }
        else if (mode==1){//1 -> output
            pinMode(pin,OUTPUT);
        }
        else if (mode==2){//2 -> input_pullup
            pinMode(pin,INPUT_PULLUP);
            }
    };
    
    static uint8_t read(int pin)
    {
        uint8_t dgv=digitalRead(pin);
        return dgv;
    };
    
    static void write(int pin, uint8_t val)
    {
        if (val == 0) {
            digitalWrite(pin,LOW);
        } else {
            digitalWrite(pin,HIGH);
        }
    };

#ifdef ACT_SIMULATION_MODE    
    static void loop ()
    {
        int val = read_uint8();
        int pin = get_pin_number();
        switch (val) {
        case INIT_N: {//'a'-> declare pin
            uint8_t v = read_uint8();
            init(pin,v);
            break;
        }
        case READ_N: {//'r'-> read digital
            uint8_t v = read(pin);
            write_uint8(v);
            break;
        }
        case WRITE_N: {//'w'-> write digital
            uint8_t v = read_uint8();
            write(pin,v);
            break;
        }
        }
    };
#endif
};

#ifndef ACT_SIMULATION_MODE

static void arduinorpininit (const uint8_t *code) {
  int pin = code_to_pin(*code);
  ActDigital::init(pin,2);
}

static void arduinorpin (const uint8_t *code, uint8_t y[]) {
  int pin = code_to_pin(*code);
  y[0]=ActDigital::read(pin);
}

static void arduinowpininit(const uint8_t *code)
{
    int pin = code_to_pin(*code);
    ActDigital::init(pin,1);
}

static void arduinowpin (const uint8_t *code, const uint8_t u[]) {
  int pin = code_to_pin(*code);
  ActDigital::write(pin,u[0]);
}

#endif

