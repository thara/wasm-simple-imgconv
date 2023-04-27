clean:
	@rm -f imgconv.wasm
	@rm -f imgconv.js

imgconv.wasm:
	@emcc imgconv.c -o imgconv.js -s WASM=1 --js-library ./random.js

build: imgconv.wasm

serve:
	@node server.js
