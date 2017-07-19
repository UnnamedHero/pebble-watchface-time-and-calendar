#simple automate 

test:	
	gcc src/c/test/test_timeutils.c -lbcunit -o watchtest
	./watchtest
	@rm watchtest

build:
	pebble build

emul:
	pebble install --emulator aplite

phone:
	pebble install --phone ${PEBBLE_PHONE}

loge:
	pebble logs

logp:
	pebble logs --phone ${PEBBLE_PHONE}

.PHONY: test