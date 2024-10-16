let reader = new FileReader();
let romChoices = 0;
let romNameForm = document.getElementById("romnameform");
let romNameText = document.getElementById("newrom");
let canvas = document.getElementById('canvas');
let gameselector = document.getElementsByClassName("gameselector")[0];
let choices = document.getElementsByClassName("choices")[0];
let bg_color = "#30323d";
let fg_color = "#e8c547";

document.body.style.backgroundColor = bg_color;
gameselector.style.backgroundColor = bg_color;
gameselector.style.color = fg_color;
gameselector.style.borderColor = fg_color;
choices.style.color = fg_color;
choices.style.borderColor = fg_color;
const style = document.createElement("style");
style.innerHTML = ".gameselector::placeholder { color: "+fg_color+" !important;}"
document.head.appendChild(style);
console.log(style.innerHTML);

var canvasClicked = false;

canvas.addEventListener('click', handleCanvasClick);
function handleCanvasClick() {
    canvasClicked = true;
    canvas.focus();
}
function handleBodyClick(event) {
    if (event.target !== canvas) {
        canvasClicked = false; // Reset canvasClicked if the click occurred outside the canvas
        canvas.blur();
    }
}

window.onerror = function() {
    location.reload();
}

// Add event listener for click events on the document body
document.body.addEventListener('click', handleBodyClick);


Module={
    canvas:document.getElementById("canvas"),
    onRuntimeInitialized:module_init,
    noExitRuntime:true,
    doNotCaptureKeyboard:true,
    preRun:function () {
        ENV.SDL_EMSCRIPTEN_KEYBOARD_ELEMENT = "#canvas";
    },
    onAbort:function() {
        location.reload();
    }
};

window.addEventListener ("resize", function (e) {

Module.canvas.width = canvas.clientWidth;
Module.canvas.height = canvas.clientHeight;

}, true);


function loadNewRom(data,name) {
    FS.writeFile('/home/web_user/'.concat(name),data);
    if (romChoices == 0) {
        console.log("started");
        callMain(["/home/web_user/".concat(name)]);
    } else {
        console.log("new rom");
        let utf8str = stringToNewUTF8("/home/web_user/".concat(name));
        _changeRom(utf8str);
    }
    romChoices+=1;
}

//--drag rom into window--
function module_init() {
    Module.canAcceptKeyboardInput = function() {
        return canvasClicked;
    };

    let dropbox = document.getElementById("dropbox");
    function drag(e) {
        e.stopPropagation();
        e.preventDefault();
    }
    function drop(e) {
        e.stopPropagation();
        e.preventDefault();

        const dt = e.dataTransfer;
        let rom = dt.files[0];
        let ext = rom.name.split('.').pop();
        console.log(rom);
        reader.addEventListener('loadend',(e)=>{
            let result = reader.result;
            const data = new Uint8Array(result);
            loadNewRom(data,rom.name);
        });
        reader.readAsArrayBuffer(rom);
        console.log(FS);
    }
    dropbox.addEventListener("dragenter",drag,false);
    dropbox.addEventListener("dragover",drag,false);
    dropbox.addEventListener("drop",drop,false);
}


//--type game name--

$(document).ready(function () {
    $("#romnameform").submit(function (e) {
        e.preventDefault();
        let formData = {"text":$("#newrom").val()};

        let req = new XMLHttpRequest();
        req.open("POST", "/process_name", true);
        req.responseType = "arraybuffer";

        req.onload = function(e) {
            let arrayBuffer = req.response;
            // if you want to access the bytes:
            let data = new Uint8Array(arrayBuffer);
            const spl = data.indexOf(10); //10 is a newline character
            filename = new TextDecoder().decode(data.subarray(0,spl));
            console.log(filename);
            rom = data.subarray(spl+1);
            loadNewRom(rom,filename);

        };
        req.setRequestHeader("Content-Type", "application/json");
        req.send(JSON.stringify(formData));
        
    });
});
