let reader = new FileReader();
let romChoices = 0;
Module={
    canvas:document.getElementById("canvas"),
    onRuntimeInitialized:module_init,
    noExitRuntime:true
};
window.addEventListener ("resize", function (e) {

Module.canvas.width = container.clientWidth;
Module.canvas.height = container.clientHeight;

}, true);
function module_init() {
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
            FS.writeFile('/home/web_user/'.concat(rom.name),data);
            if (romChoices == 0) {
                console.log("started");
                callMain(["/home/web_user/".concat(rom.name)]);
            } else {
                console.log("new rom");
                let utf8str = allocateUTF8("/home/web_user/".concat(rom.name));
                _changeRom(utf8str);
            }
            romChoices+=1;
        });
        reader.readAsArrayBuffer(rom);
        console.log(FS);
    }
    dropbox.addEventListener("dragenter",drag,false);
    dropbox.addEventListener("dragover",drag,false);
    dropbox.addEventListener("drop",drop,false);
}
