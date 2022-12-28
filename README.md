# CallerCaller

A VST3 which outputs MIDI note-on(48-C3 ch.1) when volume exceeds threshold

音量が閾値を超えたときにMIDIノートオンを送信するVST3プラグイン

https://twitter.com/koisignal1/status/1596540314629468160?s=20&t=88NK6IK4GnIBHKriFVgUew

### 使い方

・thresholdで閾値となる音量をデシベルで指定します。　

・閾値を超えるとMIDI ch.1のC3(ノートナンバー48)のノートオンが出力されます。
 
・lengthで設定したミリ秒後にノートオフが出力されます。

ノートオンの出力先にお好きなインストゥルメントを接続することで、
音量に応じて音を鳴らすことができます。

例えば、
CubaseのGroove Agent SE、
Ableton LiveのSimplerに接続することで、
任意のwavファイルを鳴らすことができるでしょう。

### 類似プラグイン

Ableton live Suiteユーザの方は、
slynk氏が無料で配布しているDuck CallというM4Lプラグインで同等以上のことができます。
https://slynk.gumroad.com/

