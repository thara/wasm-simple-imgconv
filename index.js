const canvas = document.querySelector('canvas');
const ctx = canvas.getContext('2d');
const [width, height] = [canvas.width, canvas.height];

const img = new Image();
img.src = './waterlily.png';
img.crossOrigin = 'anonymous';
img.onload = () => drawOriginal();

function drawOriginal() {
  ctx.drawImage(img, 0, 0, width, height);
}

function getOriginalImageData() {
  drawOriginal();
  return ctx.getImageData(0, 0, width, height);
}

async function loadWASM() {
  const arraySize = (width * height * 4) >>> 0;
  const pageCount = ((arraySize + 0xffff) & ~0xffff) >> 16;
  const memory = new WebAssembly.Memory({initial: pageCount});

  const wasm = await WebAssembly
    .instantiateStreaming(fetch('imgconv.wasm'), {
      env: {
        memory,
        buffer: memory.buffer,
        js_random: () => Math.random(),
      }
    });
  return wasm;
}

async function invoke(f) {
  const wasm = await loadWASM();

  const imageData = getOriginalImageData();

  const offset = wasm.instance.exports.getImageBuf();
  const bufSize = wasm.instance.exports.getImageBufSize();
  const bytes = new Uint8ClampedArray(wasm.instance.exports.memory.buffer, offset, bufSize);

  for (let i = 0; i < imageData.data.length; i++) {
    bytes[i] = imageData.data[i];
  }

  f(wasm);

  for (let i = 0; i < imageData.data.length; i++) {
     imageData.data[i] = bytes[i];
  }
  ctx.putImageData(imageData, 0, 0);
}

document.querySelector('.action.original').onclick = e => {
  e.preventDefault();
  drawOriginal();
}

document.querySelector('.action.invert').onclick = async e => {
  e.preventDefault();
  invoke((wasm) => wasm.instance.exports.invert());
}

document.querySelector('.action.grayscale').onclick = async e => {
  e.preventDefault();
  invoke((wasm) => wasm.instance.exports.grayscale());
}

document.querySelector('.action.basicMonochrome').onclick = async e => {
  e.preventDefault();
  invoke((wasm) => wasm.instance.exports.basic_monochrome(150));
}

document.querySelector('.action.randomMonochrome').onclick = async e => {
  e.preventDefault();
  invoke((wasm) => wasm.instance.exports.random_monochrome(80));
}
