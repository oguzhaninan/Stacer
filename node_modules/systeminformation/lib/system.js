'use strict';
// ==================================================================================
// index.js
// ----------------------------------------------------------------------------------
// Description:   System Information - library
//                for Node.js
// Copyright:     (c) 2014 - 2016
// Author:        Sebastian Hildebrandt
// ----------------------------------------------------------------------------------
// Contributors:  Guillaume Legrain (https://github.com/glegrain)
//                Riccardo Novaglia (https://github.com/richy24)
//                Quentin Busuttil (https://github.com/Buzut)
//                Lapsio (https://github.com/lapsio)
// ----------------------------------------------------------------------------------
// License:       MIT
// ==================================================================================
// 2. System (Hardware)
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
        manufacturer: '',
        model: 'Computer',
        version: '',
        serial: '-',
        uuid: '-'
      };

      if (_linux) {
        exec("dmidecode -t system", function (error, stdout) {
          if (!error) {
            let lines = stdout.toString().split('\n');
            lines.forEach(function (line) {
              if (line.indexOf(':') != -1) {
                if (line.toLowerCase().indexOf('manufacturer') != -1) result.manufacturer = result.manufacturer || line.split(':')[1].trim();
                if (line.toLowerCase().indexOf('product name') != -1) result.model = line.split(':')[1].trim();
                if (line.toLowerCase().indexOf('version') != -1) result.version = result.version || line.split(':')[1].trim();
                if (line.toLowerCase().indexOf('serial number') != -1) result.serial = line.split(':')[1].trim();
                if (line.toLowerCase().indexOf('uuid') != -1) result.uuid = line.split(':')[1].trim();
              }
            });
            if (result.serial.toLowerCase().indexOf('o.e.m.') != -1) result.serial = '-';
            if (result.manufacturer.toLowerCase().indexOf('o.e.m.') != -1) result.manufacturer = '';
            if (result.model.toLowerCase().indexOf('o.e.m.') != -1) result.model = 'Computer';
            if (result.version.toLowerCase().indexOf('o.e.m.') != -1) result.version = '-';

            if (result.manufacturer == '' && result.model == 'Computer' && result.version == '-') {
              // Check Raspberry Pi
              exec("grep Hardware /proc/cpuinfo; grep Serial /proc/cpuinfo; grep Revision /proc/cpuinfo", function (error, stdout) {
                if (!error) {
                  let lines = stdout.toString().split('\n');
                  lines.forEach(function (line) {
                    if (line.indexOf(':') != -1) {
                      if (line.toLowerCase().indexOf('hardware') != -1) result.model = line.split(':')[1].trim();
                      if (line.toLowerCase().indexOf('revision') != -1) result.version = line.split(':')[1].trim();
                      if (line.toLowerCase().indexOf('serial') != -1) result.serial = line.split(':')[1].trim();
                    }
                  });
                  if (result.model == 'BCM2709') {
                    result.manufacturer = 'Raspberry Pi Foundation';
                    result.model = result.model + ' - Pi 2 Model B';
                    if (['a01041', 'a21041'].indexOf(result.version) >= 0) {
                      result.version = result.version + ' - Rev. 1.1'
                    }
                  }
                  if (result.model == 'BCM2708') {
                    result.manufacturer = 'Raspberry Pi Foundation';
                    if (['0002', '0003'].indexOf(result.version) >= 0) {
                      result.model = result.model + ' - Pi Model B';
                      result.version = result.version + ' - Rev 1.0';
                    }
                    if (['0007', '0008', '0009'].indexOf(result.version) >= 0) {
                      result.model = result.model + ' - Pi Model A';
                      result.version = result.version + ' - Rev 2.0';
                    }
                    if (['0004', '0005', '0006', '000d', '000e', '000f'].indexOf(result.version) >= 0) {
                      result.model = result.model + ' - Pi Model B';
                      result.version = result.version + ' - Rev 2.0';
                    }
                    if (['0012'].indexOf(result.version) >= 0) {
                      result.model = result.model + ' - Pi Model A+';
                      result.version = result.version + ' - Rev 1.0';
                    }
                    if (['0010'].indexOf(result.version) >= 0) {
                      result.model = result.model + ' - Pi Model B+';
                      result.version = result.version + ' - Rev 1.0';
                    }
                    if (['0013'].indexOf(result.version) >= 0) {
                      result.model = result.model + ' - Pi Model B+';
                      result.version = result.version + ' - Rev 1.2';
                    }
                  }
                }
                if (callback) { callback(result) }
                resolve(result);
              })
            } else {
              if (callback) { callback(result) }
              resolve(result);
            }
          } else {
            exec("dmesg | grep -i virtual | grep -iE 'vmware|qemu|kvm|xen'", function (error, stdout) {
              if (!error) {
                let lines = stdout.toString().split('\n');
                if (lines.length > 0) result.model = 'Virtual machine'
              }
              if (fs.existsSync('/.dockerenv') || fs.existsSync('/.dockerinit')) {
                result.model = 'Docker Container'
              }
              if (callback) { callback(result) }
              resolve(result);
            });
          }
        })
      }
      if (_darwin) {
        exec("ioreg -c IOPlatformExpertDevice -d 2", function (error, stdout) {
          if (!error) {
            let lines = stdout.toString().split('\n');
            lines.forEach(function (line) {
              line = line.replace(/[<>"]/g, "");
              if (line.indexOf('=') != -1) {
                if (line.toLowerCase().indexOf('manufacturer') != -1) result.manufacturer = line.split('=')[1].trim();
                if (line.toLowerCase().indexOf('model') != -1) result.model = line.split('=')[1].trim();
                if (line.toLowerCase().indexOf('version') != -1) result.version = line.split('=')[1].trim();
                if (line.toLowerCase().indexOf('ioplatformserialnumber') != -1) result.serial = line.split('=')[1].trim();
                if (line.toLowerCase().indexOf('ioplatformuuid') != -1) result.uuid = line.split('=')[1].trim();
              }
            });
          }
          if (callback) { callback(result) }
          resolve(result);
        })
      }
    });
  });
};
