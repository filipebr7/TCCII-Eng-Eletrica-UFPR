////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  File: rotaryencoder.h                                                                                                     //
//  Date: 10/15/2020                                                                                                  //
//  Copyright (c) 1989-2021 Altair Engineering Inc. All Rights Reserved. Contains trade secrets of Altair Engineering Inc.          //
//  Copyright notice does not imply publication. Decompilation or disassembly of this software is strictly prohibited.//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int _rotaryencoder1A, _rotaryencoder1B, _mode1;
static volatile int _position1;
static int _rotaryencoder2A, _rotaryencoder2B, _mode2;
static volatile int _position2;
static int _rotaryencoder3A, _rotaryencoder3B, _mode3;
static volatile int _position3;
static int _rotaryencoder4A, _rotaryencoder4B, _mode4;
static volatile int _position4;
static int _rotaryencoder5A, _rotaryencoder5B, _mode5;
static volatile int _position5;
static int _rotaryencoder6A, _rotaryencoder6B, _mode6;
static volatile int _position6;
static int _rotaryencoder7A, _rotaryencoder7B, _mode7;
static volatile int _position7;
static int _rotaryencoder8A, _rotaryencoder8B, _mode8;
static volatile int _position8;

#define DEBOUNCE_THRESHOLD 500

static void doRotaryEncoderA(int pinA, int pinB, int *position)
{
    static unsigned long timer=0;
    if (micros() - timer < DEBOUNCE_THRESHOLD) return;
    noInterrupts();
    if (digitalRead(pinA) != digitalRead(pinB)) {
        *position = *position + 1;
    } else {
        *position = *position - 1;
    }
    timer=micros();
    interrupts();
}

static void doRotaryEncoderB(int pinA, int pinB, int *position)
{
    static unsigned long timer=0;
    if (micros() - timer < DEBOUNCE_THRESHOLD) return;
    noInterrupts();
    if (digitalRead(pinA) == digitalRead(pinB)) {
        *position = *position + 1;
    } else {
        *position = *position - 1;
    }
    timer=micros();
    interrupts();
}

#ifdef __cplusplus
extern "C"{
#endif

#define CONCAT(a,b) a##b
#define CONCAT2(a,b) CONCAT(a,b)
#define IMPLEMENT_ROTARYENCODER_INTERRUPT(nb,ch)                        \
    static void CONCAT2(CONCAT2(doRotaryEncoder,nb),ch) () {            \
        CONCAT2(doRotaryEncoder,ch)                                     \
            (CONCAT2(CONCAT2(_rotaryencoder,nb),A),                     \
             CONCAT2(CONCAT2(_rotaryencoder,nb),B),                     \
             &CONCAT2(_position,nb));                              \
    }

IMPLEMENT_ROTARYENCODER_INTERRUPT(1,A)
IMPLEMENT_ROTARYENCODER_INTERRUPT(2,A)
IMPLEMENT_ROTARYENCODER_INTERRUPT(3,A)
IMPLEMENT_ROTARYENCODER_INTERRUPT(4,A)
IMPLEMENT_ROTARYENCODER_INTERRUPT(5,A)
IMPLEMENT_ROTARYENCODER_INTERRUPT(6,A)
IMPLEMENT_ROTARYENCODER_INTERRUPT(7,A)
IMPLEMENT_ROTARYENCODER_INTERRUPT(8,A)

IMPLEMENT_ROTARYENCODER_INTERRUPT(1,B)
IMPLEMENT_ROTARYENCODER_INTERRUPT(2,B)
IMPLEMENT_ROTARYENCODER_INTERRUPT(3,B)
IMPLEMENT_ROTARYENCODER_INTERRUPT(4,B)
IMPLEMENT_ROTARYENCODER_INTERRUPT(5,B)
IMPLEMENT_ROTARYENCODER_INTERRUPT(6,B)
IMPLEMENT_ROTARYENCODER_INTERRUPT(7,B)
IMPLEMENT_ROTARYENCODER_INTERRUPT(8,B)

    
#ifdef __cplusplus
}
#endif
class XRotaryEncoder
{
    
private:
    uint8_t _mode;
    int _nb;
    int _pinA;
    int _pinB;
    int *_positionA;
    
public:
    XRotaryEncoder()
    {
        _pinA=0;
    };
    
    ~XRotaryEncoder(){};
    
    void Init(int pinA, int pinB, uint8_t mode, int nb)
    {
        void (*isrA)(void);
        void (*isrB)(void);
        switch (nb) {
        case 0:
            _rotaryencoder1A=pinA;
            _rotaryencoder1B=pinB;
            _position1=0;
            _positionA=&_position1;
            isrA=doRotaryEncoder1A;
            isrB=doRotaryEncoder1B;
            break;
        case 1:
            _rotaryencoder2A=pinA;
            _rotaryencoder2B=pinB;
            _position2=0;
            _positionA=&_position2;
            isrA=doRotaryEncoder2A;
            isrB=doRotaryEncoder2B;
            break;
        case 2:
            _rotaryencoder3A=pinA;
            _rotaryencoder3B=pinB;
            _position3=0;
            _positionA=&_position3;
            isrA=doRotaryEncoder3A;
            isrB=doRotaryEncoder3B;
            break;
        case 3:
            _rotaryencoder4A=pinA;
            _rotaryencoder4B=pinB;
            _position4=0;
            _positionA=&_position4;
            isrA=doRotaryEncoder4A;
            isrB=doRotaryEncoder4B;
            break;
        case 4:
            _rotaryencoder5A=pinA;
            _rotaryencoder5B=pinB;
            _position5=0;
            _positionA=&_position5;
            isrA=doRotaryEncoder5A;
            isrB=doRotaryEncoder5B;
            break;
        case 5:
            _rotaryencoder6A=pinA;
            _rotaryencoder6B=pinB;
            _position6=0;
            _positionA=&_position6;
            isrA=doRotaryEncoder6A;
            isrB=doRotaryEncoder6B;
            break;
        case 6:
            _rotaryencoder7A=pinA;
            _rotaryencoder7B=pinB;
            _position7=0;
            _positionA=&_position7;
            isrA=doRotaryEncoder7A;
            isrB=doRotaryEncoder7B;
            break;
        case 7:
            _rotaryencoder8A=pinA;
            _rotaryencoder8B=pinB;
            _position8=0;
            _positionA=&_position8;
            isrA=doRotaryEncoder8A;
            isrB=doRotaryEncoder8B;
            break;
        default:
            return;
        }
        _mode=mode;
        _nb=nb;
        _pinA=pinA;
        _pinB=pinB;
        pinMode(pinA,INPUT_PULLUP);
        pinMode(pinB,INPUT_PULLUP);
        switch (mode) {
        case RISINGCHANNELA_N :
            attachInterrupt(digitalPinToInterrupt(pinA), isrA, RISING);
            break;
        case FALLINGCHANNELA_N :
            attachInterrupt(digitalPinToInterrupt(pinA), isrA, FALLING);
            break;
        case CHANGECHANNELA_N :
            attachInterrupt(digitalPinToInterrupt(pinA), isrA, CHANGE);
            break;
        case CHANGEBOTHCHANNELS_N :
            attachInterrupt(digitalPinToInterrupt(pinA), isrA, CHANGE);
            attachInterrupt(digitalPinToInterrupt(pinB), isrB, CHANGE);
            break;

        }

    };
    
    void Terminate()
    {
        switch (_mode) {
        case RISINGCHANNELA_N :
            detachInterrupt(digitalPinToInterrupt(_pinA));
            break;
        case FALLINGCHANNELA_N :
            detachInterrupt(digitalPinToInterrupt(_pinA));
            break;
        case CHANGECHANNELA_N :
            detachInterrupt(digitalPinToInterrupt(_pinA));
            break;
        case CHANGEBOTHCHANNELS_N :
            detachInterrupt(digitalPinToInterrupt(_pinA));
            detachInterrupt(digitalPinToInterrupt(_pinB));
            break;
            
        }
        _pinA=0;
        _pinB=0;
        *_positionA=0;
    };

    uint8_t GetPin() {return _pinA;};

    int GetPosition(){
        if (_positionA) {
            return *_positionA;
        } else {
            return 0;
        }
    };
        
    void SetPosition(int v) {
        if (_positionA) {
            *_positionA=v;
        }
    };
    
};

#define MAX_ROTARYENCODER_NB 8

XRotaryEncoder rotaryencoders[MAX_ROTARYENCODER_NB];

class ActRotaryEncoder
{
public:
    static void init( int pinA, int pinB, uint8_t mode) {
        int i=0;
        for (int j = 0; j < MAX_ROTARYENCODER_NB; j++) {
            if (rotaryencoders[j].GetPin() == pinA){
                rotaryencoders[j].SetPosition(0);
                return;
            }
        }
        while(i < MAX_ROTARYENCODER_NB && rotaryencoders[i].GetPin()!=0){
            i++;
        }
        rotaryencoders[i].Init(pinA,pinB,mode,i);
    };

    static void terminate(uint8_t pinA)
    {
        for (int j = 0; j < MAX_ROTARYENCODER_NB; j++) {
            if (rotaryencoders[j].GetPin() == (int)pinA){
                rotaryencoders[j].Terminate();
                return;
            }
        }
    };

    static int read(uint8_t pinA)
    {
        int position = 0;
        for (int j = 0; j < MAX_ROTARYENCODER_NB; j++) {
            if (rotaryencoders[j].GetPin() == (int)pinA){
                position=rotaryencoders[j].GetPosition();
                break;
            }
        }
        return position;
    };

    static void write(uint8_t pinA, int val)
    {
        for (int j = 0; j < MAX_ROTARYENCODER_NB; j++) {
            if (rotaryencoders[j].GetPin() == (int)pinA){
                rotaryencoders[j].SetPosition(val);
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
                int pin2 = get_pin_number();
                uint8_t mode = read_uint8();
                init(pin,pin2,mode);
                break;
            }
            case TERMINATE_N: {
                terminate(pin);
                break;
            }
            case READ_N: {
                int position = read(pin);
                write_uint32(position);
                break;
            }
            case WRITE_N: {
                int position = (int)read_uint32();
                write(pin,position);
                break;
            }
            }
        };
#endif

};

#ifndef ACT_SIMULATION_MODE

static void arduinorotaryencoderinit (const uint8_t *code1, const uint8_t*code2, const uint8_t *mode) {
  int pin1 = code_to_pin(*code1);
  int pin2 = code_to_pin(*code2);
  ActRotaryEncoder::init(pin1, pin2, *mode);
}

static void arduinorotaryencoderread (const uint8_t *code, int32_t *y) {
  int pin = code_to_pin(*code);
  *y=ActRotaryEncoder::read(pin);
}

static void arduinorotaryencoderwrite (const uint8_t *code, const int32_t *u) {
  int pin = code_to_pin(*code);
  ActRotaryEncoder::write(pin,*u);
}

#endif

