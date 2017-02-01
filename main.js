const {
    app,
    BrowserWindow,
    ipcMain
} = require('electron')

const electron = require('electron')

let win

function createWindow() {

    const screenSize = electron.screen.getPrimaryDisplay().size;

    const _width = 886, _height = 650
    win = new BrowserWindow({
        width: _width,
        height: _height,
        minWidth: _width,
        minHeight: _height,
        maxWidth: _width,
        maxHeight: _height,
        maximizable: false,
        resizable: false,
        icon: './assets/img/icon.png'
    })

    if( screenSize )
        win.setPosition( (screenSize.width  - _width )  / 2,
                         (screenSize.height - _height ) / 2 )
    else
        win.center()

    win.setMenu(null);

    win.loadURL(`file://${__dirname}/index.html`)

    win.on("closed", () => {
      win = null
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
