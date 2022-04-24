#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <caml/alloc.h>
#include <caml/memory.h>
#include <caml/custom.h>

/* Period parameters */  
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

typedef struct _mt_t {
	uint32_t mta[N];	/* the array for the state vector  */
	int mti;
} mt_t;

static struct custom_operations objst_custom_ops = {
	"mti handling",
	custom_finalize_default,
	custom_compare_default,
	custom_hash_default,
	custom_serialize_default,
	custom_deserialize_default
};


/**********************************************/
/* functions for internal implementation in C */
/**********************************************/

/* initializes mta[N] with a seed */
void init_by_seed(uint32_t mta[N], uint32_t s){
	int i;

	mta[0] = s;
	for (i = 1; i < N; i++){
		mta[i] = (1812433253UL * (mta[i - 1] ^ (mta[i - 1] >> 30)) + i); 
	}
}

/* generates a random number on [0,0xffffffff]-interval */
uint32_t genrand(value mt_caml){
	uint32_t y;
	static uint32_t mag01[2] = {0x0UL, MATRIX_A};
	mt_t *mt = (mt_t *)Data_custom_val(mt_caml);

	/* mag01[x] = x * MATRIX_A  for x = 0, 1 */
	if (mt->mti >= N) { /* generate N words at one time */
		int kk;

		for (kk = 0; kk < N - M; kk++){
			y = (mt->mta[kk] & UPPER_MASK) | (mt->mta[kk + 1] & LOWER_MASK);
			mt->mta[kk] = mt->mta[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}
		for (; kk < N - 1; kk++){
			y = (mt->mta[kk] & UPPER_MASK) | (mt->mta[kk + 1] & LOWER_MASK);
			mt->mta[kk] = mt->mta[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}
		y = (mt->mta[N - 1] & UPPER_MASK) | (mt->mta[0] & LOWER_MASK);
		mt->mta[N - 1] = mt->mta[M - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];

		mt->mti = 0;
	}
	y = mt->mta[mt->mti++];

	/* Tempering */
	y ^= (y >> 11);
	y ^= (y << 7) & 0x9d2c5680UL;
	y ^= (y << 15) & 0xefc60000UL;
	y ^= (y >> 18);

	return y;
}


/****************************************/
/* functions for interfacing with OCaml */
/****************************************/

/* initializes mt_t with a seed */
CAMLprim value init_genrand(value s){
	CAMLparam1(s);
	mt_t mt;

	CAMLlocal1(mt_caml);
	mt_caml = caml_alloc_custom(&objst_custom_ops, sizeof(mt_t), 0, 1);
	init_by_seed(mt.mta, Int32_val(s));
	mt.mti = N;
	memcpy(Data_custom_val(mt_caml), &mt, sizeof(mt_t));

	CAMLreturn(mt_caml);
}

/* initialize by an array with array-length */
CAMLprim value init_by_array(value init_key){
	CAMLparam1(init_key);
	int i = 1, j = 0, k, len = Wosize_val(init_key);
	mt_t mt;

	CAMLlocal1(mt_caml);
	mt_caml = caml_alloc_custom(&objst_custom_ops, sizeof(mt_t), 0, 1);
	init_by_seed(mt.mta, 19650218UL);
	mt.mti = N;

	for (i = 1, k = (N > len ? N : len); k; k--){
		mt.mta[i] = (mt.mta[i] ^ ((mt.mta[i - 1] ^ (mt.mta[i - 1] >> 30)) * 1664525UL)) + Int32_val(Field(init_key, j)) + j;	/* non linear */
		i++; j++;
		if (i >= N){
			mt.mta[0] = mt.mta[N - 1];
			i = 1;
		}
		if (j >= len) j = 0;
	}
	for (k = N - 1; k; k--){
		mt.mta[i] = (mt.mta[i] ^ ((mt.mta[i - 1] ^ (mt.mta[i - 1] >> 30)) * 1566083941UL)) - i;	/* non linear */
		i++;
		if (i >= N){
			mt.mta[0] = mt.mta[N - 1];
			i = 1;
		}
	}
	mt.mta[0] = 0x80000000UL;	/* MSB is 1; assuring non-zero initial array */
	memcpy(Data_custom_val(mt_caml), &mt, sizeof(mt_t));

	CAMLreturn(mt_caml);
}

/* generates a random number on [0,0xffffffff]-interval */
CAMLprim value genrand_int32(value mt_caml){
	CAMLparam1(mt_caml);
	CAMLreturn(copy_int32(genrand(mt_caml)));
}

/* generates a random number on [0,0x7fffffff]-interval */
CAMLprim value genrand_int31(value mt_caml){
	CAMLparam1(mt_caml);
	CAMLreturn(Val_int(genrand(mt_caml) >> 1));
}

/* generates a random number on [0,1]-real-interval */
CAMLprim value genrand_real1(value mt_caml){
	CAMLparam1(mt_caml);
	CAMLreturn(copy_double(genrand(mt_caml) * (1.0 / 4294967295.0)));	/* divided by 2 ^ 32 - 1 */
}

/* generates a random number on [0,1)-real-interval */
CAMLprim value genrand_real2(value mt_caml){
	CAMLparam1(mt_caml);
	CAMLreturn(copy_double(genrand(mt_caml) * (1.0 / 4294967296.0)));	/* divided by 2 ^ 32 */
}

/* generates a random number on (0,1)-real-interval */
CAMLprim value genrand_real3(value mt_caml){
	CAMLparam1(mt_caml);
	CAMLreturn(copy_double((genrand(mt_caml) + 0.5) * (1.0 / 4294967296.0)));	/* divided by 2 ^ 32 */
}

/* generates a random number on [0,1) with 53-bit resolution */
CAMLprim value genrand_res53(value mt_caml){
	CAMLparam1(mt_caml);
	uint32_t a = genrand(mt_caml) >> 5, b = genrand(mt_caml) >> 6;
	CAMLreturn(copy_double((a * 67108864.0 + b) * (1.0 / 9007199254740992.0)));
}
/* These real versions are due to Isaku Wada, 2002/01/09 added */


/************************/
/* additional functions */
/************************/

/* generates a random number on [0,0xffffffffffffffff]-interval */
CAMLprim value genrand_int64(value mt_caml){
	CAMLparam1(mt_caml);
	CAMLreturn(copy_int64(((uint64_t)genrand(mt_caml) << 32) | genrand(mt_caml)));
}

/* make a copy */
CAMLprim value copy(value mt_caml){
	CAMLparam1(mt_caml);

	CAMLlocal1(mt_caml_new);
	mt_caml_new = caml_alloc_custom(&objst_custom_ops, sizeof(mt_t), 0, 1);
	memcpy(Data_custom_val(mt_caml_new), Data_custom_val(mt_caml), sizeof(mt_t));

	CAMLreturn(mt_caml_new);
}

/* converts an mt_t state into OCaml style */
CAMLprim value get_state(value mt_caml){
	CAMLparam1(mt_caml);
	int i;
	mt_t *mt = (mt_t *)Data_custom_val(mt_caml);

	CAMLlocal2(tuple, array);
	tuple = caml_alloc(2, 0);
	array = caml_alloc(N, 0);

	Store_field(tuple, 0, Val_int(mt->mti));
	Store_field(tuple, 1, array);
	for (i = 0; i < N; i++) Store_field(array, i, copy_int32(mt->mta[i]));

	CAMLreturn(tuple);
}

/* initialize by mti and mta[N] */
CAMLprim value init_by_state(value mti_caml, value mta_caml){
	CAMLparam2(mti_caml, mta_caml);
	int i = Wosize_val(mta_caml) - 1;
	mt_t mt;

	CAMLlocal1(mt_caml);
	mt_caml = caml_alloc_custom(&objst_custom_ops, sizeof(mt_t), 0, 1);
	init_by_seed(mt.mta, 19650218UL);
	mt.mti = Int_val(mti_caml);
	if (mt.mti > N) mt.mti = N;

	if (N <= i) i = N - 1;
	for (; i >= 0; i--) mt.mta[i] = Int32_val(Field(mta_caml, i));
	memcpy(Data_custom_val(mt_caml), &mt, sizeof(mt_t));

	CAMLreturn(mt_caml);
}

