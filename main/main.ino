#include <Adafruit_NeoPixel.h>

// =============================
// 1. 設定・ピン定義
// =============================

// ボタンピン (GNDに接続して押すとLOWになる想定)
const int PIN_BTN_DIFFICULT = 8; // 「龍」を選ぶボタン
const int PIN_BTN_EASY      = 9; // 「竜」を選ぶボタン

// LEDモジュール接続ピン (左上, 右上, 左下, 右下)
const int PIN_MODULE[4] = {10, 11, 12, 13};

const int MODULE_COLS = 8;
const int MODULE_ROWS = 8;
const int MODULE_LED_NUM = 64;
const int MODULE_COUNT = 4;

const int MATRIX_COLS = 16;
const int MATRIX_ROWS = 16;

// NeoPixel オブジェクト
Adafruit_NeoPixel modules[MODULE_COUNT] = {
  Adafruit_NeoPixel(MODULE_LED_NUM, PIN_MODULE[0], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(MODULE_LED_NUM, PIN_MODULE[1], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(MODULE_LED_NUM, PIN_MODULE[2], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(MODULE_LED_NUM, PIN_MODULE[3], NEO_GRB + NEO_KHZ800)
};

// 色設定
const uint8_t WHITE_BASE = 10; // 明るさ

const uint8_t WHITE_COLOR[4][3] = {
// {R, G, B}
  
  // --- 上の段 (No.0, 1) ---
  // 「緑」は引き続き抑えつつ(0.6)、
  // 「ほんの少し赤」を消すために R を少し下げます(0.85)
  {(uint8_t)(WHITE_BASE * 0.85), (uint8_t)(WHITE_BASE * 0.6), WHITE_BASE}, 
  {(uint8_t)(WHITE_BASE * 0.85), (uint8_t)(WHITE_BASE * 0.6), WHITE_BASE}, 

  // --- 下の段 (No.2, 3) ---
  // 「赤」は引き続き抑えつつ(0.6)、
  // 「青」が強すぎたので B も下げます(0.6)
  {(uint8_t)(WHITE_BASE * 0.6), WHITE_BASE, (uint8_t)(WHITE_BASE * 0.6)}, 
  {(uint8_t)(WHITE_BASE * 0.6), WHITE_BASE, (uint8_t)(WHITE_BASE * 0.6)}
};

// =============================
// 2. 漢字データ (16x16)
// =============================

// 「龍」 (画数多い)
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

// 「竜」 (画数少ない)
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

// =============================
// 3. 状態管理用バッファ
// =============================

// 現在表示されている状態を保持するバッファ
uint8_t currentBuffer[16][16];

// =============================
// 4. 関数プロトタイプ
// =============================
void applyNoiseEvolution(uint8_t current[16][16], const uint8_t target[16][16], int intensity);
void applyGlitchEvolution(uint8_t current[16][16], const uint8_t target[16][16]);
void drawImage16x16(const uint8_t img[16][16]);
void setLedXY(int x, int y, bool on);
void clearAll(bool showNow);
void showAll();
int moduleIndexFromXY(int x, int y);
int indexInModule(int localX, int localY);

// =============================
// 5. setup / loop
// =============================

void setup() {
  pinMode(PIN_BTN_DIFFICULT, INPUT_PULLUP);
  pinMode(PIN_BTN_EASY,      INPUT_PULLUP);
  
  randomSeed(analogRead(0));

  for (int i = 0; i < MODULE_COUNT; i++) {
    modules[i].begin();
    modules[i].clear();
    modules[i].show();
  }
  
  // 初期状態：真っ黒（あるいはランダムノイズでも可）
  // ここでは真っ黒からスタート
  for(int y=0; y<16; y++) for(int x=0; x<16; x++) currentBuffer[y][x] = 0;
  drawImage16x16(currentBuffer);
}

void loop() {
  bool btnDiff = (digitalRead(PIN_BTN_DIFFICULT) == LOW);
  bool btnEasy = (digitalRead(PIN_BTN_EASY) == LOW);

  // どちらかのボタンが押されたら進化処理
  if (btnDiff || btnEasy) {
    
    // ターゲット（目指すべき姿）を決める
    const uint8_t (*target)[16]; // ポインタ宣言
    if (btnDiff) {
      target = difficultDragon16x16;
    } else {
      target = easyDragon16x16;
    }

    // エフェクトの種類をランダムで決める (0: Noise, 1: Glitch)
    // 混ぜることで「多様な変化」を見せる
    int effectType = random(2); 

    if (effectType == 0) {
      // 【Noise Evolution】
      // ドット単位でパラパラとターゲットに変化する
      // intensity: 一度に変化する量 (大きいほど早くターゲットになる)
      applyNoiseEvolution(currentBuffer, target, 15); 
    } else {
      // 【Glitch Evolution】
      // 行・列・ブロック単位でバサッとターゲットに変化する
      applyGlitchEvolution(currentBuffer, target);
    }

    // 更新されたバッファを描画
    drawImage16x16(currentBuffer);

    // 連続入力の制御（少し待つ）
    delay(150); 
    
    // ボタン押しっぱなしでも連続で変化するようにしたい場合は
    // ここでボタンが離されるのを待つ処理は入れない
  }
}

// =============================
// 6. 進化・融合ロジック (収束型)
// =============================

/**
 * 【Noise Evolution】 (粒子変化)
 * 現在の画像(current)を見て、ターゲット(target)と違うドットがあれば、
 * 指定した確率(intensity %)でターゲットの色に上書きする。
 * -> 繰り返すと徐々にターゲットと完全一致していく。
 */
void applyNoiseEvolution(uint8_t current[16][16], const uint8_t target[16][16], int intensity) {
  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 16; x++) {
      // もし現在の色がターゲットと違うなら
      if (current[y][x] != target[y][x]) {
        // 確率で修正する
        if (random(100) < intensity) {
          current[y][x] = target[y][x];
        }
      }
    }
  }
}

/**
 * 【Glitch Evolution】 (ブロック/ライン変化)
 * ドット単位ではなく、「行」や「列」、あるいは「矩形ブロック」単位で
 * ターゲットの情報を強制的にコピーする。
 * デジタルノイズのような不連続な変化を生む。
 */
void applyGlitchEvolution(uint8_t current[16][16], const uint8_t target[16][16]) {
  int glitchMode = random(3); // 0:行コピー, 1:列コピー, 2:ブロックコピー

  if (glitchMode == 0) {
    // ランダムな数行をターゲットと同じにする (水平グリッチ)
    int numLines = random(1, 4); // 1〜3行
    for(int i=0; i<numLines; i++){
      int y = random(16);
      for(int x=0; x<16; x++) {
        current[y][x] = target[y][x];
      }
    }
  } 
  else if (glitchMode == 1) {
    // ランダムな数列をターゲットと同じにする (垂直グリッチ)
    int numCols = random(1, 4);
    for(int i=0; i<numCols; i++){
      int x = random(16);
      for(int y=0; y<16; y++) {
        current[y][x] = target[y][x];
      }
    }
  }
  else {
    // ランダムな矩形領域をコピーする (ブロックノイズ)
    int startX = random(16);
    int startY = random(16);
    int w = random(4, 9); // 幅
    int h = random(2, 5); // 高さ
    
    for(int y = startY; y < startY + h && y < 16; y++){
      for(int x = startX; x < startX + w && x < 16; x++){
        current[y][x] = target[y][x];
      }
    }
  }
}

// =============================
// 7. 描画系ヘルパー関数
// =============================

// モジュール番号判定 (0:左上, 1:右上, 2:左下, 3:右下)
int moduleIndexFromXY(int x, int y) {
  bool top = (y < 8);
  bool left = (x < 8);
  if (top && left) return 0;
  if (top && !left) return 1;
  if (!top && left) return 2;
  return 3;
}

// モジュール内のLED番号計算
int indexInModule(int localX, int localY) {
  return localY * MODULE_COLS + localX;
}

// 1ドットの点灯制御
// (x,y) の LED を ON/OFF する（90度回転対応版）
void setLedXY(int x, int y, bool on) {
  // 範囲外チェック
  if (x < 0 || x >= 16 || y < 0 || y >= 16) return;

  // ==========================================
  // 1. 座標の回転処理 (ここで向きを変えます)
  // ==========================================
  
  // 【時計回り (CW) に90度回転】
  //  (作品を右に90度倒して設置する場合)
  int rx = 15 - y;
  int ry = x;

  // もし逆向き（反時計回り）だった場合は、上記をコメントアウトしてこちらを使ってください
  // int rx = y;
  // int ry = 15 - x;

  // ==========================================
  // 2. 物理的なモジュール・LED位置の特定
  // ==========================================
  // ※これ以降は回転後の rx, ry を使います

  int m = moduleIndexFromXY(rx, ry);
  int localX = rx % 8;
  int localY = ry % 8;
  int idx = indexInModule(localX, localY);

  // ==========================================
  // 3. 点灯・色指定 (キャリブレーション適用)
  // ==========================================
  
  if (on) {
    // 各モジュールごとの補正色を取得
    uint8_t r = WHITE_COLOR[m][0];
    uint8_t g = WHITE_COLOR[m][1];
    uint8_t b = WHITE_COLOR[m][2];
    modules[m].setPixelColor(idx, modules[m].Color(r, g, b));
  } else {
    modules[m].setPixelColor(idx, 0);
  }
}

void showAll() {
  for (int i = 0; i < MODULE_COUNT; i++) modules[i].show();
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