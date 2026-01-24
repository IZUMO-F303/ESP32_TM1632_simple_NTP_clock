# ESP32 NTP Clock with TM1637 Display

ESP32とTM1637 7セグメントディスプレイを使用したNTPデジタル時計です。

## 主な機能

*   WiFi経由でNTPサーバーから時刻を自動取得
*   取得した時刻を4桁の7セグメントディスプレイに表示 (HH:MM形式)
*   1時間ごとに時刻を自動で再同期
*   時間帯に応じてディスプレイの輝度を自動調整 (7:00-18:59 は明るく、それ以外は暗く)

## 必要なもの

### ハードウェア

*   ESP32開発ボード
*   TM1637搭載 4桁7セグメントディスプレイ
*   配線用のジャンパーワイヤー

### ソフトウェア

*   [Visual Studio Code](https://code.visualstudio.com/)
*   [PlatformIO IDE](https://platformio.org/)

## セットアップ

1.  **配線:**
    ESP32とTM1637ディスプレイを以下のように接続します。

| TM1637 | ESP32 |
| :----: | :---: |
|  CLK   | GPIO17 |
|  DIO   | GPIO16 |
|  VCC   |  3.3V  |
|  GND   |  GND   |

2.  **WiFi設定:**
    `include` フォルダに `wifi_credentials.h` という名前でファイルを新規作成し、以下の内容を記述してお使いのWiFiアクセスポイントのSSIDとパスワードに置き換えてください。

    ```cpp
    #pragma once

    const char* ssid = "YOUR_WIFI_SSID";
    const char* password = "YOUR_WIFI_PASSWORD";
    ```

3.  **ライブラリのインストール:**
    このプロジェクトはPlatformIOを使用しているため、`platformio.ini` に記載されている依存ライブラリ (`smougenot/TM1637`) は自動的にインストールされます。

## ビルドと書き込み

1.  このプロジェクトをPlatformIOで開きます。
2.  PlatformIOのサイドバーにある「Project Tasks」から「Upload」を選択し、プログラムをESP32に書き込みます。

## トラブルシューティング

*   **時刻が表示されない:**
    *   シリアルモニターを開き、WiFiへの接続やNTPサーバーとの同期が成功しているか確認してください。
    *   `include/wifi_credentials.h` の内容が正しいか確認してください。
    *   配線が正しいか確認してください。
*   **ビルドに失敗する:**
    *   PlatformIOが正しくインストールされているか確認してください。
