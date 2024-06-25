////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  File: serial.h                                                                                                     //
//  Date: 5/12/2020                                                                                                  //
//  Copyright (c) 1989-2020 Altair Engineering Inc. All Rights Reserved. Contains trade secrets of Altair Engineering Inc.          //
//  Copyright notice does not imply publication. Decompilation or disassembly of this software is strictly prohibited.//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// read serial function: will read from the lower byte to the highest byte

uint8_t read_serial()
{
    while (Serial.available()==0) {}; // Waiting char
    return Serial.read();
}

uint8_t read_uint8()
{
    return read_serial();
}

uint16_t read_uint16()
{
    uint16_t b1 = (uint16_t)read_serial();
    uint16_t b2= (uint16_t)read_serial() << 8;
    return b1|b2;
}

uint32_t read_uint32()
{
    uint32_t b1 = (uint32_t)read_serial();
    uint32_t b2= (uint32_t)read_serial() << 8;
    uint32_t b3= (uint32_t)read_serial() << 16;
    uint32_t b4= (uint32_t)read_serial() << 24;
    return b1|b2|b3|b4;
}

void write_serial(uint8_t *val, int nb)
{
    Serial.write(val,nb);
}

void write_uint8(uint8_t val)
{
    write_serial(&val,1);
}

void write_uint16(uint16_t val)
{
    uint8_t v[2];
    v[0]=val&0x00FF;  //L 
    v[1]=(val&0xFF00)>>8;  //H
    write_serial(v,2);
}

void write_uint32(uint32_t val)
{
    uint8_t v[4];
    v[0]=val&0x000000FF;  //L 
    v[1]=(val&0x0000FF00)>>8;
    v[2]=(val&0x00FF0000)>>16;
    v[3]=(val&0xFF000000)>>24;  //H
    write_serial(v,4);
}

void write_uint8ptr(uint8_t *val, int nb)
{
    write_serial(val,nb);
}

void write_uint16ptr(uint16_t *val, int nb)
{
    int n = 2*nb;
    uint8_t *v = (uint8_t *)malloc(n);
    for (int i=0; i< nb; ++i) {
        v[2*i]=val[i]&0x00FF;  //L 
        v[2*i+1]=(val[i]&0xFF00)>>8;  //H
    }
    write_serial(v,n);
    free(v);
}

void write_uint32ptr(uint32_t *val, int nb)
{
    int n = 4*nb;
    uint8_t *v = (uint8_t *)malloc(n);
    for (int i=0; i< nb; ++i) {
        v[4*i]=val[i]&0x000000FF;  //L 
        v[4*i+1]=(val[i]&0x0000FF00)>>8;
        v[4*i+2]=(val[i]&0x00FF0000)>>16;
        v[4*i+3]=(val[i]&0xFF000000)>>24;  //H
    }
    write_serial(v,2);
    free(v);
}
