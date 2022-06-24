<!-- @format -->

# M5Paper_Slideshow_SD

M5PaperのスイッチでSDに入ったjpg画像を表示するコード

上 → 次の画像  
下 → 前の画像  
中央 → 停止

画像がないときは「File Naiyo Exception」と温度湿度を表示して停止。  


# M5Paperの公式ドキュメント

https://docs.m5stack.com/en/api/m5paper/system_api

# jpg 画像の制約
## 場所
SDカード直下。

## サイズ
540x960が最適。  
サイズ違っても表示はできる（収まりきらないが）

## 形式
jpgだが謎の制約あり。

Windows ペイントで上書き保存するとそのまま可能。

### GIMPで出力時  
最適化とプログレッシブにチェックを外さないとM5で表示されない  
<img width="387" alt="image" src="https://user-images.githubusercontent.com/63891531/175463364-5bda70db-d46b-4529-946e-4c641e8cc308.png">
