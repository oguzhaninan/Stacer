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
// 3. Operating System
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

// --------------------------
// Get current time and OS uptime

function time() {
  return {
    current: Date.now(),
    uptime: os.uptime()
  };
}

exports.time = time;

// --------------------------
// Get logo filename of OS distribution

function getLogoFile(distro) {
  distro = distro.toLowerCase();
  let result = 'linux';
  if (_windows) { result = 'windows' }
  else if (distro.indexOf('mac os') != -1) { result = 'apple' }
  else if (distro.indexOf('arch') != -1) { result = 'arch' }
  else if (distro.indexOf('centos') != -1) { result = 'centos' }
  else if (distro.indexOf('coreos') != -1) { result = 'coreos' }
  else if (distro.indexOf('debian') != -1) { result = 'debian' }
  else if (distro.indexOf('elementary') != -1) { result = 'elementary' }
  else if (distro.indexOf('fedora') != -1) { result = 'fedora' }
  else if (distro.indexOf('gentoo') != -1) { result = 'gentoo' }
  else if (distro.indexOf('mageia') != -1) { result = 'mageia' }
  else if (distro.indexOf('mandriva') != -1) { result = 'mandriva' }
  else if (distro.indexOf('manjaro') != -1) { result = 'manjaro' }
  else if (distro.indexOf('mint') != -1) { result = 'mint' }
  else if (distro.indexOf('openbsd') != -1) { result = 'openbsd' }
  else if (distro.indexOf('opensuse') != -1) { result = 'opensuse' }
  else if (distro.indexOf('pclinuxos') != -1) { result = 'pclinuxos' }
  else if (distro.indexOf('puppy') != -1) { result = 'puppy' }
  else if (distro.indexOf('raspbian') != -1) { result = 'raspbian' }
  else if (distro.indexOf('reactos') != -1) { result = 'reactos' }
  else if (distro.indexOf('redhat') != -1) { result = 'redhat' }
  else if (distro.indexOf('slackware') != -1) { result = 'slackware' }
  else if (distro.indexOf('sugar') != -1) { result = 'sugar' }
  else if (distro.indexOf('steam') != -1) { result = 'steam' }
  else if (distro.indexOf('suse') != -1) { result = 'suse' }
  else if (distro.indexOf('mate') != -1) { result = 'ubuntu-mate' }
  else if (distro.indexOf('lubuntu') != -1) { result = 'lubuntu' }
  else if (distro.indexOf('xubuntu') != -1) { result = 'xubuntu' }
  else if (distro.indexOf('ubuntu') != -1) { result = 'ubuntu' }
  return result;
}

// --------------------------
// OS Information

function osInfo(callback) {

  return new Promise((resolve, reject) => {
    process.nextTick(() => {
      if (_windows) {
        let error = new Error(NOT_SUPPORTED);
        if (callback) { callback(NOT_SUPPORTED) }
        reject(error);
      }

      let result = {

        platform: _platform,
        distro: 'unknown',
        release: 'unknown',
        codename: '',
        kernel: os.release(),
        arch: os.arch(),
        hostname: os.hostname(),
        logofile: ''
      };

      if (_linux) {

        exec("cat /etc/*-release", function (error, stdout) {
          //if (!error) {
          /**
           * @namespace
           * @property {string}  DISTRIB_ID
           * @property {string}  NAME
           * @property {string}  DISTRIB_RELEASE
           * @property {string}  VERSION_ID
           * @property {string}  DISTRIB_CODENAME
           */
          let release = {};
          let lines = stdout.toString().split('\n');
          lines.forEach(function (line) {
            if (line.indexOf('=') != -1) {
              release[line.split('=')[0].trim().toUpperCase()] = line.split('=')[1].trim();
            }
          });
          result.distro = (release.DISTRIB_ID || release.NAME || 'unknown').replace(/"/g, '');
          result.logofile = getLogoFile(result.distro);
          result.release = (release.DISTRIB_RELEASE || release.VERSION_ID || 'unknown').replace(/"/g, '');
          result.codename = (release.DISTRIB_CODENAME || '').replace(/"/g, '');
          //}
          if (callback) { callback(result) }
          resolve(result);
        })
      }
      if (_darwin) {
        exec("sw_vers", function (error, stdout) {
          let lines = stdout.toString().split('\n');
          lines.forEach(function (line) {
            if (line.indexOf('ProductName') != -1) {
              result.distro = line.split(':')[1].trim();
              result.logofile = getLogoFile(result.distro);
            }
            if (line.indexOf('ProductVersion') != -1) result.release = line.split(':')[1].trim();
          });
          if (callback) { callback(result) }
          resolve(result);
        })
      }
    });
  });
}

exports.osInfo = osInfo;

function versions(callback) {
  return new Promise((resolve, reject) => {
    process.nextTick(() => {
      if (_windows) {
        let error = new Error(NOT_SUPPORTED);
        if (callback) { callback(NOT_SUPPORTED) }
        reject(error);
      }

      let result = {
        kernel: os.release(),
        node: process.versions.node,
        v8: process.versions.v8,
        npm: '',
        pm2: '',
        openssl: process.versions.openssl
      };
      let lines = [];
      exec("npm -v", function (error, stdout) {
        if (!error) {
          result.npm = stdout.toString().split('\n')[0];
        }
        exec("pm2 -v", function (error, stdout) {
          if (!error) {
            lines = stdout.toString().split('\n');
            if (lines.length >= 2) {
              result.pm2 = lines[lines.length-2];
            }
          }
          if (callback) { callback(result) }
          resolve(result);
        });
      });
    });
  });
}

exports.versions = versions;

function shell(callback) {
  return new Promise((resolve, reject) => {
    process.nextTick(() => {
      if (_windows) {
        let error = new Error(NOT_SUPPORTED);
        if (callback) { callback(NOT_SUPPORTED) }
        reject(error);
      }

      let result = '';
      exec("echo $SHELL", function (error, stdout) {
        if (!error) {
          result = stdout.toString().split('\n')[0];
        }
        if (callback) { callback(result) }
        resolve(result);
      });
    });
  });
}

exports.shell = shell;
