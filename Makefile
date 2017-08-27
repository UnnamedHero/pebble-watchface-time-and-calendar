#simple automate 

build:
	pebble build

test:	
	@gcc -c src/c/utils/linkedlist.c -DTESTS
	@gcc -c src/c/utils/ticktimerhelper.c -DTESTS
	@gcc -c src/c/test/tests.c -DTESTS
	@gcc tests.o linkedlist.o ticktimerhelper.o -lbcunit -o lltest
	@./lltest
	@rm *.o
	@rm lltest

emul: build
	pebble install --emulator aplite

phone: build
	pebble install --phone ${PEBBLE_PHONE}

loge:
	pebble logs --emulator aplite

logp:
	pebble logs --phone ${PEBBLE_PHONE}

config:
	pebble emu-app-config
	
tap:
	pebble emu-tap
	
.PHONY: test build