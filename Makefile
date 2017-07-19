#simple automate 

build:
	pebble build

test:	
	gcc src/c/test/test_timeutils.c -lbcunit -o watchtest
	./watchtest
	@rm watchtest


emul: build
	pebble install --emulator aplite

phone: build
	pebble install --phone ${PEBBLE_PHONE}

loge:
	pebble logs

logp:
	pebble logs --phone ${PEBBLE_PHONE}

.PHONY: test build