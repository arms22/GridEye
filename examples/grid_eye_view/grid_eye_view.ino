#include <Wire.h>
#include <GridEye.h>

GridEye myeye;

void setup(void)
{
  // I2Cバスに参加
  Wire.begin();
  // フレームレートを10に設定
  myeye.setFramerate(10);

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
  // 100ms周期でピクセル温度データを読み出す
  delay(100);
  myeye.pixelOut(pixel);
  // ヘッダ送信（データの区切り位置検出用）
  Serial.write(0x55);
  Serial.write(0xaa);
  // ピクセル温度データ送信（下位バイト、上位バイトの順）
  for (int i = 0; i < 64; i++) {
    Serial.write((pixel[i]     ) & 0xff);
    Serial.write((pixel[i] >> 8) & 0xff);
  }
}
