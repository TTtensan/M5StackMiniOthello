#include <M5Stack.h>

int base[6][6]; //6×6の配列を宣言
int player = 1;
boolean skip1 = false;  //skipのflag
boolean skip2 = false;

void setup() {
  M5.begin();
  //はじめのオセロをセット
  base[2][2] = 1;
  base[3][3] = 1;
  base[2][3] = 2;
  base[3][2] = 2;
  drawGameLine(); //マス目を描画
  drawOthello();  //オセロを描画
  drawSkip();
}

void loop() {
  decidePlace(player);  //ボタン入力を受け付け、オセロを置く
  drawOthello();  //オセロを描画
  //プレイヤーを交代
  if (player == 1) {
    player = 2;
  } else if (player == 2) {
    player = 1;
  }
  M5.update();    //画面を更新
}

void decidePlace(int player) {  //ボタンを使ってオセロを設置
  int pattern = 0;  //押されたボタンでパターン分け
  int column = 0;   //行
  int row = 0;      //列
  int opponent;     //相手のコマの色。ひっくり返す判定に使う
  M5.Lcd.drawRect(41 + (column * 40), 1 + (row * 40), 39, 39, 0xff6e); //現地点を見やすく黄色で囲む
  if (player == 1) {
    M5.Lcd.drawCircle(60 + (column * 40), 20 + (row * 40), 15, 0xffff); //仮のオセロを描画
    opponent = 2;
  } else if (player == 2) {
    M5.Lcd.drawCircle(60 + (column * 40), 20 + (row * 40), 15, 0x0000);
    opponent = 1;
  }
  M5.update();
  while (pattern != 2) {

    if (M5.BtnA.isPressed()) {  //仮のオセロを下に移動
      if (column != 6) {
        M5.Lcd.fillRect(41 + (column * 40), 1 + (row * 40), 39, 39, 0x5e02); //現地点を緑で塗りつぶす
        pattern = 1;
        if (row == 5) {
          row = 0;
        } else {
          row++;
        }
        drawOthello();  //オセロを描画
        M5.Lcd.drawRect(41 + (column * 40), 1 + (row * 40), 39, 39, 0xff6e); //現地点を見やすく黄色で囲む
        if (player == 1) {
          M5.Lcd.drawCircle(60 + (column * 40), 20 + (row * 40), 15, 0xffff); //仮の白オセロを描画
        } else if (player == 2) {
          M5.Lcd.drawCircle(60 + (column * 40), 20 + (row * 40), 15, 0x0000); //仮の黒オセロを描画
        }
      } else if (column == 6) {
      }
    } else if (M5.BtnB.isPressed()) {//正しい位置にオセロを設置
      if (column != 6) {
        boolean correctset = false; //オセロを正しい位置にセットしたかどうか判断
        do {
          if (base[row][column] != 0) { //すでに置いてある場合
            M5.Lcd.fillCircle(60 + (column * 40), 20 + (row * 40), 15, 0xf9e9); //置けないことを赤丸で示す
            M5.update();
            delay(80);
            drawOthello();
            M5.Lcd.drawRect(41 + (column * 40), 1 + (row * 40), 39, 39, 0xff6e); //現地点を見やすく黄色で囲む
            if (player == 1) {
              M5.Lcd.drawCircle(60 + (column * 40), 20 + (row * 40), 15, 0xffff);
            } else if (player == 2) {
              M5.Lcd.drawCircle(60 + (column * 40), 20 + (row * 40), 15, 0x0000);
            }
            M5.update();
            break;
          }

          base[row][column] = player;
          int num = 0;  //ひっくり返す個数
          int i = 0;  //座標
          if (column != 0) { //左チェック
            for (i = column - 1; base[row][i] != 0 && base[row][i] != player; i--) {
              if (base[row][i] == opponent) {
                if (i == 0) {
                  num = 0; // 端まで相手の色ならひっくり返さない
                  break;
                }
                num++;
              }
            }
            if (base[row][i] == 0) { // 何も無ければひっくり返さない
              num = 0;
            }
            i++;
            for (int j = 0; j < num; j++) {
              base[row][i] = player;
              i++;
              correctset = true; // 正しく設置できた
              skip1 = false;
            }
            num = 0;
          }


          if (column != 5) {  //右チェック
            for (i = column + 1; base[row][i] != 0 && base[row][i] != player; i++) {
              if (base[row][i] == opponent) {
                if (i == 5) {
                  num = 0;
                  break;
                }
                num++;
              }
            }
            if (base[row][i] == 0) {
              num = 0;
            }
            i--;
            for (int j = 0; j < num; j++) {
              base[row][i] = player;
              i--;
              correctset = true;
              skip1 = false;
            }
            num = 0;
          }

          if (row != 0) { //上チェック
            for (i = row - 1; base[i][column] != 0 && base[i][column] != player; i--) {
              if (base[i][column] == opponent) {
                if (i == 0) {
                  num = 0;
                  break;
                }
                num++;
              }
            }
            if (base[i][column] == 0) {
              num = 0;
            }
            i++;
            for (int j = 0; j < num; j++) {
              base[i][column] = player;
              i++;
              correctset = true;
              skip1 = false;
            }
            num = 0;
          }

          if (row != 7) { //下チェック
            for (i = row + 1; base[i][column] != 0 && base[i][column] != player; i++) {
              if (base[i][column] == opponent) {
                if (i == 7) {
                  num = 0;
                  break;
                }
                num++;
              }
            }
            if (base[i][column] == 0) {
              num = 0;
            }
            i--;
            for (int j = 0; j < num; j++) {
              base[i][column] = player;
              i--;
              correctset = true;
              skip1 = false;
            }
            num = 0;
          }
          int h;// 追加の座標
          if (!(row == 0 || column == 0)) {
            for (i = row - 1, h = column - 1; base[i][h] != 0 && base[i][h] != player; i--, h--) { // 左上チェック
              if (base[i][h] == opponent) {
                if (i == 0 || h == 0) {
                  num = 0;
                  break;
                }
                num++;
              }
            }
            if (base[i][h] == 0) {
              num = 0;
            }
            i++;
            h++;
            for (int j = 0; j < num; j++) {
              base[i][h] = player;
              i++;
              h++;
              correctset = true;
              skip1 = false;
            }
            num = 0;
          }

          if (!(row == 5 || column == 0)) {
            for (i = row + 1, h = column - 1; base[i][h] != 0 && base[i][h] != player; i++, h--) { // 左下チェック
              if (base[i][h] == opponent) {
                if (i == 5 || h == 0) {
                  num = 0;
                  break;
                }
                num++;
              }
            }
            if (base[i][h] == 0) {
              num = 0;
            }
            i--;
            h++;
            for (int j = 0; j < num; j++) {
              base[i][h] = player;
              i--;
              h++;
              correctset = true;
              skip1 = false;
            }
            num = 0;
          }

          if (!(row == 0 || column == 5)) {
            for (i = row - 1, h = column + 1; base[i][h] != 0 && base[i][h] != player; i--, h++) { // 右上チェック
              if (base[i][h] == opponent) {
                if (i == 0 || h == 5) {
                  num = 0;
                  break;
                }
                num++;
              }
            }
            if (base[i][h] == 0) {
              num = 0;
            }
            i++;
            h--;
            for (int j = 0; j < num; j++) {
              base[i][h] = player;
              i++;
              h--;
              correctset = true;
              skip1 = false;
            }
            num = 0;
          }

          if (!(row == 5 || column == 5)) {
            for (i = row + 1, h = column + 1; base[i][h] != 0 && base[i][h] != player; i++, h++) { // 右下チェック
              if (base[i][h] == opponent) {
                if (i == 5 || h == 5) {
                  num = 0;
                  break;
                }
                num++;
              }
            }
            if (base[i][h] == 0) {
              num = 0;
            }
            i--;
            h--;
            for (int j = 0; j < num; j++) {
              base[i][h] = player;
              i--;
              h--;
              correctset = true;
              skip1 = false;
            }
            num = 0;
          }
          if (correctset == false) {
            base[row][column] = 0;
            M5.Lcd.fillCircle(60 + (column * 40), 20 + (row * 40), 15, 0xf9e9); //置けないことを赤丸で示す
            M5.update();
            delay(80);
            M5.Lcd.fillRect(41 + (column * 40), 1 + (row * 40), 39, 39, 0x5e02); //現地点を緑で塗りつぶす
            drawOthello();
            M5.Lcd.drawRect(41 + (column * 40), 1 + (row * 40), 39, 39, 0xff6e); //現地点を見やすく黄色で囲む
            if (player == 1) {
              M5.Lcd.drawCircle(60 + (column * 40), 20 + (row * 40), 15, 0xffff);
            } else if (player == 2) {
              M5.Lcd.drawCircle(60 + (column * 40), 20 + (row * 40), 15, 0x0000);
            }
            M5.update();
            break;
          }
          M5.Lcd.drawRect(41 + (column * 40), 1 + (row * 40), 39, 39, 0x5e02); //現地点の黄色枠を消す
          pattern = 2;
        } while (!correctset);

      } else if (column == 6) {
        M5.Lcd.drawRect(281, 200, 39, 40, 0xfb56);// ピンク枠
        M5.update();
        if (skip1 == true) {
          gameEnd();
        }
        skip1 = true;
        break;
      }
    } else if (M5.BtnC.isPressed()) { //仮のオセロを右に移動
      if (column != 5 && column != 6) {
        M5.Lcd.fillRect(41 + (column * 40), 1 + (row * 40), 39, 39, 0x5e02); //現地点を緑で塗りつぶす
        M5.update();
        pattern = 3;
        column++;
        drawOthello();  //オセロを描画
        M5.Lcd.drawRect(41 + (column * 40), 1 + (row * 40), 39, 39, 0xff6e); //現地点を見やすく黄色で囲む
        if (player == 1) {
          M5.Lcd.drawCircle(60 + (column * 40), 20 + (row * 40), 15, 0xffff); //仮の白オセロを描画
        } else if (player == 2) {
          M5.Lcd.drawCircle(60 + (column * 40), 20 + (row * 40), 15, 0x0000); //仮の黒オセロを描画
        }
      } else if (column == 5) {
        M5.Lcd.fillRect(41 + (column * 40), 1 + (row * 40), 39, 39, 0x5e02); //現地点を緑で塗りつぶす
        drawOthello();
        M5.Lcd.drawRect(281, 200, 39, 40, 0xff6e);  //skipを黄色で囲む
        M5.update();
        column++;
      } else if (column == 6) {
        M5.Lcd.drawRect(281, 200, 39, 40, 0xfb56);  //skipを元の色で囲む
        M5.update();
        column = 0;
        drawOthello();  //オセロを描画
        M5.Lcd.drawRect(41 + (column * 40), 1 + (row * 40), 39, 39, 0xff6e); //現地点を見やすく黄色で囲む
        if (player == 1) {
          M5.Lcd.drawCircle(60 + (column * 40), 20 + (row * 40), 15, 0xffff); //仮の白オセロを描画
        } else if (player == 2) {
          M5.Lcd.drawCircle(60 + (column * 40), 20 + (row * 40), 15, 0x0000); //仮の黒オセロを描画
        }
      }
    }
    M5.update();
    delay(120);
  }
  //M5.Lcd.drawRect(41 + (column * 40), 1 + (row * 40), 39, 39, 0x5e02); //現地点の黄色枠を消す
}

void drawOthello() {
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      switch (base[i][j]) {
        case 0:
          break;  //何も置いていない
        case 1:
          M5.Lcd.fillCircle(60 + (j * 40), 20 + (i * 40), 15, 0xffff); //白のオセロを描画
          break;
        case 2:
          M5.Lcd.fillCircle(60 + (j * 40), 20 + (i * 40), 15, 0x0000); //黒のオセロを描画
          break;
      }
    }
  }
}

void drawGameLine() {
  M5.Lcd.fillRect(41, 1, 239, 239, 0x5e02);
  for (int i = 1; i < 8; i++) {
    M5.Lcd.drawLine(i * 40, 0, i * 40, 240, 0xffff);
    M5.Lcd.drawLine(40, (i - 1) * 40, 280, (i - 1) * 40, 0xffff);
  }
  M5.Lcd.drawLine(40, 239, 280, 239, 0xffff);
}

void drawSkip() {
  M5.Lcd.drawRect(281, 200, 39, 40, 0xfb56);
  M5.Lcd.setCursor(290, 216);
  M5.Lcd.print("Skip");
}

void gameEnd() {
  int a = 0;
  int b = 0;
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      switch (base[i][j]) {
        case 0:
          break;
        case 1:
          a++;
          break;
        case 2:
          b++;
          break;
      }
    }
  }

  M5.Lcd.fillCircle(20, 20, 15, 0xffff);
  M5.Lcd.drawCircle(300, 20, 15, 0xffff);
  M5.Lcd.setCursor(10, 50);
  M5.Lcd.print(a);
  M5.Lcd.setCursor(290, 50);
  M5.Lcd.print(b);

  if (a > b) {
    M5.Lcd.setCursor(10, 90);
    M5.Lcd.print("win");
    M5.Lcd.setCursor(290, 90);
    M5.Lcd.print("lose");
  } else if (a < b) {
    M5.Lcd.setCursor(10, 90);
    M5.Lcd.print("lose");
    M5.Lcd.setCursor(290, 90);
    M5.Lcd.print("win");
  } else {
    M5.Lcd.setCursor(10, 90);
    M5.Lcd.print("draw");
    M5.Lcd.setCursor(290, 90);
    M5.Lcd.print("draw");
  }

  M5.update();
  while (1) {
    //終了
  }
}

