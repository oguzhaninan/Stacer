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
// 12. Internet
// ----------------------------------------------------------------------------------

const os = require('os');
const exec = require('child_process').exec;
const util = require('./util');

let _platform = os.type();

const _linux = (_platform == 'Linux');
const _darwin = (_platform == 'Darwin');
const _windows = (_platform == 'Windows_NT');
const NOT_SUPPORTED = 'not supported';

// --------------------------
// check if external site is available

function inetChecksite(url, callback) {

  return new Promise((resolve, reject) => {
    process.nextTick(() => {
      if (_windows) {
        let error = new Error(NOT_SUPPORTED);
        if (callback) { callback(NOT_SUPPORTED) }
        reject(error);
      }

      let result = {
        url: url,
        ok: false,
        status: 404,
        ms: -1
      };
      if (url) {
        let t = Date.now();
        let args = " -I --connect-timeout 5 -m 5 " + url + " 2>/dev/null | head -n 1 | cut -d ' ' -f2";
        let cmd = "curl";
        exec(cmd + args, function (error, stdout) {
          let statusCode = parseInt(stdout.toString());
          result.status = statusCode || 404;
          result.ok = !error && (statusCode == 200 || statusCode == 301 || statusCode == 302 || statusCode == 304);
          result.ms = (result.ok ? Date.now() - t : -1);
          if (callback) { callback(result) }
          resolve(result);
        })
      } else {
        if (callback) { callback(result) }
        resolve(result);
      }
    });
  });
}

exports.inetChecksite = inetChecksite;

// --------------------------
// check inet latency

function inetLatency(host, callback) {

  // fallback - if only callback is given
  if (util.isFunction(host) && !callback) {
    callback = host;
    host = '';
  }

  host = host || '8.8.8.8';

  return new Promise((resolve, reject) => {
    process.nextTick(() => {
      if (_windows) {
        let error = new Error(NOT_SUPPORTED);
        if (callback) { callback(NOT_SUPPORTED) }
        reject(error);
      }

      let t = Date.now();
      let cmd;
      if (_linux) {
        cmd = "ping -c 2 -w 3 " + host + " | grep rtt | cut -d'/' -f4 | awk '{ print $3 }'";
      }
      if (_darwin) {
        cmd = "ping -c 2 -t 3 " + host + " | grep avg | cut -d'/' -f4 | awk '{ print $3 }'";
      }

      exec(cmd, function (error, stdout) {
        let result = -1;
        if (!error) {
          result = parseFloat(stdout.toString());
        }
        if (callback) { callback(result) }
        resolve(result);
      })
    });
  });
}

exports.inetLatency = inetLatency;
