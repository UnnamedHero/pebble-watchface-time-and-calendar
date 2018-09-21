#simple automate 

#specify PEBBLE_PHONE in .env file
include .env

babel:
	npm run build

build: babel
	pebble build -- --enable-debug

build-release: babel
	pebble build 

test:	
	@gcc -c src/c/utils/linkedlist.c -DTESTS
	# @gcc -c src/c/utils/ticktimerhelper.c -DTESTS
	@gcc -c src/c/utils/timelib.c
	@gcc -c src/c/test/tests.c -DTESTS
	@gcc tests.o linkedlist.o timelib.o -lcunit -o lltest
	@./lltest
	@rm *.o
	@rm lltest

emul: build	
	PEBBLE_PHONE="" pebble install --emulator aplite

emul_dio: build
	PEBBLE_PHONE="" pebble install --emulator diorite

phone: build
	pebble install --phone ${PEBBLE_PHONE}

loge:
	PEBBLE_PHONE="" pebble logs --emulator aplite 

loge_dio: wipe
	PEBBLE_PHONE="" pebble logs --emulator diorite

logp:
	pebble logs --phone ${PEBBLE_PHONE}

config:
	PEBBLE_PHONE="" pebble emu-app-config
	
tap:
	pebble emu-tap

wipe:
	pebble wipe

shot:
	PEBBLE_PHONE="" pebble screenshot --emulator aplite	

shot_dio:
	PEBBLE_PHONE="" pebble screenshot --emulator diorite	
	
.PHONY: test build