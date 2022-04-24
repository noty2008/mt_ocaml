# 使い方
mersenne_twister.ml と mt_c.c をコンパイルしてリンクすることで、モジュール Mersenne_twister を使えるようになります。
　モジュール Mersenne_twister では、次の型や関数が定義されています。

　　type t

　　　Mersenne Twister を表す型です。t 型の値には、Mersenne Twister の内部状態が保存されています。


　　val init_genrand : int32 -> t

　　　int32 型の整数値をシード値として、新しい Mersenne Twister を生成します。


　　val init_by_array : int32 array -> t

　　　int32 型の配列をシード値として、新しい Mersenne Twister を生成します。


　　val init_by_state : int -> int32 array -> t

　　　関数 get_state（後述）で取得した内部状態から、Mersenne Twister を復元します。

　　　関数 get_state によって得られたもの以外を引数として与えた場合は、期待した結果が得られない可能性があります。


　　val genrand_int : t -> int

　　　Mersenne Twister から新たな int 型の乱数を得ます。

　　　乱数の範囲 : -1,073,741,824 ～ +1,073,741,823 


　　val genrand_int32 : t -> int32

　　　Mersenne Twister から新たな int32 型の乱数を得ます。

　　　乱数の範囲 : -2,147,483,648 ～ +2,147,483,647 


　　val genrand_int64 : t -> int64

　　　Mersenne Twister から新たな int64 型の乱数を得ます。

　　　乱数の範囲 : -9,223,372,036,854,775,808 ～ +9,223,372,036,854,775,807 


　　val genrand_real1 : t -> float

　　　Mersenne Twister から新たな float 型の乱数を得ます。

　　　乱数の範囲 : 0 以上 1 以下 


　　val genrand_real2 : t -> float

　　　Mersenne Twister から新たな float 型の乱数を得ます。

　　　乱数の範囲 : 0 以上 1 未満 


　　val genrand_real3 : t -> float

　　　Mersenne Twister から新たな float 型の乱数を得ます。

　　　乱数の範囲 : 0 超 1 未満 


　　val genrand_res53 : t -> float

　　　Mersenne Twister から新たな float 型の乱数を得ます。

　　　乱数の範囲 : 0 以上 1 未満 


　　val copy : t -> t

　　　引数で与えたものと全く同じ内部状態を持つ Mersenne Twister を複製し、返します。


　　val get_state : t -> int * int32 array

　　　Mersenne Twister の内部状態を得ます。

　　　これを保存しておけば、前述の関数 init_by_state を用いてこの時の状態を復元できます。


　使う際は、最初に1度 init_genrand や init_by_array で Mersenne Twister を生成し、以降はそれを genrand_int 等の引数に与えることで乱数を取得してください。
