#ifndef __GRID_EYE__
#define __GRID_EYE__

#include <Arduino.h>

#define GridEye_DeviceAddress_0 (0x68)
#define GridEye_DeviceAddress_1 (0x69)

#define GridEye_PixelOut_Unit (0.25)
#define GridEye_ThermistorTemp_Unit (0.0625)

#define GridEye_ConvertPixelOutToCelsius(t) ((t)*GridEye_PixelOut_Unit)
#define GridEye_ConvertThermistorTempToCelsius(t) ((t)*GridEye_ThermistorTemp_Unit)

class GridEye {
public:
	GridEye(byte devadr = GridEye_DeviceAddress_0);
	~GridEye();
	void sleep(void);
	void wakeUp(void);
	void standBy60(void);
	void standBy10(void);
	void flagReset(void);
	void initialReset(void);
	void setFramerate(byte framerate);
	void serMovingAverageEnable(byte flag);
	void setInterruptLevel(int hiLvl, int loLvl, int hysLvl = 0, byte absoluteMode = 0, byte intrOutEnable = 0);
	byte status(void);
	int thermistorTemp(void);
	void pixelInterrupt(byte *intrBuf);
	void pixelOut(int *pixelBuf);
	void pixelOutRaw(byte *rawBuf);
private:
	byte devadr_;
	void regw(byte adr, byte data);
	void regw(byte adr, byte *data, byte size);
	byte regr(byte adr, byte *buff, byte size);
};

#endif
