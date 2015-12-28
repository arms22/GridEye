#include <Wire.h>
#include <GridEye.h>

#define REG_PCLT	0x00
#define REG_RST		0x01
#define REG_FPSC	0x02
#define REG_INTC	0x03
#define REG_STAT	0x04
#define REG_SCLR	0x05
#define REG_AVE		0x07
#define REG_INTHL	0x08
#define REG_INTHH	0x09
#define REG_INTLL	0x0A
#define REG_INTLH	0x0B
#define REG_INHYSL	0x0C
#define REG_INHYSH	0x0D
#define REG_TOOL	0x0E
#define REG_TOOH	0x0F
#define REG_INT0	0x10
#define REG_INT1	0x11
#define REG_INT2	0x12
#define REG_INT3	0x13
#define REG_INT4	0x14
#define REG_INT5	0x15
#define REG_INT6	0x16
#define REG_INT7	0x17
#define REG_PIXL	0x80

GridEye::GridEye(byte devadr) : devadr_(devadr)
{
}

GridEye::~GridEye()
{
}

void GridEye::sleep(void)
{
	regw(REG_PCLT,0x10);
}

void GridEye::wakeUp(void)
{
	regw(REG_PCLT,0x00);
}

void GridEye::standBy60(void)
{
	regw(REG_PCLT,0x20);
}

void GridEye::standBy10(void)
{
	regw(REG_PCLT,0x21);
}

void GridEye::flagReset(void)
{
	regw(REG_RST,0x30);
}

void GridEye::initialReset(void)
{
	regw(REG_RST,0x3f);
}

void GridEye::setFramerate(byte framerate)
{
	if(framerate <= 1)
		regw(REG_FPSC,0x01);
	else
		regw(REG_FPSC,0x00);
}

void GridEye::serMovingAverageEnable(byte flag)
{
	regw(REG_AVE,flag ? 0x20 : 0x00);
}

void GridEye::setInterruptLevel(int hiLvl, int loLvl, int hysLvl, byte absoluteMode, byte intrOutEnable)
{
	int temp;
	byte buf[6];
	temp = hiLvl < 0 ? hiLvl + 4096 : hiLvl;
	buf[0] = temp >> 8;
	buf[1] = temp & 0xf;
	temp = loLvl < 0 ? loLvl + 4096 : loLvl;
	buf[2] = temp >> 8;
	buf[3] = temp & 0xf;
	temp = hysLvl < 0 ? hysLvl + 4096 : hysLvl;
	buf[4] = temp >> 8;
	buf[5] = temp & 0xf;
	regw(REG_INTC, (absoluteMode ? 0x2 : 0x0) | (intrOutEnable ? 0x1 : 0x0));
	regw(REG_INTHL, buf, sizeof(buf));
}

byte GridEye::status(void)
{
	byte stat;
	regr(REG_STAT, &stat, sizeof(stat));
	return stat;
}

int GridEye::thermistorTemp(void)
{
	byte buf[2];
	regr(REG_TOOL, buf, sizeof(buf));
	int temp = ((buf[1] & 0xf) << 8) | (buf[0]);
	if(temp > 2047){
		temp = temp - 2048;
		temp = -temp;
	}
	return temp;
}

void GridEye::pixelInterrupt(byte *intrBuf)
{
	regr(REG_INT0, intrBuf, 8);
}

void GridEye::pixelOut(int *pixelBuf)
{
	int temp;
	byte i, j, buf[32];
	for(i=0; i<4; i++){
		regr(REG_PIXL + (i * 0x20), buf, sizeof(buf));
		for(j=0; j<32; j+=2){
			temp = ((buf[j + 1] & 0xf) << 8) | buf[j];
			if(temp > 2047){
				temp = temp - 4096;
			}
			*pixelBuf++ = temp;
		}
	}
}

void GridEye::pixelOutRaw(byte *rawBuf)
{
	byte adr = REG_PIXL;
	regr(adr, rawBuf, 32); rawBuf+=32; adr+=32;
	regr(adr, rawBuf, 32); rawBuf+=32; adr+=32;
	regr(adr, rawBuf, 32); rawBuf+=32; adr+=32;
	regr(adr, rawBuf, 32);
}

void GridEye::regw(byte adr, byte data)
{
	Wire.beginTransmission(devadr_);
	Wire.write(adr);
	Wire.write(data);
	Wire.endTransmission();
}

void GridEye::regw(byte adr, byte *data, byte size)
{
	Wire.beginTransmission(devadr_);
	Wire.write(adr);
	Wire.write(data, size);
	Wire.endTransmission();
}

byte GridEye::regr(byte adr, byte *buff, byte size)
{
	Wire.beginTransmission(devadr_);
	Wire.write(adr);
	Wire.endTransmission();
	Wire.requestFrom(devadr_, size);
	Wire.readBytes(buff, size);
	return size;
}
