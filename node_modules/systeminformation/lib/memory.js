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
// 5. Memory
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

// _______________________________________________________________________________________
// |                         R A M                              |          H D           |
// |______________________|_________________________|           |                        |
// |        active             buffers/cache        |           |                        |
// |________________________________________________|___________|_________|______________|
// |                     used                            free   |   used       free      |
// |____________________________________________________________|________________________|
// |                        total                               |          swap          |
// |____________________________________________________________|________________________|

// free (older versions)
// ----------------------------------
// # free
//              total       used        free     shared    buffers     cached
// Mem:         16038 (1)   15653 (2)   384 (3)  0 (4)     236 (5)     14788 (6)
// -/+ buffers/cache:       628 (7)     15409 (8)
// Swap:        16371         83      16288
//
// |------------------------------------------------------------|
// |                           R A M                            |
// |______________________|_____________________________________|
// | active (2-(5+6) = 7) |  available (3+5+6 = 8)              |
// |______________________|_________________________|___________|
// |        active        |  buffers/cache (5+6)    |           |
// |________________________________________________|___________|
// |                   used (2)                     | free (3)  |
// |____________________________________________________________|
// |                          total (1)                         |
// |____________________________________________________________|

//
// free (since free von procps-ng 3.3.10)
// ----------------------------------
// # free
//              total       used        free     shared    buffers/cache   available
// Mem:         16038 (1)   628 (2)     386 (3)  0 (4)     15024 (5)     14788 (6)
// Swap:        16371         83      16288
//
// |------------------------------------------------------------|
// |                           R A M                            |
// |______________________|_____________________________________|
// |                      |      available (6) estimated        |
// |______________________|_________________________|___________|
// |     active (2)       |   buffers/cache (5)     | free (3)  |
// |________________________________________________|___________|
// |                          total (1)                         |
// |____________________________________________________________|
//
// Reference: http://www.software-architect.net/blog/article/date/2015/06/12/-826c6e5052.html

module.exports = function (callback) {

  return new Promise((resolve, reject) => {
    process.nextTick(() => {
      if (_windows) {
        let error = new Error(NOT_SUPPORTED);
        if (callback) { callback(NOT_SUPPORTED) }
        reject(error);
      }

      let result = {
        total: os.totalmem(),
        free: os.freemem(),
        used: os.totalmem() - os.freemem(),

        active: os.totalmem() - os.freemem(),     // temporarily (fallback)
        available: os.freemem(),                  // temporarily (fallback)
        buffcache: 0,

        swaptotal: 0,
        swapused: 0,
        swapfree: 0
      };

      if (_linux) {
        exec("free -b", function (error, stdout) {
          if (!error) {
            let lines = stdout.toString().split('\n');

            let mem = lines[1].replace(/ +/g, " ").split(' ');
            result.total = parseInt(mem[1]);
            result.free = parseInt(mem[3]);

            if (lines.length === 4) {                   // free (since free von procps-ng 3.3.10)
              result.buffcache = parseInt(mem[5]);
              result.available = parseInt(mem[6]);
              mem = lines[2].replace(/ +/g, " ").split(' ');
            } else {                                    // free (older versions)
              result.buffcache = parseInt(mem[5]) + parseInt(mem[6]);
              result.available = result.free + result.buffcache;
              mem = lines[3].replace(/ +/g, " ").split(' ');
            }
            result.active = result.total - result.free - result.buffcache;

            result.swaptotal = parseInt(mem[1]);
            result.swapfree = parseInt(mem[3]);
            result.swapused = parseInt(mem[2]);

          }
          if (callback) { callback(result) }
          resolve(result);
        });
      }
      if (_darwin) {
        exec("vm_stat | grep 'Pages active'", function (error, stdout) {
          if (!error) {
            let lines = stdout.toString().split('\n');

            result.active = parseInt(lines[0].split(':')[1]) * 4096;
            result.buffcache = result.used - result.active;
            result.available = result.free + result.buffcache;
          }
          exec("sysctl -n vm.swapusage", function (error, stdout) {
            if (!error) {
              let lines = stdout.toString().split('\n');
              if (lines.length > 0) {
                let line = lines[0].replace(/,/g, ".").replace(/M/g, "");
                line = line.trim().split('  ');
                for (let i = 0; i < line.length; i++) {
                  if (line[i].toLowerCase().indexOf('total') != -1) result.swaptotal = parseFloat(line[i].split('=')[1].trim()) * 1024 * 1024;
                  if (line[i].toLowerCase().indexOf('used') != -1) result.swapused = parseFloat(line[i].split('=')[1].trim()) * 1024 * 1024;
                  if (line[i].toLowerCase().indexOf('free') != -1) result.swapfree = parseFloat(line[i].split('=')[1].trim()) * 1024 * 1024;

                }
              }
            }
            if (callback) { callback(result) }
            resolve(result);
          });
        });
      }
    });
  });
};
