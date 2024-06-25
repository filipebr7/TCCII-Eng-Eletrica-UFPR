////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  File: lcd.h                                                                                                     //
//  Date: 7/24/2020                                                                                                  //
//  Copyright (c) 1989-2020 Altair Engineering Inc. All Rights Reserved. Contains trade secrets of Altair Engineering Inc.          //
//  Copyright notice does not imply publication. Decompilation or disassembly of this software is strictly prohibited.//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <LiquidCrystal.h>

static LiquidCrystal lcd(0, 0, 0, 0, 0, 0);
static uint8_t lcd_nbl = 0;

class ActLCD
{
public:
    static void init(uint8_t rs,
                     uint8_t enable,
                     uint8_t nbC,
                     uint8_t nbL,
                     uint8_t d4,
                     uint8_t d5,
                     uint8_t d6,
                     uint8_t d7)
    {
        lcd=LiquidCrystal(rs,enable,d4,d5,d6,d7);
        lcd.begin(nbC,nbL);
        lcd_nbl=nbL;
    };

	static void init(uint8_t rs,
					 uint8_t rw,
					 uint8_t enable,
					 uint8_t nbC,
					 uint8_t nbL,
					 uint8_t d4,
					 uint8_t d5,
					 uint8_t d6,
					 uint8_t d7)
	{
      lcd = LiquidCrystal(rs, rw, enable, d4, d5, d6, d7);
      lcd.begin(nbC, nbL);
      lcd_nbl=nbL;
	};

	static void init(uint8_t rs,
					 uint8_t enable,
					 uint8_t nbC,
					 uint8_t nbL,
					 uint8_t d0,
					 uint8_t d1,
					 uint8_t d2,
					 uint8_t d3,
					 uint8_t d4,
					 uint8_t d5,
					 uint8_t d6,
					 uint8_t d7)
	{
      lcd = LiquidCrystal(rs, enable, d0, d1, d2, d3, d4, d5, d6, d7);
      lcd.begin(nbC, nbL);
      lcd_nbl=nbL;
	};

	static void init(uint8_t rs,
					 uint8_t rw,
					 uint8_t enable,
					 uint8_t nbC,
					 uint8_t nbL,
					 uint8_t d0,
					 uint8_t d1,
					 uint8_t d2,
					 uint8_t d3,
					 uint8_t d4,
					 uint8_t d5,
					 uint8_t d6,
					 uint8_t d7)
	{
      lcd = LiquidCrystal(rs, rw, enable, d0, d1, d2, d3, d4, d5, d6, d7);
      lcd.begin(nbC, nbL);
      lcd_nbl=nbL;
      
	};

  static void write(const uint8_t InPut[], const uint8_t DataSize)
  {
      int j=0;
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.setCursor(0, j);
      for (int i = 0; i < DataSize; i++) {
          uint8_t c = InPut[i];
          if (c == 0) {
              break;
          } else if (c == 10) {
              j=j+1;
              lcd.setCursor(0, j);
          } else {
              lcd.write(c); 
          }
      }
  };

#ifdef ACT_SIMULATION_MODE
private:
	static void _init()
	{
      int method = 0, rs = 0, rw = 0, enable = 0, d0 = 0, d1 = 0, d2 = 0, d3 = 0, d4 = 0, d5 = 0, d6 = 0, d7 = 0, nbl, nbc;

      method = read_uint8();
      rs = read_uint8();
      enable = read_uint8();
      nbc = read_uint8();
      nbl = read_uint8();
      d7 = read_uint8();
      d6 = read_uint8();
      d5 = read_uint8();
      d4 = read_uint8();
      
      switch (method)
      {
      case 0: {
          init(rs, enable, nbc, nbl, d4, d5, d6, d7);
          break;
      }
      case 1: {
          rw = read_uint8();
          init(rs, rw, enable, nbc, nbl, d4, d5, d6, d7);
          break;
      }
      case 2: {
          d3 = read_uint8();
          d2 = read_uint8();
          d1 = read_uint8();
          d0 = read_uint8();
          init(rs, enable, nbc, nbl, d0, d1, d2, d3, d4, d5, d6, d7);
          break;
      }
      case 3: {
          d3 = read_uint8();
          d2 = read_uint8();
          d1 = read_uint8();
          d0 = read_uint8();
          rw = read_uint8();
          init(rs, rw, enable, nbc, nbl, d0, d1, d2, d3, d4, d5, d6, d7);
          break;
      }
      }
	};
  
  static void _write()
  {
      uint8_t sz = read_uint8();
      uint8_t *data = (uint8_t *)malloc(sz*sizeof(uint8_t));
      if (!data) return;
		  for (int i=0;i<sz;i++) {
          data[i] = read_uint8();
		  }
      write(data,sz);
      free(data);
  };
    
public:
    
    static loop ()
    {
		
        int val = read_uint8();
        switch (val) {
        case INIT_N: {
            _init();
            break;
        }
        case WRITE_N: {
            _write();
            break;
        }
        }
    };
#endif
};
#ifndef ACT_SIMULATION_MODE

static void arduinolcdinit(const uint8_t pins[],const uint8_t* nbC, const uint8_t* nbL, const uint8_t* mode)
{
	
		uint8_t rs = code_to_pin(pins[0]);
		uint8_t e = code_to_pin(pins[1]);
		uint8_t d7 = code_to_pin(pins[2]);
		uint8_t d6 = code_to_pin(pins[3]);
		uint8_t d5 = code_to_pin(pins[4]);
		uint8_t d4 = code_to_pin(pins[5]);

		switch (*mode)
		{
		case 0: {
        ActLCD::init(rs, e, *nbC, *nbL, d4, d5, d6, d7);
        break;
		}
		case 1: {
        uint8_t rw = code_to_pin(pins[6]);
        ActLCD::init(rs, rw, e, *nbC, *nbL, d4, d5, d6, d7);
			break;
		}
		case 2: {
        uint8_t d3 = code_to_pin(pins[6]);
        uint8_t d2 = code_to_pin(pins[7]);
        uint8_t d1 = code_to_pin(pins[8]);
        uint8_t d0 = code_to_pin(pins[9]);
        ActLCD::init(rs, e, *nbC, *nbL, d0, d1, d2, d3, d4, d5, d6, d7);
			break;
		}
		case 3: {
        uint8_t d3 = code_to_pin(pins[6]);
        uint8_t d2 = code_to_pin(pins[7]);
        uint8_t d1 = code_to_pin(pins[8]);
        uint8_t d0 = code_to_pin(pins[9]);
        uint8_t rw = code_to_pin(pins[10]);
        ActLCD::init(rs, rw, e, *nbC, *nbL, d0, d1, d2, d3, d4, d5, d6, d7);
        break;
		}
		}
}

static void arduinolcd(const uint8_t InPut[],const uint8_t DataSize)
{
    ActLCD::write(InPut, DataSize);
}


#endif
