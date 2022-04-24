type t

external init_genrand : int32 -> t = "init_genrand"
external init_by_array : int32 array -> t = "init_by_array"
external init_by_state : int -> int32 array -> t = "init_by_state"
external genrand_int : t -> int = "genrand_int31"
external genrand_int32 : t -> int32 = "genrand_int32"
external genrand_int64 : t -> int64 = "genrand_int64"
external genrand_real1 : t -> float = "genrand_real1"
external genrand_real2 : t -> float = "genrand_real2"
external genrand_real3 : t -> float = "genrand_real3"
external genrand_res53 : t -> float = "genrand_res53"
external copy : t -> t = "copy"
external get_state : t -> int * int32 array = "get_state"

