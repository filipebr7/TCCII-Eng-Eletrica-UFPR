////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  File: uart.h                                                                                                     //
//  Date: 6/12/2020                                                                                                  //
//  Copyright (c) 1989-2020 Altair Engineering Inc. All Rights Reserved. Contains trade secrets of Altair Engineering Inc.          //
//  Copyright notice does not imply publication. Decompilation or disassembly of this software is strictly prohibited.//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ActUART
{
private:
    static uint8_t get_configuration(uint8_t bytesize,uint8_t stopbits,uint8_t parity)
    {
        String conf="SERIAL_";
        conf=conf+(String)bytesize;
        
        switch(parity)
        {
        case 0:{
            conf+="N";
            break;
        }
        case 1:{
            conf+="E";
            break;
        }
        case 2:{
            conf+="O";
            break;
        }
        }  
        conf+=(String)stopbits;
        
        if(conf=="SERIAL_5N1") return SERIAL_5N1;
        if(conf=="SERIAL_6N1") return SERIAL_6N1;
        if(conf=="SERIAL_7N1") return SERIAL_7N1;
        if(conf=="SERIAL_8N1") return SERIAL_8N1;
        
        if(conf=="SERIAL_5E1") return SERIAL_5E1;
        if(conf=="SERIAL_6E1") return SERIAL_6E1;
        if(conf=="SERIAL_7E1") return SERIAL_7E1;
        if(conf=="SERIAL_8E1") return SERIAL_8E1;
        
        if(conf=="SERIAL_5O1") return SERIAL_5O1;
        if(conf=="SERIAL_6O1") return SERIAL_6O1;
        if(conf=="SERIAL_7O1") return SERIAL_7O1;
        if(conf=="SERIAL_8O1") return SERIAL_8O1;
        
        if(conf=="SERIAL_5N2") return SERIAL_5N2;
        if(conf=="SERIAL_6N2") return SERIAL_6N2;
        if(conf=="SERIAL_7N2") return SERIAL_7N2;
        if(conf=="SERIAL_8N2") return SERIAL_8N2;
        
        if(conf=="SERIAL_5E2") return SERIAL_5E2;
        if(conf=="SERIAL_6E2") return SERIAL_6E2;
        if(conf=="SERIAL_7E2") return SERIAL_7E2;
        if(conf=="SERIAL_8E2") return SERIAL_8E2;
        
        if(conf=="SERIAL_5O2") return SERIAL_5O2;
        if(conf=="SERIAL_6O2") return SERIAL_6O2;
        if(conf=="SERIAL_7O2") return SERIAL_7O2;
        if(conf=="SERIAL_8O2") return SERIAL_8O2;
    };
    
public:
    
    static void init(uint8_t port,uint8_t bytesize,uint8_t stopbits,uint8_t parity,uint32_t br)
    {
        uint8_t conf=get_configuration(bytesize,stopbits,parity);
        
        switch(port)
        {
        case 0:{
            //This case should not be used in pingpong
            Serial.begin(br,conf);
            break;
        }
        case 1:{
#ifdef HAVE_HWSERIAL1
            Serial1.begin(br,conf);
#endif
            break;
        }
        case 2:{
#ifdef HAVE_HWSERIAL2
            Serial2.begin(br,conf);
#endif
            break;
        }
        case 3:{
#ifdef HAVE_HWSERIAL3
            Serial3.begin(br,conf);
#endif
            break;
        }
            
        }  
    };
    
    static void write(uint8_t port,uint8_t *data,int packetsize)
    {
 
        switch(port)
        {
        case 0:{
            //This case should not be used in pingpong
            Serial.write(data,packetsize);
            break;
        }
        case 1:{
#ifdef HAVE_HWSERIAL1
            Serial1.write(data,packetsize);
#endif
            break;
        }
        case 2:{
#ifdef HAVE_HWSERIAL2
            Serial2.write(data,packetsize);
#endif
            break;
        }
        case 3:{
#ifdef HAVE_HWSERIAL3
            Serial3.write(data,packetsize);
#endif
            break;
        }
            
        }
    };
private:
    static void _read(Stream *serial, uint8_t *data, int packetsize)
    {
        if (serial->available() > 0){
            size_t readSize = 0;
            while (readSize < packetsize) {
                size_t newReadSize = serial->readBytes(data+readSize,packetsize - readSize);
                readSize+=newReadSize;
            }
        }
    }

public:
    static void read(uint8_t port,uint8_t *data,int packetsize)
    {
        
        switch(port)
        {
        case 0:{
            //This case should not be used in pingpong
            _read(&Serial,data,packetsize);
            break;
        }
        case 1:{
#ifdef HAVE_HWSERIAL1
            _read(&Serial1,data,packetsize);
#endif
            break;
        }
            
        case 2:{
#ifdef HAVE_HWSERIAL2
            _read(&Serial2,data,packetsize);
#endif
            break;
        }
            
        case 3:{
#ifdef HAVE_HWSERIAL3
            _read(&Serial3,data,packetsize);
#endif
            break;
        }
        }
    };
private:
    
    static void empty_serial(HardwareSerial *serial)
    {
        while (serial->available() > 0) {
            serial->read();
        }
    };

public:    
    static void terminate(uint8_t port)
    {
        switch(port) {
        case 0:{
            //This case should not be used in pingpong
            Serial.end();
            break;
        }
        case 1:{
#ifdef HAVE_HWSERIAL1
            Serial1.end();
#endif
            break;
        }
        case 2:{
#ifdef HAVE_HWSERIAL2
            //empty_serial(&Serial2);
            Serial2.end();
#endif
            break;
        }
        case 3:{
#ifdef HAVE_HWSERIAL3
            Serial3.end();
#endif
            break;
        }
            
        }  
    };

#ifdef ACT_SIMULATION_MODE
private:
    static void uart_init()
    {
        uint8_t port;
        uint8_t bytesize;
        uint8_t stopbits;
        uint8_t parity;
        uint32_t br;
        
        port=read_uint8();
        bytesize=read_uint8();
        stopbits=read_uint8();
        parity=read_uint8();
        br=read_uint32();
        init(port,bytesize,stopbits,parity,br);
    };

    static void uart_read()
    {
        uint8_t port=read_uint8();
        uint8_t packetsize=read_uint8();
        
        uint8_t *v = (uint8_t *)malloc(packetsize);
        
        read(port,v,packetsize);
        write_uint8ptr(v,packetsize);
        free(v);
    };

    static void uart_write()
    {
        uint8_t port=read_uint8();
        uint8_t packetsize=read_uint8();

        uint8_t *v = (uint8_t *)malloc(packetsize);
        for (int i = 0; i < packetsize; i++) {
            v[i]=read_uint8();
        }
        write(port,v,packetsize);
        free(v);
    };

    static void uart_finish()
    {
        uint8_t port=read_uint8();
        terminate(port);
    };
public:
    static void loop()
    {
        int val = read_uint8();
        switch (val) {
        case INIT_N : { // 'a' -> initialize
            uart_init();
            break;
        }
        case TERMINATE_N : { // 'd' -> initialize
            uart_finish();
            break;
        }
        case READ_N : { // 'r' -> read 
            uart_read();
            break;
        }
        case WRITE_N : { // 'w' -> read 
            uart_write();
            break;
        }
        }
    };
#endif
    
};

#ifndef ACT_SIMULATION_MODE

static void arduinouartinit(const uint8_t port[],
                            const uint8_t bytesize[],
                            const uint8_t stopbits[],
                            const uint8_t parity[],
                            const uint32_t baudrate[])
{       
    ActUART::init(*port,*bytesize,*stopbits,*parity,*baudrate);
};

static void arduinouartread(const uint8_t port[],
                            const uint8_t packetsize[],
                            uint8_t *data)
{
    ActUART::read(*port,data,*packetsize);
};

static void arduinouartwrite(const uint8_t port[],
                             const uint8_t packetsize[],
                             const uint8_t *data)
{
    ActUART::write(*port,data,*packetsize);
};

static void arduinouartfinish(const uint8_t port[])
{
    ActUART::terminate(*port);
};

#endif
