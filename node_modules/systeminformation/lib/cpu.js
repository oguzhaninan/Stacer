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
// 4. CPU
// ----------------------------------------------------------------------------------

const os = require('os');
const exec = require('child_process').exec;
const fs = require('fs');
const util = require('./util');

let _platform = os.type();

const _linux = (_platform == 'Linux');
const _darwin = (_platform == 'Darwin');
const _windows = (_platform == 'Windows_NT');
const NOT_SUPPORTED = 'not supported';

let _cpu_speed = '0.00';

function cpuBrandManufacturer(res) {
  res.brand = res.brand.replace(/\(R\)+/g, "®");
  res.brand = res.brand.replace(/\(TM\)+/g, "™");
  res.brand = res.brand.replace(/\(C\)+/g, "©");
  res.brand = res.brand.replace(/CPU+/g, "").trim();
  res.manufacturer = res.brand.split(' ')[0];

  let parts = res.brand.split(' ');
  parts.shift();
  res.brand = parts.join(' ');
  return res;
}

// --------------------------
// CPU - brand, speed

function getCpu() {

  return new Promise((resolve, reject) => {
    process.nextTick(() => {
      if (_windows) {
        let error = new Error(NOT_SUPPORTED);
        if (callback) { callback(NOT_SUPPORTED) }
        reject(error);
      }

      let result = {
        manufacturer: 'unknown',
        brand: 'unknown',
        speed: '0.00',
        cores: util.cores()
      };
      if (_darwin) {
        exec("sysctl -n machdep.cpu.brand_string", function (error, stdout) {
          if (!error) {
            let lines = stdout.toString().split('\n');
            result.brand = lines[0].split('@')[0].trim();
            result.speed = lines[0].split('@')[1].trim();
            result.speed = parseFloat(result.speed.replace(/GHz+/g, ""));
            _cpu_speed = result.speed;
          }
          result = cpuBrandManufacturer(result);
          resolve(result);
        });
      }
      if (_linux) {
        exec("cat /proc/cpuinfo | grep 'model name'", function (error, stdout) {
          if (!error) {
            let lines = stdout.toString().split('\n');
            let line = lines[0].split(':')[1];
            result.brand = line.split('@')[0].trim();
            result.speed = line.split('@')[1] ? parseFloat(line.split('@')[1].trim()).toFixed(2) : '0.00';
            if (result.speed == '0.00') {
              let current = getCpuCurrentSpeedSync();
              if (current != '0.00') result.speed = current;
            }
            _cpu_speed = result.speed;
          }
          result = cpuBrandManufacturer(result);
          resolve(result);
        })
      }
    });
  });
}

// --------------------------
// CPU - Processor Data

function cpu(callback) {

  return new Promise((resolve, reject) => {
    process.nextTick(() => {
      if (_windows) {
        let error = new Error(NOT_SUPPORTED);
        if (callback) { callback(NOT_SUPPORTED) }
        reject(error);
      }

      getCpu().then(result => {
        if (callback) { callback(result) }
        resolve(result);
      })
    });
  });
}

exports.cpu = cpu;

// --------------------------
// CPU - current speed - in GHz

function getCpuCurrentSpeedSync() {

  let cpus = os.cpus();
  let minFreq = 999999999;
  let maxFreq = 0;
  let avgFreq = 0;

  if (cpus.length) {
    for (let i in cpus) {
      if (cpus.hasOwnProperty(i)) {
        avgFreq = avgFreq + cpus[i].speed;
        if (cpus[i].speed > maxFreq) maxFreq = cpus[i].speed;
        if (cpus[i].speed < minFreq) minFreq = cpus[i].speed;
      }
    }
    avgFreq = avgFreq / cpus.length;
    return {
      min: parseFloat((minFreq / 1000).toFixed(2)),
      max: parseFloat((maxFreq / 1000).toFixed(2)),
      avg: parseFloat((avgFreq / 1000).toFixed(2))
    }
  } else {
    return {
      min: 0,
      max: 0,
      avg: 0
    }
  }
}

function cpuCurrentspeed(callback) {

  return new Promise((resolve) => {
    process.nextTick(() => {
      let result = getCpuCurrentSpeedSync();
      if (result == 0 && _cpu_speed != '0.00') result = parseFloat(_cpu_speed);

      if (callback) { callback(result) }
      resolve(result);
    });
  });
}

exports.cpuCurrentspeed = cpuCurrentspeed;

// --------------------------
// CPU - temperature
// if sensors are installed

function cpuTemperature(callback) {

  return new Promise((resolve, reject) => {
    process.nextTick(() => {
      if (_windows) {
        let error = new Error(NOT_SUPPORTED);
        if (callback) { callback(NOT_SUPPORTED) }
        reject(error);
      }

      let result = {
        main: -1.0,
        cores: [],
        max: -1.0
      };
      if (_linux) {
        exec("sensors", function (error, stdout) {
          if (!error) {
            let lines = stdout.toString().split('\n');
            lines.forEach(function (line) {
              let regex = /\+([^°]*)/g;
              let temps = line.match(regex);
              if (line.split(':')[0].toUpperCase().indexOf('PHYSICAL') != -1) {
                result.main = parseFloat(temps);
              }
              if (line.split(':')[0].toUpperCase().indexOf('CORE ') != -1) {
                result.cores.push(parseFloat(temps));
              }
            });
            if (result.cores.length > 0) {
              let maxtmp = Math.max.apply(Math, result.cores);
              result.max = (maxtmp > result.main) ? maxtmp : result.main;
            }
            if (callback) { callback(result) }
            resolve(result);
          } else {
            exec("/opt/vc/bin/vcgencmd measure_temp", function (error, stdout) {
              if (!error) {
                let lines = stdout.toString().split('\n');
                if (lines.length > 0 && lines[0].indexOf('=')) {
                  result.main = parseFloat(lines[0].split("=")[1]);
                  result.max = result.main
                }
              }
              if (callback) { callback(result) }
              resolve(result);
            });

          }
        });
      }
      if (_darwin) {
        if (callback) { callback(result) }
        resolve(result);
      }
    });
  });
}

exports.cpuTemperature = cpuTemperature;
