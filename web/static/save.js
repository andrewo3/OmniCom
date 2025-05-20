window.addEventListener("beforeunload",(e) => {
    e.preventDefault();
    return "";
});

let isSaving = false;

const AUTOSAVE_INTERVAL = 1000;

setInterval(() => {
    if (!isSaving && romChoices > 0) {
        _saveCurrentRom();
        syncFileSystem(true);
    }
}, AUTOSAVE_INTERVAL);

function syncFileSystem(auto) {
    isSaving = true;
    FS.syncfs(false, function(err) {
        if (err) console.error("Sync to IDB failed", err);
        else {
        console.log("FS synced to IndexedDB");
        if (!auto) {
            alert("Game saved successfully!");
        }
        isSaving = false;
        }
    });
}