all: logger acmonitor test_aclog

logger: logger.c
	gcc -Wall -fPIC -shared -o logger.so logger.c -lcrypto -ldl 

acmonitor: acmonitor.c 
	gcc acmonitor.c -o acmonitor

test_aclog: test_aclog.c 
	gcc test_aclog.c -o test_aclog -ldl

run: logger.so test_aclog
	LD_PRELOAD=./logger.so ./test_aclog
	cat file_logging.log
clean:
	rm -rf logger.so
	rm -rf test_aclog
	rm -rf acmonitor
clean-files:
	rm -f file_0 file_1 file_2 file_3 file_4
	rm -f file_5 file_6 file_7 file_8 file_9
	rm -f file_logging.log

