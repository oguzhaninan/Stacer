const {
    app,
    BrowserWindow
} = require("electron")

const {
    ipcMain
} = require('electron')

let win

function createWindow() {

    win = new BrowserWindow({
        width: 886,
        height: 700,
        frame: false,
        minWidth: 920,
        minHeight: 700,
        maxWidth: 920,
        maxHeight: 700,
        maximizable: false,
        resizable: false,
        center: true,
        icon: './assets/img/icon.png'
    })

    win.loadURL(`file://${__dirname}/index.html`)

    win.on("closed", () => {
      win = null
    })

    ipcMain.on("close-app", () => {
      app.quit()
    })

    ipcMain.on("minimize-app", () => {
      BrowserWindow.getFocusedWindow().minimize()
    })

}

app.on("ready", createWindow)

app.on("window-all-closed", () => {
    app.quit()
})

app.on("activate", () => {

    if (win === null)
        createWindow()

})
