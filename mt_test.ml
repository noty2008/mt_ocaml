open Mersenne_twister
open Printf
let _ =
	let mt = init_by_array [| 0x123l; 0x234l; 0x345l; 0x456l |] in

	printf "1000 outputs of genrand_int32()\n";
	for i = 0 to 999 do
		printf "%10lu " (genrand_int32 mt);
		if i mod 5 = 4 then printf "\n"
	done;
printf "\n1000 outputs of genrand_real2()\n";
	for i = 0 to 999 do
		printf "%10.8f " (genrand_real2 mt);
		if i mod 5 = 4 then printf "\n"
	done;
exit 0
