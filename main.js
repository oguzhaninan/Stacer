const {
    app,
    BrowserWindow,
    ipcMain
} = require("electron")
const Config = require('electron-config')

let win
let config = new Config()

function createWindow() {

    win = new BrowserWindow({
        x: config.get('position.x') || undefined,
        y: config.get('position.y') || undefined,
        width: 886,
        height: 700,
        frame: false,
        minWidth: 920,
        minHeight: 700,
        maxWidth: 920,
        maxHeight: 700,
        maximizable: false,
        resizable: false,
        icon: './assets/img/icon.png'
    })

    win.loadURL(`file://${__dirname}/index.html`)

    win.on("closed", () => {
      win = null
    })

    win.on("move", () => {
        if (win !== null) {
            saveWindowPosition(win)
        }
    })

    ipcMain.on("close-app", () => {
      app.quit()
    })

    ipcMain.on("minimize-app", () => {
      BrowserWindow.getFocusedWindow().minimize()
    })
}

function saveWindowPosition (window) {
    let position = window.getPosition()

    config.set('position.x', position[0])
    config.set('position.y', position[1])
}

app.on("ready", createWindow)

app.on("window-all-closed", () => {
    app.quit()
})

app.on("activate", () => {

    if (win === null)
        createWindow()
})
