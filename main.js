const {
    app,
    BrowserWindow,
} = require('electron')

const electron = require('electron')

var win

function createWindow() {        
    const _width = 935, _height = 620
    win = new BrowserWindow({
        width: _width,
        height: _height,
        minWidth: _width,
        minHeight: _height,
        maxWidth: _width,
        maxHeight: _height,
        maximizable: false,
        resizable: false,
        icon: './assets/img/icons/icon256x256.png'
    })

    try {
        const screenSize = electron.screen.getPrimaryDisplay().size;
        const w = (screenSize.width  - _width )  / 2 ,
              h = (screenSize.height - _height ) / 2
        win.setPosition(w,h)
    }
    catch(err) {
        win.center()
    }

    win.setMenu(null)

    win.loadURL(`file://${__dirname}/bundle/index.html`)
    
    win.on('closed', () => win = null )
}

app.on('ready', createWindow)

app.on('window-all-closed', () => app.quit() )

app.on('activate', () => {
    if (win === null)
        createWindow()
})
