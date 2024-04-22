# Echoserv
約2ヶ月前、学校の課題でC++98を使って、nginxに似たHTTPサーバーを作成しました。

復習も兼ねて、サンプルコードを作成しました。

このサンプルコードはEchoサーバーで、ブラウザやcurlなどのHTTPクライアントから受け取ったリクエストメッセージをそのまま送り返します。非同期I/Oのためにpoll()を使用し、ソケットをノンブロッキングモードにしています。

HTTPサーバーそのものではありませんが、このサンプルを理解することで、技術的な側面ではnginxのようなHTTPサーバーの実装に必要な基礎知識が身につくと考えています。ただし、システムコールや関数、マクロなどの詳細な説明は含まれていないため、足りない部分は自身で調べる必要があります。

最低限の説明はコメントで記載していますが、わかりにくい部分があるかもしれません。ChatGPTやClaude等のAIを活用し、コードリーディングすることをお勧めします。

## 実行方法

1. codeディレクトリに移動
```
cd code
```
2. codeディレクトリでmakeを実行
```
make
```
3. 実行するとcodeディレクトリにechoservという実行ファイルができますので、
引数にポート番号を渡す。
```
./echoserv 8080 4242
```
4. curlを使って、以下のようなコマンドでリクエストの送受信ができます。
```
// 8080でリッスンしている時
curl localhost:8080
```
5. 以下のようにURLをブラウザのURL欄に入力することでも、ブラウザでリクエストの送受信ができます。
```
// 8080でリッスンしている時
http://localhost:8080/
```

## HTTPサーバー
今回書いたEchoサーバーだけでなく、
課題で実際に書いたHTTPサーバーのコードも見たいという方は以下のURLからリポジトリに行けます。
https://github.com/deerman31/http_server_webserv