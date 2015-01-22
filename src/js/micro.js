function b64ToUint6 (nChr) {

    return nChr > 64 && nChr < 91 ?
        nChr - 65
        : nChr > 96 && nChr < 123 ?
        nChr - 71
        : nChr > 47 && nChr < 58 ?
        nChr + 4
        : nChr === 43 ?
        62
        : nChr === 47 ?
        63
        :
        0;
}

function base64DecToArr (sBase64, nBlocksSize) {
    var
    sB64Enc = sBase64.replace(/[^A-Za-z0-9\+\/]/g, ""), nInLen = sB64Enc.length,
    nOutLen = nBlocksSize ? Math.ceil((nInLen * 3 + 1 >> 2) / nBlocksSize) * nBlocksSize : nInLen * 3 + 1 >> 2, taBytes = new Uint8Array(nOutLen);

    for (var nMod3, nMod4, nUint24 = 0, nOutIdx = 0, nInIdx = 0; nInIdx < nInLen; nInIdx++) {
        nMod4 = nInIdx & 3;
        nUint24 |= b64ToUint6(sB64Enc.charCodeAt(nInIdx)) << 18 - 6 * nMod4;
        if (nMod4 === 3 || nInLen - nInIdx === 1) {
            for (nMod3 = 0; nMod3 < 3 && nOutIdx < nOutLen; nMod3++, nOutIdx++) {
                taBytes[nOutIdx] = nUint24 >>> (16 >>> nMod3 & 24) & 255;
            }
            nUint24 = 0;

        }
    }
    return taBytes;
}

window.lastNote = 0;
window.lastNoteLength = 0;
window.time = 0;

var sound_visualizer = document.getElementById('sound_visualizer').getContext('2d');
sound_visualizer.lineWidth = 1;
sound_visualizer.strokeStyle = 'black'
sound_visualizer.moveTo(0, 128);

function visualize(arr) {
  sound_visualizer.beginPath()
  sound_visualizer.clearRect(0, 0, 256, 256)
  for (var i = 0, len = arr.length; i < len; ++i) {
    sound_visualizer.lineTo(i, 128 - arr[i] * 100);
  }
  sound_visualizer.stroke();
}

var noisearray = new Float32Array(4096);
var noiseiterations = 1;

var playerStatus = 0;
var RECORDING_NOISE = 1;
var GETTING_NOTES = 2;

var i = 0;
var dataarray = new Float32Array(4096);
var callback = Module.cwrap('processMicroSignal', 'number', ['number']);
var nDataBytes = 4096 * dataarray.BYTES_PER_ELEMENT;
var dataPtr = Module._malloc(nDataBytes);
var dataHeap = new Uint8Array(Module.HEAPU8.buffer, dataPtr, nDataBytes);

function jellymicCallback(data) {
    var arr = base64DecToArr(data, 4);
    var arraybuff = arr.buffer;
    var dataview = new DataView(arraybuff);

    var note = 0;

    for (var j = 0, len = arr.length / 4; j < len; ++j, ++i) {
      dataarray[i] = dataview.getFloat32(j * 4);
      noisearray[i] += dataarray[i];

      if (i == 4095) {
        if (playerStatus == GETTING_NOTES) {
          dataarray[i] -= noisearray[i] / noiseiterations;
          dataHeap.set(new Uint8Array(dataarray.buffer));
          console.log("Before callback");
          note = callback(dataHeap.byteOffset);
          console.log("After callback, note = ", note, "window.lastNote = ", window.lastNote);

          if (window.lastNote !== note) { // && !isNan(note)) {
            console.log("In if");
            if (window.lastNote > 0) {
              console.log("Adding note", window.lastNote, window.lastNoteLength);
              window.score.addNote(window.time - window.lastNoteLength, window.lastNote, window.lastNoteLength)
            }
            window.lastNote = note;
            window.lastNoteLength = 0;
          }
          ++window.lastNoteLength;
          ++window.time;
        }
        else if (playerStatus == RECORDING_NOISE)
          ++noiseiterations;
        visualize(dataarray);
        i = -1;
      }
    }
}

function callbackname(name) {
    console.log("mic name:", name);
}

function callbackallow(event) {
    console.log("mic allowed", event);
}

function callbackclose() {
    console.log("mic closed");
}

var swfid="jellymic";

var jellymic = document.getElementById(swfid);
if (jellymic) {
    var interval = window.setInterval(function(){
        if((typeof jellymic.startMicrophone) !== "undefined"){
            console.log("start mic !!!!");
            jellymic.startMicrophone("jellymicCallback", "callbackname", "callbackallow", "callbackclose", 5);
            clearInterval(interval);
        }
    }, 100);
}

console.log("In mic.js")
