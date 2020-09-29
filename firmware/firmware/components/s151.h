#ifndef S514_H
#define S514_H

#include <Bounce2.h>
#include "../enums.h"
#include <Wire.h>

#define SI514 0x55

// note that the s514 needs different settings for higher freqs, @see vendor/SiliconLabs
#define FREQ 10000

class S514
{
public:
    Pos3()
    {
    }

    short setup()
    {
        Wire.begin();
        setFreq(FREQ);
    }
    int loop()
    {
    }

protected:
    void writeReg(unsigned char reg, unsigned char d)
    {
        Wire.beginTransmission(SI514);
        Wire.write(reg);
        Wire.write(d);
        Wire.endTransmission();
    }

    void setFreq(unsigned char m_int,
                 unsigned long m_frac,
                 unsigned short hs_div,
                 unsigned char ls_div,
                 unsigned char lp1,
                 unsigned char lp2)
    {
        // 1: Clear OE (bit 2) in register 132 register
        writeReg(0x84, 0x00);

        // 2: Write LP1 and LP2, reg 0, [7:4],[3:0]
        writeReg(0x00, uint8_t(((lp1 << 4) | (lp2))));

        // 3: Write MFRAC[7:0], reg 5
        writeReg(0x05, (m_frac & 0xFF));

        // 4: Write MFRAC[15:8], reg 6
        writeReg(0x06, (m_frac >> 8) & 0xFF);

        // 5: Write MFRAC[23:16], reg 7
        writeReg(0x07, (m_frac >> 16) & 0xFF);

        // 6: Write  (M_Int[2:0] M_Frac[28:24]) reg 8
        writeReg(0x08, (m_int & 0x05) << 5 | (m_frac >> 24) & 0x1F);

        // 7: Write  (M_Int[8:3]) reg 9
        writeReg(0x09, (m_int >> 3) & 0x1F);

        // 8: Write  HS_DIV[7:0] reg 10
        writeReg(0x0A, hs_div & 0xFF);

        // 9: Write  LS_DIV[2:0] HS_DIV[9:8] reg 11
        writeReg(0x0B, ((ls_div & 0x05) << 4) | ((hs_div >> 8) & 0x03));

        // 10: start freq cal process, set reg 132 bit 0
        writeReg(0x84, 0x01);

        // 11: output enable
        writeReg(0x84, 0x04);
    }
    setFreq(double freq)
    {
        unsigned char m_int = 5;
        unsigned long m_frac;
        unsigned short hs_div = 650;
        unsigned char ls_div = 5;
        unsigned char lp1 = 2;
        unsigned char lp2 = 2;
        double M = (freq * hs_div) / 31980000.0;
        m_frac = (float)(M - m_int) * pow(2, 29);
        Si514SetFreq(m_int, m_frac, hs_div, ls_div, lp1, lp2);
    }
#ifdef HAS_DEBUG
    void readRegs(unsigned char reg, unsigned char numBytes)
    {
        int i = reg;
        Wire.beginTransmission(SI514);
        Wire.write(reg);
        Wire.endTransmission();
        Wire.requestFrom(SI514, numBytes);
        while (Wire.available() && i - reg < numBytes)
        {
            byte b = Wire.read();
            Serial.print("Reg ");
            Serial.print(i);
            Serial.print(":");
            Serial.println(b, HEX);
            i++;
        }
    }
    void read()
    {
        readRegs(0x00, 1);
        readRegs(0x05, 7);
        readRegs(0x0B, 1);
        readRegs(0x0E, 1);
        readRegs(0x80, 1);
        readRegs(0x84, 1);
    }
#endif
};

#endif
