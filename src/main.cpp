#include <M5EPD.h>
File root;
M5EPD_Canvas canvas(&M5.EPD);
static char* file_name_pointers[100];//文字列へのポインタの配列
static int n_file = 0;//ファイル数
static int selected_file_number=0;//ファイル位置を保持

/**
* 文字列が特定の文字列で終わるか？ 
*/
bool ends_with(const std::string& str, const std::string& suffix) {
  size_t len1 = str.size();
  size_t len2 = suffix.size();
  return len1 >= len2 && str.compare(len1 - len2, len2, suffix) == 0;
}

/**
* 文字列を辞書順にソート
*/
void SortByName()
{
	int i, j;
	char* buf;//比較時逃がす

	for (i = 0; i < n_file - 1; i++) {
		//整列済みの箇所は探索不要なので(j>i)
		for (j = n_file - 1; j > i; j--) {
			if (strcmp(file_name_pointers[j-1], file_name_pointers[j]) > 0) {
				buf = file_name_pointers[j];
				file_name_pointers[j] = file_name_pointers[j - 1];
				file_name_pointers[j - 1] = buf;
			}
		}
	}
}

/**
* .jpgのファイル名のポインタをfile_name_pointers配列に格納
*/
void roadDirectory(File dir) {
  int i=0;
  const char *name;
  while(true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      dir.rewindDirectory();
      break;
    }
    name = entry.name();
    Serial.printf("file_name:%s\n",name);
    if(ends_with(name,".jpg") || ends_with(name,".JPG")){
      file_name_pointers[i] = (char*)malloc(strlen(name) + 1);//文字列へのポインタを確保
      strcpy(file_name_pointers[i], name);//文字列へのポインタをコピー
      i++;
    }
  }
  n_file = i;
  // 配列をソート(デフォルトでは不思議な順番になるので名前順に変更)
  SortByName();
  
}

/** 
* ファイル読み込み確認用
*/
void PrintLines(int num)
{
	int i;
	for (i = 0; i < num; i++){
		Serial.printf("%s\n", file_name_pointers[i]);
  }
}

/** 
* 指定された位置のファイルを描画
*/
void drawImg(int number){
  Serial.printf("drawing%d:%s\n", number,file_name_pointers[number]);
  canvas.drawJpgFile(SD,file_name_pointers[number]); 
  canvas.pushCanvas(0,0,UPDATE_MODE_GC16);
}

/** 
* バッテリー確認処理
*/
int Chk_battery()
{
  char buf[20];
  uint32_t vol = M5.getBatteryVoltage();
  if (vol < 3300)
  {
    vol = 3300;
  }
  else if (vol > 4350)
  {
    vol = 4350;
  }
  float battery = (float)(vol - 3300) / (float)(4350 - 3300);
  if (battery <= 0.01)
  {
    battery = 0.01;
  }
  if (battery > 1)
  {
    battery = 1;
  }
  //uint8_t px = battery * 25;
  // sprintf(buf, "BATT%d%% ", (int)(battery * 100));
  // canvas.drawString(buf, 450, 0);

  return (int)(battery * 100);
}

/** 
* スライドショーにして良いファイルかを判定する
*/
bool is_slide(int slideshow_number){
  if(ends_with(file_name_pointers[slideshow_number],"_noslide.jpg") || ends_with(file_name_pointers[slideshow_number],"_noslide.JPG")){
    return false;
  }else{
    return true;
  }
}

/** 
* Arduino共通のセットアップ処理
*/
void setup()
{
  M5.begin();
  SD.begin();

  //描画初期化
  M5.EPD.SetRotation(90);
  M5.EPD.Clear(true);
  canvas.createCanvas(540, 960);

  //ファイル読み込み
  root = SD.open("/");
  roadDirectory(root);

  if(n_file ==0){
    // ファイル無いときのエラーハンドリング大変なので停止せる
    Serial.println("no file");
    canvas.setTextSize(2);
    canvas.drawString("File Naiyo Exception", 45, 350);

    // 気温取得
    M5.SHT30.Begin();//温度計周り
    M5.SHT30.UpdateData();
    float tem = M5.SHT30.GetTemperature();
    float hum = M5.SHT30.GetRelHumidity();
    char temStr[10];
    char humStr[10];
    dtostrf(tem, 2, 2, temStr); //小数点を含む数値を文字列に変換
    dtostrf(hum, 2, 2, humStr);
    canvas.setTextSize(2);
    canvas.drawString("Temp:" + String(temStr) + "*C  " + "Humi:" + String(humStr) + "%", 0, 0);

    // 描画
    canvas.pushCanvas(0, 0, UPDATE_MODE_GC16);
    delay(2000);
    //終了
    M5.shutdown(86400);//1日停止

  }else{
    PrintLines(n_file);
    drawImg(0);
  }
}

void loop() {
  int slideshow_number;

  M5.update();

    // バッテリー残量取得
  int btLevel = Chk_battery();

  //充電中は画面更新しないようにする
  if(btLevel==1){
    //充電時の処理
    canvas.createCanvas(540, 960);
    canvas.setTextSize(6);
    canvas.drawString("Charging....", 45, 350);
    canvas.pushCanvas(0, 0, UPDATE_MODE_GC16);
    delay(60000);
  }else{
    //ファイル位置設定
    if(M5.BtnL.isPressed()){
      //37番ピンに対応(上ボタン)

      selected_file_number++;
      if(selected_file_number>(n_file-1)){
        //末端まで行ったら最初に戻る
        selected_file_number=0;
      }
      drawImg(selected_file_number);
      delay(1000);
    }else if(M5.BtnR.isPressed()){
      //39番ピンに対応(下ボタン)

      selected_file_number--;
      if(selected_file_number<0){
        //先端まで行ったら最後に戻る
        selected_file_number=(n_file-1);
      }
      drawImg(selected_file_number);
      delay(500);
    }else if(M5.BtnP.isPressed()){
      slideshow_number= selected_file_number+1;
      Serial.printf("Enable:slideshowMode\n");
      //スライドショーモード
      while(true){
        //上でインクリメントしてるので==n_file-1でなく>=にしてる
        slideshow_number = (slideshow_number>=(n_file-1)) ? 0 : slideshow_number+1;
        if(is_slide(slideshow_number)){
          drawImg(slideshow_number);
          delay(10000);
        }
      }
    }
  }
}