#include <Wire.h>
#include <GridEye.h>

GridEye myeye;

void setup(void)
{
  // I2Cバスに参加
  Wire.begin();
  // シリアルポート初期化
  Serial.begin(115200);
  while (!Serial) {
    ; // シリアルポート接続待ち
  }
}

// ピクセル温度データ保存用
int pixel[64];

void loop(void)
{
  // サーミスタ温度読み出し
  int temp = myeye.thermistorTemp();
  Serial.print(F("Thermistor Temp: "));
  Serial.println(temp * 0.065); // 1単位 = 0.065度

  // ピクセル温度データ読み出し
  myeye.pixelOut(pixel);
  Serial.println(F("Pixel Output: "));
  for (int i = 0; i < 64; i++) {
    if (i && ((i % 8) == 0)) {
      Serial.println();
    }
    Serial.print(pixel[i] * 0.25); // 1単位 = 0.25度
    Serial.print(' ');
  }
  Serial.println();

  // 1秒待つ
  delay(1000);
}
