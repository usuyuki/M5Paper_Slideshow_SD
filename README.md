<!-- @format -->

# M5Paper_Slideshow_SD

M5Paper で SD カード に入った jpg 画像を名前順に表示するコード。M5Paper のスイッチで進んだり戻ったりが可能。(1~100 枚まで多分対応)  
※突貫工事で作ったコードです。グローバル変数を使い倒してたりします。

上 → 次の画像  
下 → 前の画像  
中央 → 停止

![IMG20220624134241](https://user-images.githubusercontent.com/63891531/175464271-c0326cc6-5f05-4ccd-9c8d-d8a0944e3832.jpg)

画像がないときは「File Naiyo Exception」と温度湿度を表示して停止。

# M5Paper の公式ドキュメント

https://docs.m5stack.com/en/api/m5paper/system_api

# jpg 画像の制約

## 場所

SD カード直下。

## サイズ

540x960 が最適。  
サイズ違っても表示はできる（収まりきらないが）

## 形式

jpg だが謎の制約あり。

Windows ペイントで上書き保存するとそのまま可能。

### GIMP で出力時

最適化とプログレッシブにチェックを外さないと M5 で表示されない  
<img width="387" alt="image" src="https://user-images.githubusercontent.com/63891531/175463364-5bda70db-d46b-4529-946e-4c641e8cc308.png">
