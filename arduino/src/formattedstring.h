#ifndef ACT_SIMULATION_MODE

static void arduinoformattedstring(uint8_t *y, uint16_t sz, uint8_t n, ...)
{
    va_list vl;
    va_start(vl,n);
    uint8_t tp;
    int i=0;
    String s="";
    memset(y,0,sz);
    while (i <(int)n) {
        tp=va_arg(vl,unsigned int);
        switch (tp) {
        case 1: {
            double v=va_arg(vl,double);
            uint8_t decimal = va_arg(vl,unsigned int);
            s=s+String(v,decimal);
            i=i+3;
            break;
        }
        case 3: {
            int32_t v=va_arg(vl,long);
            uint8_t base = va_arg(vl,unsigned int);
            s=s+String(v,base);
            i=i+3;
            break;
        }
        case 4: {
            int16_t v=va_arg(vl,int);
            uint8_t base = va_arg(vl,unsigned int);
            s=s+String(v,base);
            i=i+3;
            break;
        }
        case 5: {
            int8_t v=va_arg(vl,int);
            uint8_t base = va_arg(vl,unsigned int);
            s=s+String(v,base);
            i=i+3;
            break;
        }
        case 6: {
            uint32_t v=va_arg(vl,unsigned long);
            uint8_t base = va_arg(vl,unsigned int);
            s=s+String(v,base);
            i=i+3;
            break;
        }
        case 7: {
            uint16_t v=va_arg(vl,unsigned int);
            uint8_t base = va_arg(vl,unsigned int);
            s=s+String(v,base);
            i=i+3;
            break;
        }
        case 8: {
            uint8_t v=va_arg(vl,unsigned int);
            uint8_t base = va_arg(vl,unsigned int);
            s=s+String(v,base);
            i=i+3;
            break;
        }
        case 9: {
            int32_t v=va_arg(vl,unsigned long);
            v=(v!=0)?1:0;
            uint8_t base = va_arg(vl,unsigned int);
            s=s+String(v,base);
            i=i+3;
            break;
        }
        case 10: {
            char *v=va_arg(vl,char *);
            s=s+String(v);
            i=i+2;
            break;
        }
        default: {
            i=i+2;
            break;
        }   
        }
    }
    va_end(vl);
    int nn = s.length();
    memcpy(y,s.c_str(),min(sz,nn+1));
}

#endif
