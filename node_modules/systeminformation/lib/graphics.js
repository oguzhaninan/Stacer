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
// 7. Graphics (controller, display)
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

function graphics(callback) {

  function parseLinesDarwin(lines) {
    let starts = [];
    let level = -1;
    let lastlevel = -1;
    let controllers = [];
    let displays = [];
    let currentController = {};
    let currentDisplay = {};
    for (let i = 0; i < lines.length; i++) {
      if ('' != lines[i].trim()) {
        let start = lines[i].search(/\S|$/);
        if (-1 == starts.indexOf(start)) {
          starts.push(start);
        }
        level = starts.indexOf(start);
        if (level < lastlevel) {
          if (Object.keys(currentController).length > 0) {// just changed to Displays
            controllers.push(currentController);
            currentController = {};
          }
          if (Object.keys(currentDisplay).length > 0) {// just changed to Displays
            displays.push(currentDisplay);
            currentDisplay = {};
          }
        }
        lastlevel = level;
        let parts = lines[i].split(':');
        if (2 == level) {       // grafics controller level
          if (parts.length > 1 && parts[0].replace(/ +/g, "").toLowerCase().indexOf('chipsetmodel') != -1) currentController.model = parts[1].trim();
          if (parts.length > 1 && parts[0].replace(/ +/g, "").toLowerCase().indexOf('bus') != -1) currentController.bus = parts[1].trim();
          if (parts.length > 1 && parts[0].replace(/ +/g, "").toLowerCase().indexOf('vendor') != -1) currentController.vendor = parts[1].split('(')[0].trim();
          if (parts.length > 1 && parts[0].replace(/ +/g, "").toLowerCase().indexOf('vram(total)') != -1) {
            currentController.vram = parseInt(parts[1]);    // in MB
            currentController.vramDynamic = false;
          }
          if (parts.length > 1 && parts[0].replace(/ +/g, "").toLowerCase().indexOf('vram(dynamic,max)') != -1) {
            currentController.vram = parseInt(parts[1]);    // in MB
            currentController.vramDynamic = true;
          }
        }
        if (3 == level) {       // display controller level
          if (parts.length > 1 && '' == parts[1]) {
            currentDisplay.model = parts[0].trim();
            currentDisplay.main = false;
            currentDisplay.builtin = false;
            currentDisplay.connection = '';
            currentDisplay.sizex = -1;
            currentDisplay.sizey = -1;
          }
        }
        if (4 == level) {       // display controller details level
          if (parts.length > 1 && parts[0].replace(/ +/g, "").toLowerCase().indexOf('resolution') != -1) {
            let resolution = parts[1].split('x');
            currentDisplay.resolutionx = (resolution.length > 1 ? parseInt(resolution[0]) : 0);
            currentDisplay.resolutiony = (resolution.length > 1 ? parseInt(resolution[1]) : 0);
          }
          if (parts.length > 1 && parts[0].replace(/ +/g, "").toLowerCase().indexOf('pixeldepth') != -1) currentDisplay.pixeldepth = parseInt(parts[1]); // in BIT
          if (parts.length > 1 && parts[0].replace(/ +/g, "").toLowerCase().indexOf('maindisplay') != -1 && parts[1].replace(/ +/g, "").toLowerCase() == 'yes') currentDisplay.main = true;
          if (parts.length > 1 && parts[0].replace(/ +/g, "").toLowerCase().indexOf('built-in') != -1 && parts[1].replace(/ +/g, "").toLowerCase() == 'yes') {
            currentDisplay.builtin = true;
            currentDisplay.connection = '';
          }
          if (parts.length > 1 && parts[0].replace(/ +/g, "").toLowerCase().indexOf('connectiontype') != -1) {
            currentDisplay.builtin = false;
            currentDisplay.connection = parts[1].trim();
          }
        }
      }
    }
    if (Object.keys(currentController).length > 0) {// just changed to Displays
      controllers.push(currentController);
    }
    if (Object.keys(currentDisplay).length > 0) {// just changed to Displays
      displays.push(currentDisplay);
    }
    return ({
      controllers: controllers,
      displays: displays
    })
  }

  function parseLinesLinuxControllers(lines) {
    let controllers = [];
    let currentController = {};
    let is_vga = false;
    for (let i = 0; i < lines.length; i++) {
      if ('' != lines[i].trim()) {
        if (' ' != lines[i][0] && '\t' != lines[i][0]) {        // first line of new entry
          let vgapos = lines[i].toLowerCase().indexOf('vga');
          if (vgapos != -1) {         // VGA
            if (Object.keys(currentController).length > 0) {// already a controller found
              controllers.push(currentController);
              currentController = {};
            }
            is_vga = true;
            let endpos = lines[i].search(/\[[0-9a-f]{4}:[0-9a-f]{4}]|$/);
            let parts = lines[i].substr(vgapos, endpos - vgapos).split(':');
            if (parts.length > 1) {
              parts[1] = parts[1].trim();
              if (parts[1].toLowerCase().indexOf('corporation')) {
                currentController.vendor = parts[1].substr(0, parts[1].toLowerCase().indexOf('corporation') + 11).trim();
                currentController.model = parts[1].substr(parts[1].toLowerCase().indexOf('corporation') + 11, 200).trim().split('(')[0];
                currentController.bus = '';
                currentController.vram = -1;
                currentController.vramDynamic = false;
              }
            }

          } else {
            is_vga = false;
          }
        }
        if (is_vga) { // within VGA details
          let parts = lines[i].split(':');
          if (parts.length > 1 && parts[0].replace(/ +/g, "").toLowerCase().indexOf('devicename') != -1 && parts[0].toLowerCase().indexOf('onboard') != -1)  currentController.bus = 'Onboard';
          if (parts.length > 1 && parts[0].replace(/ +/g, "").toLowerCase().indexOf('region') != -1 && parts[1].toLowerCase().indexOf('memory') != -1) {
            let memparts = parts[1].split("=");
            if (memparts.length > 1) {
              currentController.vram = parseInt(memparts[1]);
            }
          }
        }
      }
    }
    if (Object.keys(currentController).length > 0) {// still controller information available
      controllers.push(currentController);
    }
    return (controllers)
  }

  function parseLinesLinuxEdid(edid) {
    // parsen EDID
    // --> model
    // --> resolutionx
    // --> resolutiony
    // --> builtin = false
    // --> pixeldepth (?)
    // --> sizex
    // --> sizey
    let result = {};
    // find first "Detailed Timing Description"
    let start = 108;
    if (edid.substr(start, 6) == '000000') {
      start += 36;
    }
    if (edid.substr(start, 6) == '000000') {
      start += 36;
    }
    if (edid.substr(start, 6) == '000000') {
      start += 36;
    }
    if (edid.substr(start, 6) == '000000') {
      start += 36;
    }
    result.resolutionx = parseInt('0x0' + edid.substr(start + 8, 1) + edid.substr(start + 4, 2));
    result.resolutiony = parseInt('0x0' + edid.substr(start + 14, 1) + edid.substr(start + 10, 2));
    result.sizex = parseInt('0x0' + edid.substr(start + 28, 1) + edid.substr(start + 24, 2));
    result.sizey = parseInt('0x0' + edid.substr(start + 29, 1) + edid.substr(start + 26, 2));
    // monitor name
    start = edid.indexOf('000000fc00'); // find first "Monitor Description Data"
    if (start >= 0) {
      let model_raw = edid.substr(start + 10, 26);
      if (model_raw.indexOf('0a') != -1) {
        model_raw = model_raw.substr(0, model_raw.indexOf('0a'))
      }
      result.model = model_raw.match(/.{1,2}/g).map(function (v) {
        return String.fromCharCode(parseInt(v, 16));
      }).join('');
    } else {
      result.model = '';
    }
    return result;
  }

  function parseLinesLinuxDisplays(lines, depth) {
    let displays = [];
    let currentDisplay = {};
    let is_edid = false;
    let edid_raw = '';
    let start = 0;
    for (let i = 1; i < lines.length; i++) {        // start with second line
      if ('' != lines[i].trim()) {
        if (' ' != lines[i][0] && '\t' != lines[i][0] && lines[i].toLowerCase().indexOf(' connected ') != -1) {        // first line of new entry
          if (Object.keys(currentDisplay).length > 0) {         // push last display to array
            displays.push(currentDisplay);
            currentDisplay = {};
          }
          let parts = lines[i].split(' ');
          currentDisplay.connection = parts[0];
          currentDisplay.main = (parts[2] == 'primary');
          currentDisplay.builtin = (parts[0].toLowerCase().indexOf('edp') >= 0)
        }

        // try to read EDID information
        if (is_edid) {
          if (lines[i].search(/\S|$/) > start) {
            edid_raw += lines[i].toLowerCase().trim();
          } else {
            // parsen EDID
            let edid_decoded = parseLinesLinuxEdid(edid_raw);
            currentDisplay.model = edid_decoded.model;
            currentDisplay.resolutionx = edid_decoded.resolutionx;
            currentDisplay.resolutiony = edid_decoded.resolutiony;
            currentDisplay.sizex = edid_decoded.sizex;
            currentDisplay.sizey = edid_decoded.sizey;
            currentDisplay.pixeldepth = depth;
            is_edid = false;
          }
        }
        if (lines[i].toLowerCase().indexOf('edid:') != -1) {
          is_edid = true;
          start = lines[i].search(/\S|$/);
        }
      }
    }

    // pushen displays
    if (Object.keys(currentDisplay).length > 0) {         // still information there
      displays.push(currentDisplay);
    }
    return displays
  }

  // function starts here
  return new Promise((resolve, reject) => {
    process.nextTick(() => {
      if (_windows) {
        let error = new Error(NOT_SUPPORTED);
        if (callback) {
          callback(NOT_SUPPORTED)
        }
        reject(error);
      }
      let result = {
        controllers: [],
        displays: []
      };
      if (_darwin) {
        let cmd = 'system_profiler SPDisplaysDataType';
        exec(cmd, function (error, stdout) {
          if (!error) {
            let lines = stdout.toString().split('\n');
            result = parseLinesDarwin(lines);
          }
          if (callback) {
            callback(result)
          }
          resolve(result);
        })
      }
      if (_linux) {
        let cmd = 'lspci -vvv  2>/dev/null';
        exec(cmd, function (error, stdout) {
          if (!error) {
            let lines = stdout.toString().split('\n');
            result.controllers = parseLinesLinuxControllers(lines);
          }
          let cmd = "xdpyinfo 2>/dev/null | grep 'depth of root window' | awk '{ print $5 }'";
          exec(cmd, function (error, stdout) {
            let depth = 0;
            if (!error) {
              let lines = stdout.toString().split('\n');
              depth = parseInt(lines[0]) || 0;
            }
            let cmd = 'xrandr --verbose 2>/dev/null';
            exec(cmd, function (error, stdout) {
              if (!error) {
                let lines = stdout.toString().split('\n');
                result.displays = parseLinesLinuxDisplays(lines, depth);
              }
              if (callback) {
                callback(result)
              }
              resolve(result);
            })
          })
        })
      }
    });
  });
}

exports.graphics = graphics;
