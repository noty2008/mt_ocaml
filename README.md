擬似乱数生成アルゴリズム Mersenne Twisterを OCaml で使えるライブラリにしました。
# 使い方
mersenne_twister.ml と mt_c.c をコンパイルしてリンクすることで、モジュール Mersenne_twister を使えるようになります。
　モジュール Mersenne_twister では、次の型や関数が定義されています。

```type t```


　　　Mersenne Twister を表す型です。t 型の値には、Mersenne Twister の内部状態が保存されています。


　　```val init_genrand : int32 -> t```

　　　int32 型の整数値をシード値として、新しい Mersenne Twister を生成します。


　　```val init_by_array : int32 array -> t```

　　　int32 型の配列をシード値として、新しい Mersenne Twister を生成します。


　　```val init_by_state : int -> int32 array -> t```

　　　関数 get_state（後述）で取得した内部状態から、Mersenne Twister を復元します。

　　　関数 get_state によって得られたもの以外を引数として与えた場合は、期待した結果が得られない可能性があります。


　　```val genrand_int : t -> int```

　　　Mersenne Twister から新たな int 型の乱数を得ます。

　　　乱数の範囲 : -1,073,741,824 ～ +1,073,741,823 


　　```val genrand_int32 : t -> int32```

　　　Mersenne Twister から新たな int32 型の乱数を得ます。

　　　乱数の範囲 : -2,147,483,648 ～ +2,147,483,647 


　　```val genrand_int64 : t -> int64```

　　　Mersenne Twister から新たな int64 型の乱数を得ます。

　　　乱数の範囲 : -9,223,372,036,854,775,808 ～ +9,223,372,036,854,775,807 


　　```val genrand_real1 : t -> float```

　　　Mersenne Twister から新たな float 型の乱数を得ます。

　　　乱数の範囲 : 0 以上 1 以下 


　　```val genrand_real2 : t -> float```

　　　Mersenne Twister から新たな float 型の乱数を得ます。

　　　乱数の範囲 : 0 以上 1 未満 


　　```val genrand_real3 : t -> float```

　　　Mersenne Twister から新たな float 型の乱数を得ます。

　　　乱数の範囲 : 0 超 1 未満 


　　```val genrand_res53 : t -> float```

　　　Mersenne Twister から新たな float 型の乱数を得ます。

　　　乱数の範囲 : 0 以上 1 未満 


　　```val copy : t -> t```

　　　引数で与えたものと全く同じ内部状態を持つ Mersenne Twister を複製し、返します。


　　```val get_state : t -> int * int32 array```

　　　Mersenne Twister の内部状態を得ます。

　　　これを保存しておけば、前述の関数 init_by_state を用いてこの時の状態を復元できます。


　使う際は、最初に1度 init_genrand や init_by_array で Mersenne Twister を生成し、以降はそれを genrand_int 等の引数に与えることで乱数を取得してください。

# コンパイル・実行の仕方
OCaml 環境はインストール済とします。
以下、Windows + Visual C++ と、Linux + GCC での方法を記します。他の環境でもほぼ同様です。

## インタプリタでの使い方
### Windows + Visual C++ の場合
Visual Studio コマンドプロンプトで次のようにすると、インタプリタ mt_interp.exe を生成できます。ユーザが打つのは太字の部分です。
生成した mt_interp.exe は普通にインタプリタとして使用でき、かつモジュール Mersenne_twister が使えます。
```
cl /c /I "C:\Program Files\Objective Caml\lib" mt_c.c
mt_c.c をコンパイルして mt_c.obj を作る, リンクは行わない（/c オプション）
"C:\Program Files\Objective Caml\lib" の部分は、OCaml のインストール状況に応じて適切に指定する
> ocamlmktop -custom mt_c.obj mersenne_twister.ml -o mt_interp.exe
Mersenne Twister を含めてインタプリタ mt_interp.exe を生成
```

### Linux + GCC の場合
ターミナル画面で次のようにすると、インタプリタ mt_interp を生成できます。ユーザが打つのは太字の部分です。
生成した mt_interp.exe は普通にインタプリタとして使用でき、かつモジュール Mersenne_twister が使えます。
```
$ gcc -c -I /usr/lib/ocaml mt_c.c
mt_c.c をコンパイルして mt_c.o を作る, リンクは行わない（-c オプション）
/usr/lib/ocaml の部分は、OCaml のインストール状況に応じて適切に指定する
$ ocamlmktop -custom mt_c.o mersenne_twister.ml -o mt_interp
Mersenne Twister を含めてインタプリタ mt_interp を生成
```

## バイトコードコンパイラ ocamlc での使い方
### Windows + Visual C++ の場合
モジュール Mersenne_twister を含む OCaml プログラム mt_test.ml を ocamlc でコンパイルするには、Visual Studio コマンドプロンプトで次のようにします。
ユーザが打つのは太字の部分です。
```
> cl /c /I "C:\Program Files\Objective Caml\lib" mt_c.c
mt_c.c をコンパイルして mt_c.obj を作る, リンクは行わない（/c オプション）
"C:\Program Files\Objective Caml\lib" の部分は、OCaml のインストール状況に応じて適切に指定する
> ocamlc -custom mt_c.obj mersenne_twister.ml mt_test.ml -o mt_test.exe
Mersenne Twister を含めて mt_test.ml をコンパイルし、実行ファイル mt_test.exe を生成
```

### Linux + GCC の場合
モジュール Mersenne_twister を含む OCaml プログラム mt_test.ml を ocamlc でコンパイルするには、ターミナル画面で次のようにします。
ユーザが打つのは太字の部分です。
```
$ gcc -c -I /usr/lib/ocaml mt_c.c
mt_c.c をコンパイルして mt_c.o を作る, リンクは行わない（-c オプション）
/usr/lib/ocaml の部分は、OCaml のインストール状況に応じて適切に指定する
$ ocamlc -custom mt_c.o mersenne_twister.ml mt_test.ml -o mt_test
Mersenne Twister を含めて mt_test.ml をコンパイルし、実行ファイル mt_test を生成
```

## ネイティブコードコンパイラ ocamlopt での使い方
### Windows + Visual C++ の場合
モジュール Mersenne_twister を含む OCaml プログラム mt_test.ml を ocamlopt でコンパイルするには、Visual Studio コマンドプロンプトで次のようにします。
ユーザが打つのは太字の部分です。
```
> cl /c /I "C:\Program Files\Objective Caml\lib" mt_c.c
mt_c.c をコンパイルして mt_c.obj を作る, リンクは行わない（/c オプション）
"C:\Program Files\Objective Caml\lib" の部分は、OCaml のインストール状況に応じて適切に指定する
> ocamlopt mt_c.obj mersenne_twister.ml mt_test.ml -o mt_test.exe
Mersenne Twister を含めて mt_test.ml をコンパイルし、実行ファイル mt_test.exe を生成
```

### Linux + GCC の場合
モジュール Mersenne_twister を含む OCaml プログラム mt_test.ml を ocamlopt でコンパイルするには、ターミナル画面で次のようにします。
ユーザが打つのは太字の部分です。
```
$ gcc -c -I /usr/lib/ocaml mt_c.c
mt_c.c をコンパイルして mt_c.o を作る, リンクは行わない（-c オプション）
/usr/lib/ocaml の部分は、OCaml のインストール状況に応じて適切に指定する
$ ocamlopt mt_c.o mersenne_twister.ml mt_test.ml -o mt_test
Mersenne Twister を含めて mt_test.ml をコンパイルし、実行ファイル mt_test を生成
```

## 注意事項
本ライブラリでは mt_c.c の中で stdint.h を include して使用していますが、コンパイラのバージョンによっては stdint.h が利用できない場合があります。
その場合は、mt_c.c 中の #include <stdint.h> を削除し、typedef unsigned long uint32_t のような記述を追加してください
よくわからない場合は、最新版の Visual C++を使うことをお勧めします。
このプログラムを利用したことにより生じる如何なる損害に対しても、その原因の如何を問わず、開発者は一切の責任を負いません。　

## Mersenne Twister について
Mersenne Twisterは、松本眞先生と西村拓士先生が開発なさった優れた擬似乱数生成アルゴリズムです。著作権はこの両名に属します。
Mersenne Twister はBSDライセンスに基づき無償かつ無保証にて頒布されるものです。
