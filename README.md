<!-- @format -->

# M5Paper_Slideshow_SD

M5Paper で SD カード に入った jpg 画像を名前順に表示するコード。M5Paper のスイッチで進んだり戻ったりが可能。(1~100 枚まで多分対応)  
※突貫工事で作ったコードです。グローバル変数を使い倒してたりします。

上 → 次の画像  
下 → 前の画像  
中央 → 10 秒ごとのスライドショーループ(電源を切るまで,ファイル名の末尾が\_noslide のものは除外)


https://user-images.githubusercontent.com/63891531/175484731-62abc46d-aadb-4193-9f0c-cb3b28e2b4b1.mp4


画像がないときは「File Naiyo Exception」と温度湿度を表示して停止。

# M5Paper の公式ドキュメント

https://docs.m5stack.com/en/api/m5paper/system_api

# jpg 画像の制約

## 場所

SD カード直下。  
辞書順に表示されるので、明示的に順番を指定したいときは 0\_など数字プレフィックスを付けることをおすすめ。

## 形式

- 540x960 が最適。  
  サイズ違っても表示はできる（収まりきらないが）
- 拡張子は.jpg または.JPG  
   m5 的には png も行けるが表示におそろしく時間掛かるので対象外とした
- ファイル名は英数字のみ  
   内部でソートを行う際に日本語扱えないため
- ファイル名の長さは制限なし  
   文字列ポインタの配列で管理しているため
- ファイル名の末尾に\_noslide を付けるとスライドショーでは表示しないようにできる  
   例:5_zaikonasi_noslide.jpg
- 謎の制約あり
  プログレッシブ jpg や最適化していると表示できません……(下を参考)

### GIMP で出力時

下記のように最適化とプログレッシブにチェックを外すと m5 で表示できる  
<img width="387" alt="image" src="https://user-images.githubusercontent.com/63891531/175463364-5bda70db-d46b-4529-946e-4c641e8cc308.png">

Windows ペイントで上書き保存した jpg はそのまま可能。
