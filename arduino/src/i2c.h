////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  File: i2c.h                                                                                                     //
//  Date: 5/12/2020                                                                                                  //
//  Copyright (c) 1989-2020 Altair Engineering Inc. All Rights Reserved. Contains trade secrets of Altair Engineering Inc.          //
//  Copyright notice does not imply publication. Decompilation or disassembly of this software is strictly prohibited.//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Wire.h>

#define SLV_TX_BUFF_SIZE 100

uint8_t slv_tx_val[SLV_TX_BUFF_SIZE];
int slv_tx_nval=0;

#define SLV_RX_BUFF_SIZE 100

uint8_t slv_rx_val[SLV_RX_BUFF_SIZE];
int slv_rx_nval=0;


class ActI2CMasterTX
{
public:
    static void init(uint8_t add)
    {
        Wire.begin(add);
    };

    static void write(uint8_t add, int nval, const uint8_t *val)
    {
        Wire.beginTransmission(add);
        Wire.write(val,nval);
        Wire.endTransmission();
    };
};

class ActI2CMasterRX
{
public:
    static void init(uint8_t add)
    {
        Wire.begin(add);
    };

    static int read(uint8_t add, int nval, uint8_t *val)
    {
        int i=-1;
        Wire.requestFrom((int)add,nval);
        while (Wire.available()) {
            char c = Wire.read();
            i++;
            val[i]=c;
        }
        return (i+1);
    };
};

class ActI2CSlaveTX
{
private:
    static void slaveTxEvent()
    {
        Wire.write(slv_tx_val,slv_tx_nval);
        slv_tx_nval=0;
    };
public:
    
    static void init(uint8_t add)
    {
        Wire.begin(add);
        Wire.onRequest(slaveTxEvent);
    };

    static void write(uint8_t add, int nval, const uint8_t *val)
    {
        slv_tx_nval=nval;
        for (int i=0; i< nval; ++i) {
            slv_tx_val[i]=val[i];
        }
    };
};

class ActI2CSlaveRX
{
private:

    static void slaveRxEvent(int howMany)
    {
        slv_rx_nval=0;
        while (Wire.available() > 0 && slv_rx_nval < howMany) {
            slv_rx_val[slv_rx_nval]=Wire.read();
            slv_rx_nval ++;
        }
    };
    
public:
    static void init(uint8_t add)
    {
        Wire.begin(add);
        Wire.onReceive(slaveRxEvent);
    };

    static int read(uint8_t add, int nval, uint8_t *val)
    {
        nval = min(nval,slv_rx_nval);
        for (int i=0; i< nval; ++i) {
            val[i]=slv_rx_val[i];
        }
        return nval;
    };
};

#ifdef ACT_SIMULATION_MODE
class ActI2C
{
private:
    static void attach_i2c_rx_master()
    {
        int add = read_uint8();
        add = min(max(0,add),127); // address
        ActI2CMasterRX::init(add);
    };

    static void comm_i2c_rx_master()
    {
        int add = read_uint8();
        add = min(max(0,add),127); // address
        int nval = read_uint8(); // number of bytes to recieve
        uint8_t *val = (uint8_t *)malloc(nval);
        nval = ActI2CMasterRX::read(add,nval,val);
        write_uint8(nval);
        write_uint8ptr(val,nval);
        free(val);
    };

    static void treat_i2c_rx_master()
    {
        int val = read_uint8();
        switch (val) {
        case INIT_N : { 
            attach_i2c_rx_master();
            break;
        }
        case READ_N : { 
            comm_i2c_rx_master();
            break;
        }
        }
    };

    static void attach_i2c_rx_slave()
    {
        int add = read_uint8();
        add = min(max(0,add),127); // address
        ActI2CSlaveRX::init(add);
    };

    static void comm_i2c_rx_slave()
    {
        int add = read_uint8();
        add = min(max(0,add),127); // address
        int nval = read_uint8(); // number of bytes to recieve
        uint8_t *val = (uint8_t *)malloc(nval);
        nval = ActI2CSlaveRX::read(add,nval,val);
        write_uint8(nval);
        write_uint8ptr(val,nval);
        free(val);
    };

    static void treat_i2c_rx_slave()
    {
        int val = read_uint8();
        switch (val) {
        case INIT_N : {
            attach_i2c_rx_slave();
            break;
        }
        case READ_N : { 
            comm_i2c_rx_slave();
            break;
        }
        }
    };

    static void treat_i2c_rx()
    {
        int val = read_uint8();
        switch (val) {
        case MASTER_N : {
            treat_i2c_rx_master();
            break;
        }
        case SLAVE_N : { 
            treat_i2c_rx_slave();
            break;
        }
        }
    };

    static void attach_i2c_tx_master()
    {
        int add = read_uint8();
        add = min(max(0,add),127); // address
        ActI2CMasterTX::init(add);
    };

    static void comm_i2c_tx_master()
    {
        int add = read_uint8();
        add = min(max(0,add),127); // address
        int nval = read_uint8(); // number of bytes to recieve
        uint8_t *val = (uint8_t *)malloc(nval);
        for (int i = 0; i < nval; i++) {
            val[i]=read_uint8();
        }
        ActI2CMasterTX::write(add,nval,val);
        free(val);
    };

    static void treat_i2c_tx_master()
    {
        int val = read_uint8();
        switch (val) {
        case INIT_N : {
            attach_i2c_tx_master();
            break;
        }
        case WRITE_N : {
            comm_i2c_tx_master();
            break;
        }
        }
    };

    static void attach_i2c_tx_slave()
    {
        int add = read_uint8();
        add = min(max(0,add),127); // address
        ActI2CSlaveTX::init(add);
    };

    static void comm_i2c_tx_slave()
    {
        int add = read_uint8();
        add = min(max(0,add),127); // address
        int nval = read_uint8(); // number of bytes to recieve
        uint8_t *val = (uint8_t *)malloc(nval);
        for (int i = 0; i < nval; i++) {
            val[i]=read_uint8();
        }
        ActI2CSlaveTX::write(add,nval,val);
        free(val);
    };

    static void treat_i2c_tx_slave()
    {
        int val = read_uint8();
        switch (val) {
        case INIT_N : {
            attach_i2c_tx_slave();
            break;
        }
        case WRITE_N : {
            comm_i2c_tx_slave();
            break;
        }
        }
    };

    static void treat_i2c_tx()
    {
        int val = read_uint8();
        switch (val) {
        case MASTER_N : {
            treat_i2c_tx_master();
            break;
        }
        case SLAVE_N : {
            treat_i2c_tx_slave();
            break;
        }
        }
    };
    
public:
    static void loop()
    {
        int val = read_uint8();
        switch (val) {
        case RECEIVE_N : {
            treat_i2c_rx();
            break;
        }
        case TRANSMIT_N : {
            treat_i2c_tx();
            break;
        }
        }
        
    };
};
#endif


#ifndef ACT_SIMULATION_MODE

static void arduinoi2cmtxinit (const uint8_t address[]) {
    ActI2CMasterTX::init(address[0]);
}

static void arduinoi2cmtx (const uint8_t address[], const uint8_t u[], const int32_t szin[]) {
    ActI2CMasterTX::write(address[0],(int)szin[0],u);
}

static void arduinoi2cstxinit (const uint8_t address[]) {
    ActI2CSlaveTX::init(address[0]);
}

static void arduinoi2cstx (const uint8_t address[], const uint8_t u[], const int32_t szin[]) {
    ActI2CSlaveTX::write(address[0],(int)szin[0],u);
}

static void arduinoi2cmrxinit (const uint8_t address[], uint8_t y[], const int32_t szout[]) {
    ActI2CMasterRX::init(address[0]);
}

static void arduinoi2cmrx (const uint8_t address[], uint8_t y[], const int32_t szout[]) {
    ActI2CMasterRX::read(address[0],(int)szout[0],y);
}

static void arduinoi2csrxinit (const uint8_t address[], uint8_t y[], const int32_t szout[]) {
    ActI2CSlaveRX::init(address[0]);
}

static void arduinoi2csrx (const uint8_t address[], uint8_t y[], const int32_t szout[]) {
    ActI2CSlaveRX::read(address[0],(int)szout[0],y);
}

#endif
