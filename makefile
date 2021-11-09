main: simple_crypto.h simple_crypto.c
	@gcc -o main main.c

main2: simple_crypto.h simple_crypto.c main
	@rm main
	@gcc -o main main.c

clean: main
	@rm main
	@echo 'old file cleared'
