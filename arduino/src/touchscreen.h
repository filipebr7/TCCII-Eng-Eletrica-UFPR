////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  File: touchscreen.h                                                                                                     //
//  Date: 5/12/2020                                                                                                  //
//  Copyright (c) 1989-2020 Altair Engineering Inc. All Rights Reserved. Contains trade secrets of Altair Engineering Inc.          //
//  Copyright notice does not imply publication. Decompilation or disassembly of this software is strictly prohibited.//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <TouchScreen.h>

/* create and initialize touchscreen  */
TouchScreen __ts(0,0,0,0,0);

class ActTouchScreen
{
public:
    static void init(uint8_t xp,
                     uint8_t yp,
                     uint8_t xm,
                     uint8_t ym,
                     uint16_t rx)
    {
        __ts=TouchScreen(xp,yp,xm,ym,rx);
    };

    static void read(int16_t pos[3])
    {
        TSPoint tsPoint=__ts.getPoint();
        pos[0]=tsPoint.x;
        pos[1]=tsPoint.y;
        pos[2]=tsPoint.z;
    };

#ifdef ACT_SIMULATION_MODE
private:
    static void _init()
    {
        int val;
        uint8_t xp = get_pin_number();
        uint8_t yp = get_pin_number();
        uint8_t xm = get_pin_number();
        uint8_t ym = get_pin_number();
        uint16_t rx = read_uint16();
        init(xp,yp,xm,ym,rx);
    };

    static void _read()
    {
        int16_t xyz[3];
        read(xyz);
        write_uint16ptr((uint16_t *)xyz,3);
    };
    
public:
    
    static void loop ()
    {
        int val = read_uint8();
        switch (val) {
        case INIT_N: {//'a'-> init touchscreen
            _init();
            break;
        }
        case READ_N: {//'r'-> read touchscreen value
            _read();
            break;
        }
        }
    };
#endif
};

#ifndef ACT_SIMULATION_MODE

static void arduinotouchscreeninit (const uint8_t pins[], const uint16_t rx[])
{
    uint8_t xp = code_to_pin(pins[0]);
    uint8_t yp = code_to_pin(pins[1]);
    uint8_t xm = code_to_pin(pins[2]);
    uint8_t ym = code_to_pin(pins[3]);
    ActTouchScreen::init(xp,yp,xm,ym,rx);
}

static void arduinotouchscreen (uint16_t y1[], uint16_t y2[], uint16_t y3[])
{
    uint16_t xyz[3];
    ActTouchScreen::read(xyz);
    y1[0]=xyz[0];
    y2[0]=xyz[1];
    y3[0]=xyz[2];
}
#endif
