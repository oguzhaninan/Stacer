/* eslint-disable func-names */
'use strict';

var proxyquire = require('proxyquire');
var path = require('path');
require('should');

var files = [
  '/root',
  '/root/folder',
  '/root/folder/subfolder',
  '/root/folder/subfolder/file4',
  '/root/folder/file3',
  '/root/file1',
  '/root/file2'
];

var sizes = {
  '/root/folder/subfolder/file4': 4,
  '/root/folder/file3': 3,
  '/root/file1': 1,
  '/root/file2': 2
};

Object.keys(sizes).forEach(function(file) {
  var file2 = file.replace(/\//g, path.sep);
  sizes[file2] = sizes[file];
});

files.map(function(file) {
  return file.replace(/\//g, path.sep);
});

var fs = {
  lstat: function(item, cb) {
    var stats = {
      size: sizes[item],
      isDirectory: function() {
        return ((item === files[0]) || /folder$/.test(item));
      }
    };

    setImmediate(function() {
      cb(null, stats);
    });
  },
  readdir: function(item, cb) {
    setImmediate(function() {
      var list = files.filter(function(file) {
        return ((file !== item) && (file.indexOf(item) !== -1));
      }).map(function(file) {
        return file.replace(item, '');
      }).filter(function(it) {
        return (it.lastIndexOf(path.sep) <= 0);
      });

      cb(null, list);
    });
  }
};

describe('getSize', function() {
  var getSize;

  before(function() {
    getSize = proxyquire.load('./index', {
      fs: fs
    });
  });

  it('should get the size of the folder', function(done) {
    getSize(files[0], function(err, total) {
      total.should.eql(10);

      done();
    });
  });

  it('should ignore files', function(done) {
    getSize(files[0], /(file1|file2)/, function(err, total) {
      total.should.eql(7);

      done();
    });
  });
});
