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
// 6. Battery
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

module.exports = function (callback) {

  return new Promise((resolve, reject) => {
    process.nextTick(() => {
      if (_windows) {
        let error = new Error(NOT_SUPPORTED);
        if (callback) { callback(NOT_SUPPORTED) }
        reject(error);
      }

      let result = {
        hasbattery: false,
        cyclecount: 0,
        ischarging: false,
        maxcapacity: 0,
        currentcapacity: 0,
        percent: 0
      };

      if (_linux) {
        let battery_path = '';
        if (fs.existsSync('/sys/class/power_supply/BAT1/status')) {
          battery_path = '/sys/class/power_supply/BAT1/'
        } else if (fs.existsSync('/sys/class/power_supply/BAT0/status')) {
          battery_path = '/sys/class/power_supply/BAT0/'
        }
        if (battery_path) {
          exec("cat " + battery_path + "status", function (error, stdout) {
            if (!error) {
              let lines = stdout.toString().split('\n');
              if (lines.length > 0 && lines[0]) result.ischarging = (lines[0].trim().toLowerCase() == 'charging')
            }
            exec("cat " + battery_path + "cyclec_ount", function (error, stdout) {
              if (!error) {
                let lines = stdout.toString().split('\n');
                if (lines.length > 0 && lines[0]) result.cyclecount = parseFloat(lines[0].trim());
              }
              exec("cat " + battery_path + "charge_full", function (error, stdout) {
                if (!error) {
                  let lines = stdout.toString().split('\n');
                  if (lines.length > 0 && lines[0]) result.maxcapacity = parseFloat(lines[0].trim());
                }
                exec("cat " + battery_path + "charge_now", function (error, stdout) {
                  if (!error) {
                    let lines = stdout.toString().split('\n');
                    if (lines.length > 0 && lines[0]) result.currentcapacity = parseFloat(lines[0].trim());
                  }
                  if (result.maxcapacity && result.currentcapacity) {
                    result.hasbattery = true;
                    result.percent = 100.0 * result.currentcapacity / result.maxcapacity;
                  }
                  if (callback) { callback(result) }
                  resolve(result);
                })
              })
            })
          })
        } else {
          if (callback) { callback(result) }
          resolve(result);
        }
      }
      if (_darwin) {
        exec("ioreg -n AppleSmartBattery -r | grep '\"CycleCount\"';ioreg -n AppleSmartBattery -r | grep '\"IsCharging\"';ioreg -n AppleSmartBattery -r | grep '\"MaxCapacity\"';ioreg -n AppleSmartBattery -r | grep '\"CurrentCapacity\"'", function (error, stdout) {
          if (!error) {
            let lines = stdout.toString().replace(/ +/g, "").replace(/"+/g, "").split('\n');
            lines.forEach(function (line) {
              if (line.indexOf('=') != -1) {
                if (line.toLowerCase().indexOf('cyclecount') != -1) result.cyclecount = parseFloat(line.split('=')[1].trim());
                if (line.toLowerCase().indexOf('ischarging') != -1) result.ischarging = (line.split('=')[1].trim().toLowerCase() == 'yes');
                if (line.toLowerCase().indexOf('maxcapacity') != -1) result.maxcapacity = parseFloat(line.split('=')[1].trim());
                if (line.toLowerCase().indexOf('currentcapacity') != -1) result.currentcapacity = parseFloat(line.split('=')[1].trim());
              }
            });
          }
          if (result.maxcapacity && result.currentcapacity) {
            result.hasbattery = true;
            result.percent = 100.0 * result.currentcapacity / result.maxcapacity;
          }
          if (callback) { callback(result) }
          resolve(result);
        });
      }
    });
  });
};
