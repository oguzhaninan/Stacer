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
//
// Sections
// --------------------------------
// 1. General
// 2. System (HW)
// 3. OS - Operating System
// 4. CPU
// 5. Memory
// 6. Battery
// 7. Graphics
// 8. File System
// 9. Network
// 10. Processes
// 11. Users/Sessions
// 12. Internet
// 13. Docker
// 14. GetAll - get all data
//
// ==================================================================================
//
// Installation
// --------------------------------
//
// # npm install systeminformation --save
//
// Since version 2.0 systeminformation has no more dependencies.
//
// ==================================================================================
//
// Usage
// --------------------------------
// All functions (except `version` and `time`) are asynchronous functions. Here a small example how to use them:
//
// var si = require('systeminformation');
//
// // callback style
// si.cpu(function(data) {
//	  console.log('CPU-Information:');
//	  console.log(data);
// })
//
// // promises style
// si.cpu()
// 	.then(data => console.log(data))
// 	.catch(error => console.error(error));
//
// ==================================================================================
//
// Comments
// --------------------------------
//
// This library is still work in progress. Version 3 comes with further improvements. First it
// requires now node.js version 4.0 and above. Another big change is, that all functions now
// return promises. You can use them like before with callbacks OR with promises (see documentation
// below. I am sure, there is for sure room for improvement. I was only able to test it on several
// Debian, Raspbian, Ubuntu distributions as well as OS X (Mavericks, Yosemite, El Captain).
// Since version 2 nearly all functionality is available on OS X/Darwin platforms.
// But be careful, this library will definitely NOT work on Windows platforms!
//
// Comments, suggestions & reports are very welcome!
//
// ==================================================================================
//
// Version history
// --------------------------------
//
// version	date	      comment
// 3.13.0	  2016-11-23	added shell (determines standard shell)
// 3.12.0	  2016-11-17	refactoring and extended currentLoad (better OSX coverage and added irq load)
// 3.11.2	  2016-11-16	blockDevices: improved for older lsblk versions
// 3.11.1	  2016-11-16	fixed small bug in blockDevices
// 3.11.0	  2016-11-15	blockDevices for OSX and extended blockDevices
// 3.10.2	  2016-11-14	bug fix fsSize on OSX
// 3.10.1	  2016-11-14	optimization fsStats, disksIO, networkStats
// 3.10.0	  2016-11-12	added blockDevices, fixed fsSize,  added file system type
// 3.9.0	  2016-11-11	added MAC address to networkInterfaces, fixed currentLoad
// 3.8.1	  2016-11-04	updated docs
// 3.8.0	  2016-11-04	added dockerContainerProcesses
// 3.7.1	  2016-11-03	code refactoring
// 3.7.0	  2016-11-02	extended docker stats, and no longer relying on curl (version conflicts)
// 3.6.0	  2016-09-16	added versions (kernel, ssl, node, npm, pm2, ...)
// 3.5.1	  2016-09-14	bugfix graphics info
// 3.5.0	  2016-09-14	added graphics info (controller, display)
// 3.4.4	  2016-09-02	tiny fixes system.model, getDefaultNetworkInterface
// 3.4.3	  2016-09-02	tiny bug fix fsStats, disksIO OSX
// 3.4.2	  2016-09-01	improved default network interface
// 3.4.1	  2016-08-30	updated docs
// 3.4.0	  2016-08-30	rewritten current process cpu usage (linux)
// 3.3.0	  2016-08-24	added process list
// 3.2.1	  2016-08-20	updated docs, improvement system
// 3.2.0	  2016-08-19	added battery info
// 3.1.1	  2016-08-18	improved system and os detection (vm, ...), bug fix disksIO
// 3.1.0	  2016-08-18	added docker stats
// 3.0.1	  2016-08-17	Bug-Fix disksIO, users, updated docs
// 3.0.0	  2016-08-03	new major version 3.0
// 2.0.5	  2016-02-22	some more tiny correction ...
// 2.0.4	  2016-02-22	tiny correction - removed double quotes CPU brand, ...
// 2.0.3	  2016-02-22	optimized cpuCurrentspeed
// 2.0.2	  2016-02-22	added CoreOS identification
// 2.0.1	  2016-01-07	minor patch
// 2.0.0	  2016-01-07	new major version 2.0
// 1.0.7	  2015-11-27	fixed: si.network_speed()
// 1.0.6	  2015-09-17	fixed: si.users()
// 1.0.5	  2015-09-14	updated dependencies
// 1.0.4	  2015-07-18	updated docs
// 1.0.3	  2015-07-18	bugfix cpu cores
// 1.0.2	  2015-07-18	bugfix cpu_currentspeed, cpu_temperature
// 1.0.1	  2015-07-18	documentation update
// 1.0.0	  2015-07-18	bug-fixes, version bump, published as npm component
// 0.0.3	  2014-04-14	bug-fix (cpu_speed)
// 0.0.2	  2014-03-14	Optimization FS-Speed & CPU current speed
// 0.0.1	  2014-03-13	initial release
//
// ==================================================================================

// ----------------------------------------------------------------------------------
// Dependencies
// ----------------------------------------------------------------------------------

const os = require('os')
  , fs = require('fs');

const lib_version = require('../package.json').version;
const util = require('./util');
const system = require('./system');
const osInfo = require('./osinfo');
const cpu = require('./cpu');
const mem = require('./memory');
const battery = require('./battery');
const graphics = require('./graphics');
const filesystem = require('./filesystem');
const network = require('./network');
const processes = require('./processes');
const users = require('./users');
const internet = require('./internet');
const docker = require('./docker');

let _platform = os.type();
let _windows = (_platform == 'Windows_NT');

const NOT_SUPPORTED = 'not supported';

// ----------------------------------------------------------------------------------
// 1. General
// ----------------------------------------------------------------------------------

function version() {
  return lib_version;
}

// ----------------------------------------------------------------------------------
// 14. get all
// ----------------------------------------------------------------------------------

// --------------------------
// get static data - they should not change until restarted

function getStaticData(callback) {

  return new Promise((resolve, reject) => {
    process.nextTick(() => {
      if (_windows) {
        let error = new Error(NOT_SUPPORTED);
        if (callback) { callback(NOT_SUPPORTED) }
        reject(error);
      }

      let data = {};

      data.version = version();

      system().then(res => {
        data.system = res;
        osInfo.osInfo().then(res => {
          data.os = res;
          osInfo.versions().then(res => {
            data.versions = res;
            cpu.cpu().then(res => {
              data.cpu = res;
              graphics.graphics().then(res => {
                data.graphics = res;
                network.networkInterfaces().then(res => {
                  data.net = res;
                  if (callback) { callback(data) }
                  resolve(data);
                })
              })
            })
          })
        })
      })
    });
  });
}

// --------------------------
// get all dynamic data - e.g. for monitoring agents
// may take some seconds to get all data
// --------------------------
// 2 additional parameters needed
// - srv: 		comma separated list of services to monitor e.g. "mysql, apache, postgresql"
// - iface:	define network interface for which you like to monitor network speed e.g. "eth0"

function getDynamicData(srv, iface, callback) {

  if (util.isFunction(iface)) {
    callback = iface;
    iface = '';
  }
  if (util.isFunction(srv)) {
    callback = srv;
    srv = '';
  }

  return new Promise((resolve, reject) => {
    process.nextTick(() => {
      if (_windows) {
        let error = new Error(NOT_SUPPORTED);
        if (callback) { callback(NOT_SUPPORTED) }
        reject(error);
      }

      iface = iface || network.getDefaultNetworkInterface();
      srv = srv || '';

      // use closure to track ƒ completion
      let functionProcessed = (function () {
        let totalFunctions = 14;

        return function () {
          if (--totalFunctions === 0) {
            if (callback) { callback(data) }
            resolve(data);
          }
        };
      })();

      // var totalFunctions = 14;
      // function functionProcessed() {
      //   if (--totalFunctions === 0) {
      //     if (callback) { callback(data) }
      //     resolve(data);
      //   }
      // }

      let data = {};

      // get time
      data.time = osInfo.time();

      /**
       * @namespace
       * @property {Object}  versions
       * @property {string}  versions.node
       * @property {string}  versions.v8
       */
      data.node = process.versions.node;
      data.v8 = process.versions.v8;

      cpu.cpuCurrentspeed().then(res => {
        data.cpuCurrentspeed = res;
        functionProcessed();
      });

      users.users().then(res => {
        data.users = res;
        functionProcessed();
      });

      processes.processes().then(res => {
        data.processes = res;
        functionProcessed();
      });

      processes.currentLoad().then(res => {
        data.currentLoad = res;
        functionProcessed();
      });

      cpu.cpuTemperature().then(res => {
        data.temp = res;
        functionProcessed();
      });

      network.networkStats(iface).then(res => {
        data.networkStats = res;
        functionProcessed();
      });

      network.networkConnections().then(res => {
        data.networkConnections = res;
        functionProcessed();
      });

      mem().then(res => {
        data.mem = res;
        functionProcessed();
      });

      battery().then(res => {
        data.battery = res;
        functionProcessed();
      });

      processes.services(srv).then(res => {
        data.services = res;
        functionProcessed();
      });

      filesystem.fsSize().then(res => {
        data.fsSize = res;
        functionProcessed();
      });

      filesystem.fsStats().then(res => {
        data.fsStats = res;
        functionProcessed();
      });

      filesystem.disksIO().then(res => {
        data.disksIO = res;
        functionProcessed();
      });

      internet.inetLatency().then(res => {
        data.inetLatency = res;
        functionProcessed();
      });
    });
  });
}

// --------------------------
// get all data at once
// --------------------------
// 2 additional parameters needed
// - srv: 		comma separated list of services to monitor e.g. "mysql, apache, postgresql"
// - iface:	define network interface for which you like to monitor network speed e.g. "eth0"

function getAllData(srv, iface, callback) {

  return new Promise((resolve, reject) => {
    process.nextTick(() => {
      if (_windows) {
        let error = new Error(NOT_SUPPORTED);
        if (callback) { callback(NOT_SUPPORTED) }
        reject(error);
      }

      let data = {};

      getStaticData().then(res => {
        data = res;
        getDynamicData(srv, iface).then(res => {
          for (let key in res) {
            if (res.hasOwnProperty(key)) {
              data[key] = res[key];
            }
          }
          if (callback) { callback(data) }
          resolve(data);
        });
      })
    });
  });
}

// ----------------------------------------------------------------------------------
// export all libs
// ----------------------------------------------------------------------------------

exports.version = version;
exports.system = system;

exports.time = osInfo.time;
exports.osInfo = osInfo.osInfo;
exports.versions = osInfo.versions;
exports.shell = osInfo.shell;

exports.cpu = cpu.cpu;
exports.cpuCurrentspeed = cpu.cpuCurrentspeed;
exports.cpuTemperature = cpu.cpuTemperature;

exports.mem = mem;

exports.battery = battery;

exports.graphics = graphics.graphics;

exports.fsSize = filesystem.fsSize;
exports.blockDevices = filesystem.blockDevices;
exports.fsStats = filesystem.fsStats;
exports.disksIO = filesystem.disksIO;

exports.networkInterfaceDefault = network.networkInterfaceDefault;
exports.networkInterfaces = network.networkInterfaces;
exports.networkStats = network.networkStats;
exports.networkConnections = network.networkConnections;

exports.currentLoad = processes.currentLoad;
exports.fullLoad = processes.fullLoad;
exports.services = processes.services;
exports.processes = processes.processes;
exports.processLoad = processes.processLoad;

exports.users = users.users;

exports.inetChecksite = internet.inetChecksite;
exports.inetLatency = internet.inetLatency;

exports.dockerContainers = docker.dockerContainers;
exports.dockerContainerStats = docker.dockerContainerStats;
exports.dockerContainerProcesses = docker.dockerContainerProcesses;
exports.dockerAll = docker.dockerAll;

exports.getStaticData = getStaticData;
exports.getDynamicData = getDynamicData;
exports.getAllData = getAllData;
