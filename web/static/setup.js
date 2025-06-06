if ( window.history.replaceState ) {
    window.history.replaceState( null, null, window.location.href );
  }

let reader = new FileReader();
let romChoices = 0;
let romNameForm = document.getElementById("romnameform");
let romNameText = document.getElementById("newrom");
let canvas = document.getElementById('canvas');
let gameselector = document.getElementsByClassName("gameselector")[0];
let choices = document.getElementsByClassName("choices")[0];
let game_choices = Array.from(document.getElementsByClassName("game_choice"));

function hexToRgba(hex,alpha) {
    var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
    return result ? "rgba("+parseInt(result[1], 16).toString()+","+parseInt(result[2], 16).toString()+","
        +parseInt(result[3], 16).toString()+","+alpha.toString()+")" : null;
}

let bg_color = "#30323d";
let fg_color = "#e8c547";
const anim_timeout = 20;

let bg_translucent = hexToRgba(bg_color,1.0);
let fg_translucent = hexToRgba(fg_color,0.3);
let fg_placeholder = hexToRgba(fg_color,0.5);

canvas_opacity = 0;
document.body.style.backgroundColor = bg_color;
gameselector.style.backgroundColor = bg_color;
gameselector.style.color = fg_color;
gameselector.style.borderColor = fg_color;
choices.style.color = fg_color;
choices.style.backgroundColor = bg_translucent;
choices.style.borderColor = fg_color;
const style = document.createElement("style");
style.innerHTML = ".gameselector::placeholder { color: "+fg_placeholder+" !important; } \
.choices hr { color: "+fg_color+";}\
 .choices p:hover {background-color:"+fg_translucent+";}"
document.head.appendChild(style);

function getRandomInt(max) {
    return Math.floor(Math.random() * max);
  }

let text_iter = 0;
let placeholder_text = "<rom title>";
let text_choices = ["<rom title>","super mario bros","tetris","legend of zelda","pac-man"];
let change_text = 0;
let change_choice = 0;
window.setInterval(function() {
    out_text = placeholder_text;
    if (Math.floor((text_iter/anim_timeout)/2)%2==0 && change_text == 0) {
        out_text+="_";
    }
    if (Math.floor(text_iter/anim_timeout)==10 && change_text==0) {
        change_text = 1;
        if (change_choice == 0) {
            change_choice = getRandomInt(text_choices.length-1)+1;
        } else {
            change_choice = 0;
        }
    }
    else if (change_text == 1 && text_iter%2==0 && placeholder_text.length>0) {
        placeholder_text = placeholder_text.slice(0,-1);
        if (placeholder_text.length==0) {
            change_text = 2;
        }
    } else if (change_text == 2 && text_iter%2==0 && placeholder_text!=text_choices[change_choice]) {
        placeholder_text = text_choices[change_choice].slice(0,placeholder_text.length+1);
        if (placeholder_text==text_choices[change_choice]) {
            change_text = 0;
            text_iter = 0;
        }
    }
    gameselector.placeholder = out_text;
    text_iter+=1;
},anim_timeout);

let choices_style = document.createElement("style");
choices_style.innerHTML = ".choices { top: 3px; border-style: none; height: 0px;}"
document.head.appendChild(choices_style);

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
        function interp(secs) {
            return 1-(1-secs)**5;
        }
        let move_iter = 0;
        let timer = setInterval(function() {
            let i = (move_iter/anim_timeout)/1.5;
            if (i>1) {
                canvas.style.marginBottom = "3vh";
                clearInterval(timer);
            } else {
                canvas.style.marginBottom = (-20+23*interp(i)).toString()+"vh";
            }
            move_iter+=1;
        },anim_timeout);
        callMain(["/home/web_user/".concat(name)]);
    } else {
        console.log("new rom");
        let utf8str = stringToNewUTF8("/home/web_user/".concat(name));
        _changeRom(utf8str);
    }
    let timer = setInterval(function() {
        if (canvas_opacity>=1) {
            canvas.style.opacity = "1";
            clearInterval(timer);
        } else {
            canvas_opacity+=0.1;
            canvas.style.opacity = canvas_opacity.toString();
        }
    },anim_timeout);
    
    romChoices+=1;
}

function module_init() {
    //load past saved data from IDB
    FS.mkdir('/persistent');
    FS.mount(IDBFS, {}, '/persistent');

    FS.syncfs(true, function(err) {
        if (err) console.error("Sync from IDB failed", err);
        else console.log("FS synced from IndexedDB");
    });

    Module.canAcceptKeyboardInput = function() {
        return canvasClicked;
    };
    
    //--drag rom into window--
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

function get_results() {
    let formData = {"text":$("#newrom").val()};
    if (formData.text!="") {
        let req = new XMLHttpRequest();
        req.open("POST", "/matches", true);
        req.responseType = "json";
        req.onload = function(e) {
            roms = req.response;
            console.log(roms);
            while (choices.firstChild) {
                choices.removeChild(choices.firstChild);
            }
            game_choices = [];
            for (const rom of roms) {
                let newP = document.createElement("p");
                newP.tabIndex=0;
                let line = document.createElement("hr");
                newP.innerHTML = rom.substring(0,rom.length-4);
                game_choices.push(newP);
                choices.appendChild(newP);
                choices.appendChild(line);
            }
            //setup click events
            console.log("click events");
            game_choices.forEach(element => {
                element.addEventListener("click",() => {
                    let formData = {"text":element.innerHTML+".nes"};
                    console.log(formData);
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
                        num_sel(0);
            
                    };
                    req.setRequestHeader("Content-Type", "application/json");
                    req.send(JSON.stringify(formData));
                })
            });
            if (text_sel) {
                num_sel(roms.length);
            }
        }
        req.setRequestHeader("Content-Type", "application/json");
        req.send(JSON.stringify(formData));
        search_timeout = null;
    }
}

search_timeout = null;

$(document).ready(function () {
    $("#newrom").on('input propertychange paste',function() {
        change_choice = 0;
        change_text = 2;
        placeholder_text = "";
        num_sel(0);
        if (search_timeout) {
            window.clearTimeout(search_timeout);
        }
        search_timeout = window.setTimeout(get_results,1000);
    });
    $("#romnameform").submit(function (e) {
        e.preventDefault();
        get_results();
    });
    /*$("#romnameform").submit(function (e) {
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
            num_sel(0);
            $("#newrom").blur();
            //window.clearTimeout(search_timeout);

        };
        req.setRequestHeader("Content-Type", "application/json");
        req.send(JSON.stringify(formData));
        
    });*/
});

choice_sel = false;
text_sel = false;

choices.addEventListener("focus",function(){
    choice_sel = true;
    num_sel(choices.children.length/2);
});
choices.addEventListener("blur",function(event){
    choice_sel = false;
    if (!choice_sel && !text_sel && !choices.contains(event.explicitOriginalTarget)) {
        num_sel(0);
    }
});

gameselector.addEventListener("focus",function(){
    text_sel = true;
    console.log(choices.children.length/2);
    num_sel(choices.children.length/2);
});

gameselector.addEventListener("blur",function(event){
    console.log(event);
    text_sel = false;
    console.log(text_sel);
    console.log(choice_sel);
    console.log(event.relatedTarget);
    console.log(choices.contains(event.relatedTarget));
    if (!choice_sel && !text_sel && !choices.contains(event.relatedTarget)) {
        num_sel(0);
    }
});


//choice box animations above the game name textbox
animations = [];

function animate_borderless(on) {
    function stylize(r) {
        new_style = document.createElement("style");
        new_style.id = "borderless";
        new_style.innerHTML = ".gameselector {border-top-left-radius: "+r.toString()+"px !important; border-top-right-radius: "+r.toString()+"px !important;}";
        if (document.getElementById("borderless")==null) {
            document.head.appendChild(new_style);
        } else {
            new_style = document.getElementById("borderless");
        }
        return new_style;
    }

    //clear all previous animations
    for (let i = 0; i<animations.length; i++) {
        clearInterval(animations[i]);
    }
    animations = [];
    function interp(secs) {
        return 1-(1-secs)**5;
    }
    if (on) {
        
        let radius = 15;
        let new_style = stylize(radius);
        let iters = 0;
        let timer = setInterval(function() {
            if (radius<=0) {
                radius = 0;
                new_style.innerHTML = ".gameselector {border-top-left-radius: 0px !important; border-top-right-radius: 0px !important;}";
                clearInterval(timer);
            } else {
                radius = 15*(1-interp(iters*anim_timeout/1000));
                new_style.innerHTML = ".gameselector {border-top-left-radius: "+radius.toString()+"px !important; border-top-right-radius: "+radius.toString()+"px !important;}";
            }
            iters+=1;
        },anim_timeout);
        animations.push(timer);
    } else {
        let radius = 0;
        let new_style = stylize(radius);
        let iters = 0;
        let timer = setInterval(function() {
            if (radius>=15) {
                document.getElementById("borderless").remove();
                clearInterval(timer);
            } else {
                radius = 15*interp(iters*anim_timeout/1000);
                new_style.innerHTML = ".gameselector {border-top-left-radius: "+radius.toString()+"px !important; border-top-right-radius: "+radius.toString()+"px !important;}";
            }
            iters+=1;
        },anim_timeout);
        animations.push(timer);
    }
}

let choices_y = 3;
let choices_h = 0;
let box_anims = [];
function box_appear() {
    choices_y = 3;
    for (let i = 0; i<box_anims.length; i++) {
        clearInterval(box_anims[i]);
    }
    box_anims = [];
    let = iters = 0;
    choices = document.getElementsByClassName("choices")[0];
    let h = 0;
    choices_style.innerHTML = ".choices { top: "+choices_y.toString()+"px; height: 0px}"
    function interp(secs) {
        return 1-(1-secs)**5;
    }
    let timer = setInterval(function() {
        let elapsed = iters*anim_timeout;
        if (elapsed<500) {
            //first half
            choices_y = 3-13*interp(elapsed/500);
            choices_style.innerHTML = ".choices { top: "+choices_y.toString()+"px; height: 0px;}"
        } else {
            choices_style.innerHTML = ".choices { top: 10px; height: 0px;}"
            clearInterval(timer);
        }
        iters+=1;
    },anim_timeout);
    box_anims.push(timer);
}

function box_disappear() {
    choices_y = -10;
    for (let i = 0; i<box_anims.length; i++) {
        clearInterval(box_anims[i]);
    }
    box_anims = [];
    let = iters = 0;
    choices = document.getElementsByClassName("choices")[0];
    let h = 0;
    choices_style.innerHTML = ".choices { top: "+choices_y.toString()+"px; height: 0px}"
    function interp(secs) {
        return 1-(1-secs)**5;
    }
    let timer = setInterval(function() {
        let elapsed = iters*anim_timeout;
        if (elapsed<500) {
            //first half
            choices_y = -10+13*interp(elapsed/500);
            choices_style.innerHTML = ".choices { top: "+choices_y.toString()+"px; height: 0px;}"
        } else {
            choices_style.innerHTML = ".choices { top: 0px; border-style: none; height: 0px;}"
            clearInterval(timer);
        }
        iters+=1;
    },anim_timeout);
    box_anims.push(timer);
}

const element_h = 5.3;
function box_expand(choices,start) {
    const target_h = choices*(element_h);
    const start_h = start;
    const diff = target_h-start_h;

    for (let i = 0; i<box_anims.length; i++) {
        clearInterval(box_anims[i]);
    }
    box_anims = [];
    iters = 0;
    function interp(secs) {
        return 1-(1-secs)**5;
    }
    let timer = setInterval(function() {
        let elapsed = iters*anim_timeout;
        if (elapsed<500) {
            let new_h = start_h+diff*interp(elapsed/500);
            choices_style.innerHTML = ".choices { border-width: medium;top: "+choices_y.toString()+"px; height: "+new_h.toString()+"vh;}"
        } else {
            choices_style.innerHTML = ".choices { border-width: medium; top: "+choices_y.toString()+"px; height: "+target_h+"vh;}"
            clearInterval(timer);
        }
        iters+=1;
    },anim_timeout);
    box_anims.push(timer);
    return target_h;
}


let new_box = false;
async function num_sel(num_choices) {
    //let num_choices = document.getElementById("num_choice").value;
    //console.log(choices_size);
    if (num_choices>10) {
        num_choices = 10;
    }
    if (num_choices>0 && !new_box) {
        new_box = true
        //animate_borderless(true);
        box_appear();
        choices_h = num_choices*(element_h);
        await new Promise(r => setTimeout(r,500));
        if (choices_h==num_choices*(element_h)) {
            box_expand(num_choices,0);
        }
        
    } else if (num_choices==0 && new_box) {
        new_box = false;
        //animate_borderless(false);
        choices_h = box_expand(num_choices,choices_h);
        await new Promise(r => setTimeout(r,500));
        choices_style.innerHTML = ".choices { border-width: 1.5px;}"
        box_disappear();
    } else if (num_choices > 0) {
        choices_h = box_expand(num_choices,choices_h);
    }
    
}
