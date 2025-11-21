   #include <Adafruit_NeoPixel.h>

   // =============================
   // 設定
   // =============================

   // 使用ピン
   const int PIN_MODULE[4] = {2, 3, 4, 8};  
   //   0: D2 (左上)
   //   1: D3 (右上)
   //   2: D4 (左下)
   //   3: D8 (右下)

   const int MODULE_COLS = 8;
   const int MODULE_ROWS = 8;
   const int MODULE_LED_NUM = MODULE_COLS * MODULE_ROWS; // 64 LEDs
   const int MODULE_COUNT = 4;

   const int MATRIX_COLS = 16;
   const int MATRIX_ROWS = 16;

   // 全体LED = 8×8×4 = 256
   const int LED_COUNT = MODULE_LED_NUM;

   // NeoPixel オブジェクト（4個）
   Adafruit_NeoPixel modules[MODULE_COUNT] = {
      Adafruit_NeoPixel(MODULE_LED_NUM, PIN_MODULE[0], NEO_GRB + NEO_KHZ800),
      Adafruit_NeoPixel(MODULE_LED_NUM, PIN_MODULE[1], NEO_GRB + NEO_KHZ800),
      Adafruit_NeoPixel(MODULE_LED_NUM, PIN_MODULE[2], NEO_GRB + NEO_KHZ800),
      Adafruit_NeoPixel(MODULE_LED_NUM, PIN_MODULE[3], NEO_GRB + NEO_KHZ800)
   };

   // 白の明るさ
   const uint8_t WHITE_BASE = 2;

   // モジュールごとの「白」(R,G,B)
   // 0: 左上(D1), 1: 右上(D2), 2: 左下(D7), 3: 右下(D9)
   const uint8_t WHITE_COLOR[4][3] = {
      // {R, G, B}
      {WHITE_BASE, WHITE_BASE, WHITE_BASE}, // 0: 左上（赤っぽい → R少し下げ目）
      {WHITE_BASE, WHITE_BASE, WHITE_BASE}, // 1: 右上（緑っぽい → G少し下げ目）
      {WHITE_BASE, WHITE_BASE, WHITE_BASE}, // 2: 左下（左側と同じ調整）
      {WHITE_BASE, WHITE_BASE, WHITE_BASE}  // 3: 右下（右側と同じ調整）
   };

   // =============================
   // 関数プロトタイプ
   // =============================

   int moduleIndexFromXY(int x, int y);
   int indexInModule(int localX, int localY);
   void setLedXY(int x, int y, bool on);
   void clearAll(bool showNow = true);
   void showAll();

   void drawImage16x16(const uint8_t img[16][16]);
   void drawSubImage8x8(const uint8_t img[8][8], int offsetX, int offsetY);

   // =============================
   // サンプル画像（16x16）
   // =============================

   const uint8_t sampleImage16x16[16][16] = {
   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
   {1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
   {1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1},
   {1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1},
   {1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1},
   {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1},
   {1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1},
   {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1},
   {1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1},
   {1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1},
   {1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1},
   {1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
   };

   const uint8_t difficultDragon16x16[16][16] = {
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
   {0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0},
   {0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,0},
   {0,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0},
   {0,0,1,0,0,1,0,0,0,1,1,1,1,1,1,0},
   {0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,0},
   {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0},
   {0,0,1,1,1,1,1,0,0,1,0,0,0,0,0,0},
   {0,0,1,0,0,0,1,0,0,1,1,1,1,1,0,0},
   {0,0,1,1,1,1,1,0,0,1,0,0,0,0,0,0},
   {0,0,1,0,0,0,1,0,0,1,1,1,1,1,0,0},
   {0,0,1,1,1,1,1,0,0,1,0,0,0,0,0,0},
   {0,0,1,0,0,0,1,0,0,1,1,1,1,1,0,0},
   {0,0,1,0,0,0,1,0,0,1,0,0,0,0,1,0},
   {0,0,1,0,0,1,1,0,0,1,1,1,1,1,1,0},
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
   };

   const uint8_t easyDragon16x16[16][16] = {
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
   {0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
   {0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0},
   {0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0},
   {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
   {0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0},
   {0,0,0,1,0,0,0,1,1,0,0,0,1,1,0,0},
   {0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0},
   {0,0,0,1,0,0,0,1,1,0,0,0,1,1,0,0},
   {0,0,0,1,0,0,0,1,1,0,0,0,1,1,0,0},
   {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},
   {0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,0},
   {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0},
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
   };

   // 8x8 の画像サンプル
   const uint8_t sampleImage8x8[8][8] = {
   {1,0,0,0,0,0,0,1},
   {0,1,0,0,0,0,1,0},
   {0,0,1,0,0,1,0,0},
   {0,0,0,1,1,0,0,0},
   {0,0,0,1,1,0,0,0},
   {0,0,1,0,0,1,0,0},
   {0,1,0,0,0,0,1,0},
   {1,0,0,0,0,0,0,1},
   };

   // =============================
   // setup / loop
   // =============================

   void setup() {
      for (int i = 0; i < MODULE_COUNT; i++) {
         modules[i].begin();
         modules[i].clear();
         modules[i].show();
      }
   }

   void loop() {
      clearAll(false);
      drawImage16x16(sampleImage16x16);
      delay(2000);
      clearAll();
      delay(500);

      clearAll(false);
      drawSubImage8x8(sampleImage8x8, 0, 0); // 左上
      showAll();
      delay(2000);

      clearAll(false);
      drawSubImage8x8(sampleImage8x8, 8, 8); // 右下
      showAll();
      delay(2000);

      clearAll(false);
      drawImage16x16(difficultDragon16x16);
      delay(2000);
      clearAll(false);
      delay(500);

      clearAll(false);
      drawImage16x16(easyDragon16x16);
      delay(2000);
      clearAll(false);
      delay(500);

      fillAll();
      delay(2000);
   }

   // =============================
   // LED マッピング処理
   // =============================

   // 0: 左上(D1)
   // 1: 右上(D2)
   // 2: 左下(D7)
   // 3: 右下(D9)
   int moduleIndexFromXY(int x, int y) {
      bool top = (y < 8);
      bool left = (x < 8);

      if (top && left) return 0;
      if (top && !left) return 1;
      if (!top && left) return 2;
      return 3;
   }

   // モジュール内 8×8 の座標を LED インデックスに変換
   int indexInModule(int localX, int localY) {
      return localY * MODULE_COLS + localX;
   }

   // (x,y) の LED を ON/OFF する
   void setLedXY(int x, int y, bool on) {
      if (x < 0 || x >= 16 || y < 0 || y >= 16) return;

      int m = moduleIndexFromXY(x, y);
      int localX = x % 8;
      int localY = y % 8;
      int idx = indexInModule(localX, localY);

      if (on) {
         uint8_t r = WHITE_COLOR[m][0];
         uint8_t g = WHITE_COLOR[m][1];
         uint8_t b = WHITE_COLOR[m][2];
         modules[m].setPixelColor(idx, modules[m].Color(r, g, b));
      } else {
         modules[m].setPixelColor(idx, 0);
      }
   }

   // =============================
   // 表示系関数
   // =============================

   void showAll() {
      for (int i = 0; i < MODULE_COUNT; i++) {
         modules[i].show();
      }
   }

   void clearAll(bool showNow) {
      for (int i = 0; i < MODULE_COUNT; i++) {
         modules[i].clear();
         if (showNow) modules[i].show();
      }
   }

   void drawImage16x16(const uint8_t img[16][16]) {
      for (int y = 0; y < 16; y++) {
         for (int x = 0; x < 16; x++) {
            setLedXY(x, y, img[y][x] != 0);
         }
      }
      showAll();
   }

   // 8×8 の画像を offsetX, offsetY に描画
   void drawSubImage8x8(const uint8_t img[8][8], int offsetX, int offsetY) {
      for (int y = 0; y < 8; y++) {
         for (int x = 0; x < 8; x++) {
            setLedXY(offsetX + x, offsetY + y, img[y][x] != 0);
         }
      }
   }

   void fillAll() {
      for (int y = 0; y < 16; y++) {
         for (int x = 0; x < 16; x++) {
            setLedXY(x, y, true);
         }
      }
      showAll();
   }


