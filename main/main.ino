#include <Adafruit_NeoPixel.h>

// =============================
// 1. 設定・ピン定義
// =============================

// ボタンピン (GNDに接続して押すとLOWになる想定)
const int PIN_BTN_DIFFICULT = 8; // 「龍」を選ぶボタン
const int PIN_BTN_EASY      = 9; // 「竜」を選ぶボタン

// LEDモジュール接続ピン (左上, 右上, 左下, 右下 の順を想定)
const int PIN_MODULE[4] = {10, 11, 12, 13};

const int MODULE_COLS = 8;
const int MODULE_ROWS = 8;
const int MODULE_LED_NUM = 64;
const int MODULE_COUNT = 4;

const int MATRIX_COLS = 16;
const int MATRIX_ROWS = 16;

// NeoPixel オブジェクト生成
Adafruit_NeoPixel modules[MODULE_COUNT] = {
  Adafruit_NeoPixel(MODULE_LED_NUM, PIN_MODULE[0], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(MODULE_LED_NUM, PIN_MODULE[1], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(MODULE_LED_NUM, PIN_MODULE[2], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(MODULE_LED_NUM, PIN_MODULE[3], NEO_GRB + NEO_KHZ800)
};

// 色設定
const uint8_t WHITE_BASE = 10; // 明るさ調整 (適宜変更してください)
const uint8_t WHITE_COLOR[4][3] = {
  {WHITE_BASE, WHITE_BASE, WHITE_BASE},
  {WHITE_BASE, WHITE_BASE, WHITE_BASE},
  {WHITE_BASE, WHITE_BASE, WHITE_BASE},
  {WHITE_BASE, WHITE_BASE, WHITE_BASE}
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

// 表示用バッファ (計算結果をここに入れる)
uint8_t displayBuffer[16][16];

// =============================
// 3. 関数プロトタイプ宣言
// =============================
int moduleIndexFromXY(int x, int y);
int indexInModule(int localX, int localY);
void setLedXY(int x, int y, bool on);
void clearAll(bool showNow = true);
void showAll();
void drawImage16x16(const uint8_t img[16][16]);

// 融合ロジック関数
void createGlitchArt(const uint8_t imgA[16][16], const uint8_t imgB[16][16], uint8_t result[16][16]);
void createNoiseArt(const uint8_t baseImg[16][16], const uint8_t noiseSource[16][16], uint8_t result[16][16], int probability);
void createChimeraArt(const uint8_t upperImg[16][16], const uint8_t lowerImg[16][16], uint8_t result[16][16]);


// =============================
// 4. セットアップ & ループ
// =============================

void setup() {
  // ボタン設定 (プルアップ)
  pinMode(PIN_BTN_DIFFICULT, INPUT_PULLUP);
  pinMode(PIN_BTN_EASY,      INPUT_PULLUP);
  
  // ランダムシード
  randomSeed(analogRead(0));

  // LED初期化
  for (int i = 0; i < MODULE_COUNT; i++) {
    modules[i].begin();
    modules[i].clear();
    modules[i].show();
  }
  
  // 起動時に一度クリア
  clearAll(true);
}

void loop() {
  // ボタンの状態を取得 (押されると LOW)
  bool btnDiff = (digitalRead(PIN_BTN_DIFFICULT) == LOW);
  bool btnEasy = (digitalRead(PIN_BTN_EASY) == LOW);

  if (btnDiff || btnEasy) {
    // --------------------------------------------------
    // どちらかのボタンが押された時の処理
    // --------------------------------------------------
    
    // 1. フラッシュ演出 (入力反応)
    clearAll(false);
    showAll(); 
    delay(50); 

    // 2. どの融合モードを使うかランダムに決める (デモ用)
    //    0: グリッチ, 1: ノイズ, 2: キメラ
    int mode = random(3);

    if (mode == 0) {
      // 【グリッチ融合】: XORで2つを混ぜる
      createGlitchArt(difficultDragon16x16, easyDragon16x16, displayBuffer);
      
    } else if (mode == 1) {
      // 【ノイズ融合】: 選んだ方をベースに、選ばなかった方をノイズとして混ぜる
      if (btnDiff) {
         // 龍がベース、竜がノイズ
         createNoiseArt(difficultDragon16x16, easyDragon16x16, displayBuffer, 30); // 30%の確率で混ざる
      } else {
         // 竜がベース、龍がノイズ
         createNoiseArt(easyDragon16x16, difficultDragon16x16, displayBuffer, 30);
      }
      
    } else if (mode == 2) {
      // 【キメラ融合】: 上半分と下半分を合体
      if (btnDiff) {
         // 上が龍、下が竜
         createChimeraArt(difficultDragon16x16, easyDragon16x16, displayBuffer);
      } else {
         // 上が竜、下が龍
         createChimeraArt(easyDragon16x16, difficultDragon16x16, displayBuffer);
      }
    }

    // 3. 結果を描画
    drawImage16x16(displayBuffer);

    // 4. しばらく表示してから消す、または次の入力まで待つ
    //    ここではチャタリング防止も兼ねて少し待つ
    delay(1000); 
    
    // 必要であればここで clearAll() して待機状態に戻す
    // clearAll(true);

  } else {
    // 何も押されていない時
    // 待機アニメーションなどを入れるならここ
    // 例: 1秒おきに「龍」と「竜」を交互に表示するなど
  }
}


// =============================
// 5. 融合アルゴリズムの実装
// =============================

/**
 * 【グリッチ融合】
 * 2つの画像の排他的論理和(XOR)をとる。
 * 重なっている部分は消え、ズレている部分だけが残るデジタルな表現。
 */
void createGlitchArt(const uint8_t imgA[16][16], const uint8_t imgB[16][16], uint8_t result[16][16]) {
  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 16; x++) {
      // 両方1なら0、片方だけ1なら1
      bool valA = (imgA[y][x] != 0);
      bool valB = (imgB[y][x] != 0);
      result[y][x] = (valA ^ valB) ? 1 : 0;
    }
  }
}

/**
 * 【確率的ノイズ融合】
 * baseImg を基本としつつ、noiseSource の画素を確率的に混ぜ込む。
 * probability (0-100): noiseSourceの画素が上書きされる確率
 */
void createNoiseArt(const uint8_t baseImg[16][16], const uint8_t noiseSource[16][16], uint8_t result[16][16], int probability) {
  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 16; x++) {
      // まずベースをコピー
      result[y][x] = baseImg[y][x];

      // ノイズソース側にドットがある場合、一定確率でそれを採用(または反転)させる
      if (noiseSource[y][x] != 0) {
        if (random(100) < probability) {
           // ノイズソースの情報を上書き (1にする、あるいは反転する等)
           // ここでは「混ざる」表現として反転させてみる
           result[y][x] = !result[y][x]; 
        }
      }
    }
  }
}

/**
 * 【構造的キメラ融合】
 * 画面を上下に分割し、異なる漢字を結合する。
 * y=0-7: upperImg, y=8-15: lowerImg
 */
void createChimeraArt(const uint8_t upperImg[16][16], const uint8_t lowerImg[16][16], uint8_t result[16][16]) {
  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 16; x++) {
      if (y < 8) {
        result[y][x] = upperImg[y][x];
      } else {
        result[y][x] = lowerImg[y][x];
      }
    }
  }
}


// =============================
// 6. 描画系ヘルパー関数
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