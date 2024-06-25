////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  File: pin.h                                                                                                     //
//  Date: 5/12/2020                                                                                                  //
//  Copyright (c) 1989-2020 Altair Engineering Inc. All Rights Reserved. Contains trade secrets of Altair Engineering Inc.          //
//  Copyright notice does not imply publication. Decompilation or disassembly of this software is strictly prohibited.//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int code_to_pin(int val)
{
    //val= val-48;
    switch (val) {
    case 150:
        val=SDA;
        break;
    case 151:
        val=SCL;
        break;
#ifdef SDA1
    case 152:
        val=SDA1;
        break;
    case 153:
        val=SCL1;
        break;
#endif
    case 160:
        val=SS;
        break;
    case 161:
        val=MOSI;
        break;
    case 162:
        val=MISO;
        break;
    case 163:
        val=SCK;
        break;
    default:
        if (val >= 100) val = A0+(val-100);
    }
    return val;
}

#ifdef ACT_SIMULATION_MODE
int get_pin_number()
{
    return code_to_pin(read_serial());
}
#endif
