////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  File: spi.h                                                                                                     //
//  Date: 5/26/2020                                                                                                  //
//  Copyright (c) 1989-2020 Altair Engineering Inc. All Rights Reserved. Contains trade secrets of Altair Engineering Inc.          //
//  Copyright notice does not imply publication. Decompilation or disassembly of this software is strictly prohibited.//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <SPI.h>

static uint8_t __spi_enabled=0;

class XSpiSettings
{
public:
    XSpiSettings()
    {
        pin=0;
    }
    ~XSpiSettings(){}

    int pin;
    uint32_t bitrate = 0;
    uint8_t bitorder = 0;
    uint8_t mode = 0;
};

#define MAX_SETTING_NB NUM_DIGITAL_PINS
XSpiSettings __spisettings[MAX_SETTING_NB];

class ActSPIMaster
{
public:
    static void init(const int ss, const uint32_t bitrate, const uint8_t dataorder,const uint8_t tp)
    {
        if (!__spi_enabled) {
            SPI.begin();
            __spi_enabled=1;
        }
        uint8_t mode=SPI_MODE0;
        if(tp == 0)
            mode = SPI_MODE0;
        else if(tp == 1)
            mode = SPI_MODE1;
        else if(tp == 2)
            mode = SPI_MODE2;
        else if(tp == 3)
            mode = SPI_MODE3;

        uint8_t bitorder = dataorder==1?MSBFIRST : LSBFIRST;
        for (int j = 0; j < MAX_SETTING_NB; j++) {
            if (__spisettings[j].pin == 0 || __spisettings[j].pin == ss) {
                __spisettings[j].pin=ss;
                __spisettings[j].bitrate=bitrate;
                __spisettings[j].bitorder=bitorder;
                __spisettings[j].mode=mode;
            }
        }
        pinMode(ss,OUTPUT);
        digitalWrite(ss,HIGH);
    };

    static void terminate(const uint8_t ss)
    {
        if (__spi_enabled) {
            SPI.end();
            __spi_enabled=0;
        }
    }
    
    static void write(const uint8_t SS,const uint8_t datasize, const uint8_t *data)
    {
        XSpiSettings xspiset;
        for (int j = 0; j < MAX_SETTING_NB; j++) {
            if (__spisettings[j].pin == SS) {
                xspiset=__spisettings[j];
            }
        }
        if (xspiset.pin == 0) return;
        SPISettings spiset(xspiset.bitrate, xspiset.bitorder, xspiset.mode);
        pinMode(SS,OUTPUT);
        digitalWrite(SS,LOW);
        SPI.beginTransaction(spiset);
        for (int i =0; i < datasize; ++i) {
            SPI.transfer(data[i]);
        }
        SPI.endTransaction();
        digitalWrite(SS,HIGH);
    }

    static void read(const uint8_t SS, const uint8_t datasize, uint8_t *val)
    {
        XSpiSettings xspiset;
        for (int j = 0; j < MAX_SETTING_NB; j++) {
            if (__spisettings[j].pin == SS) {
                xspiset=__spisettings[j];
            }
        }
        if (xspiset.pin == 0) return;
        SPISettings spiset(xspiset.bitrate, xspiset.bitorder, xspiset.mode);
        pinMode(SS,OUTPUT);
        digitalWrite(SS, LOW);
        SPI.beginTransaction(spiset);
        for (int i =0; i < datasize; ++i) {
            val[i]=SPI.transfer(0);
        }
        SPI.endTransaction();
        digitalWrite(SS,HIGH);
    }
};





class ActSPISlave
{
public:
    static void init(const int ss, const uint32_t bitrate, const uint8_t dataorder,const uint8_t tp)
    {
        if (!__spi_enabled) {
            SPI.begin();
            __spi_enabled=1;
        }
    }

    static void write(const uint8_t SS,const uint8_t datasize, const uint8_t *data)
    {       
        pinMode(SS,INPUT);
        if(!digitalRead(SS)){
            for (int i =0; i < datasize; ++i) {
                SPI.transfer(data[i]);
            }
        }
    }

    static uint8_t read(const uint8_t SS, const uint8_t datasize, uint8_t *val)
    {
        pinMode(SS,INPUT);
        if(!digitalRead(SS)){
            for (int i =0; i < datasize; ++i) {
                val[i] = SPI.transfer(0);
            }
        }
    }

    static void terminate(const uint8_t ss)
    {
        if (__spi_enabled) {
            SPI.end();
            __spi_enabled=0;
        }
    }
};

#ifdef ACT_SIMULATION_MODE
class ActSPI
{
private:
    static void spi_master_init()
    {
        uint8_t ss;
        uint8_t dataorder;
        uint8_t mode;
        uint32_t br;
        ss=read_uint8();
        dataorder=read_uint8();
        mode=read_uint8();
        br=read_uint32();
        ActSPIMaster::init(ss,br,dataorder,mode);
    }

    static void spi_master_read()
    {
        uint8_t ss=read_uint8();
        uint8_t datasize=read_uint8();
        uint8_t *val=(uint8_t *)malloc(sizeof(uint8_t)*datasize);
        ActSPIMaster::read(ss,datasize,val);
        write_uint8ptr((uint8_t *)val,datasize);
        free(val);
    }
    
    static void spi_master_write()
    {
        uint8_t ss=read_uint8();
        uint8_t datasize=read_uint8();
  
        uint8_t *v = (uint8_t *)malloc(datasize);
        for (int i = 0; i < datasize; i++) {
            v[i]=read_uint8();
        }
        ActSPIMaster::write(ss,datasize,v);
        free(v);
    }

    static void spi_master_terminate()
    {
        uint8_t ss=read_uint8();
        ActSPIMaster::terminate(ss);
    }
    
    static void master_loop()
    {
        int val = read_uint8();
        switch (val) {
        case INIT_N : {
            spi_master_init();
            break;
        }
        case READ_N : { 
            spi_master_read();
            break;
        }
        case WRITE_N : { 
            spi_master_write();
            break;
        }
        case TERMINATE_N : { 
            spi_master_terminate();
            break;
        }
        }
    }
    
    
public:
    static void loop()
    {
        int val = read_uint8();
        switch (val) {
        case MASTER_N : {
            master_loop();
            break;
        }
        case SLAVE_N : { 
            //slave_loop();
            break;
        }
        }
    }
};

#endif

#ifndef ACT_SIMULATION_MODE
static void arduinospimasterinit(const uint8_t sspin[], const uint32_t bitrate[], const uint8_t dataorder[], const uint8_t tp[])
{
    ActSPIMaster::init(*sspin, *bitrate, *dataorder, *tp);
}

static void arduinospimasterread(const uint8_t sspin[], const uint8_t datasize[], uint8_t *data)
{
    ActSPIMaster::read(*sspin, *datasize, data);
}

static void arduinospimasterwrite(const uint8_t sspin[], const uint8_t datasize[], const uint8_t *data)
{
    ActSPIMaster::write(*sspin, *datasize, data);
}

static void arduinospimasterfinish(const uint8_t sspin[])
{
    ActSPIMaster::terminate(*sspin);
}

static void arduinospislaveinit(const uint8_t sspin[], const uint32_t bitrate[], const uint8_t dataorder[], const uint8_t tp[])
{
    ActSPISlave::init(*sspin, *bitrate, *dataorder, *tp);
}

static void arduinospislaveread(const uint8_t sspin[], const uint8_t datasize[], uint8_t *data)
{
    ActSPISlave::read(*sspin, *datasize, data);
}

static void arduinospislavewrite(const uint8_t sspin[], const uint8_t datasize[], const uint8_t *data)
{
    ActSPISlave::write(*sspin, *datasize, data);
}

static void arduinospislavefinish(const uint8_t sspin[])
{
    ActSPISlave::terminate(*sspin);
}

#endif
