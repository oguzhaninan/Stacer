'use strict';
// ==================================================================================
// index.js
// ----------------------------------------------------------------------------------
// Description:   System Information - library
//                for Node.js
// Copyright:     (c) 2014 - 2016
// Author:        Sebastian Hildebrandt
// ----------------------------------------------------------------------------------
// License:       MIT
// ==================================================================================
// 0. helper functions
// ----------------------------------------------------------------------------------

const os = require('os');
let _cores = 0;

function isFunction(functionToCheck) {
  let getType = {};
  return functionToCheck && getType.toString.call(functionToCheck) === '[object Function]';
}

function unique(obj){
  let uniques=[];
  let stringify={};
  for(let i=0;i<obj.length;i++){
    let keys=Object.keys(obj[i]);
    keys.sort(function(a,b) {return a-b});
    let str='';
    for(let j=0;j<keys.length;j++){
      str+= JSON.stringify(keys[j]);
      str+= JSON.stringify(obj[i][keys[j]]);
    }
    if(!stringify.hasOwnProperty(str)){
      uniques.push(obj[i]);
      stringify[str]=true;
    }
  }
  return uniques;
}

function sortByKey(array, keys) {
  return array.sort(function(a, b) {
    let x ='';
    let y ='';
    keys.forEach(function (key) {
      x = x + a[key]; y = y + b[key];
    });
    return ((x < y) ? -1 : ((x > y) ? 1 : 0));
  });
}

function cores() {
  if (_cores == 0) {
    _cores = os.cpus().length;
  }
  return _cores;
}

exports.isFunction = isFunction;
exports.unique = unique;
exports.sortByKey= sortByKey;
exports.cores = cores;
