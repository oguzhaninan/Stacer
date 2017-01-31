# get-folder-size

Get the size of a folder by iterating through its sub-files and folders.

## Usage

```js
getFolderSize(folder, [regexIgnorePattern], callback)
```

Example:

```js
var getSize = require('get-folder-size');

getSize(myFolder, function(err, size) {
  if (err) { throw err; }

  console.log(size + ' bytes');
  console.log((size / 1024 / 1024).toFixed(2) + ' Mb');
});
```

## CLI tool

```bash
npm i -g get-folder-size
get-folder-size --folder=/my/folder --ignore=node_modules
```

## Size vs Size on disk

[This module calculates the actual file size, and not the size on disk.](http://stackoverflow.com/questions/15470787/please-help-me-understand-size-vs-size-on-disk)

## License

MIT
