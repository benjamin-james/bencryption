bencryption: src/demo.c src/bencryption_demo.c src/bencryption.c
	gcc -march=native src/demo.c src/bencryption_demo.c src/bencryption.c -O2 -o bencryption
