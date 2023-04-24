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
        abort: (_msg, _file, line, column) => console.error(`Abort at ${line}:${column}`),
        seed: () => new Date().GetTime(),
      }
    });
  return wasm.instance.exports;
}

document.querySelector('.action.original').onclick = e => {
  e.preventDefault();
  drawOriginal();
}

document.querySelector('.action.grayscale').onclick = async e => {
  e.preventDefault();

  const wasm = await loadWASM();

  const imgData = getOriginalImageData();
  console.log(imgData.data);

  const imgBuf = imgData.data

  const bytes = new Uint8ClampedArray(wasm.memory.buffer);
  for (let i = 0; i < imgData.data.length; i++) {
    bytes[i] = imgData.data[i];
  }
  console.log(bytes);

  wasm['grayscale'](bytes, width, height);

  for (let i = 0; i < imgData.data.length; i++) {
     imgData.data[i] = bytes[i];
  }
  ctx.putImageData(imgData, 0, 0);
  console.log(imgData.data);
  console.log(bytes);

  // ctx.putImageData(imgData, 0, 0);
}
