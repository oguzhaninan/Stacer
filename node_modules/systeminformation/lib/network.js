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
// 9. Network
// ----------------------------------------------------------------------------------

const os = require('os');
const exec = require('child_process').exec;
const execSync = require('child_process').execSync;
const fs = require('fs');
const util = require('./util');

let _platform = os.type();

const _linux = (_platform == 'Linux');
const _darwin = (_platform == 'Darwin');
const _windows = (_platform == 'Windows_NT');
const NOT_SUPPORTED = 'not supported';

let _network = {};
let _default_iface;

function getDefaultNetworkInterface() {

  if (!_default_iface) {
    let ifacename = '';
    let cmd = (_linux ? "route 2>/dev/null | grep default | awk '{print $8}'" : "route get 0.0.0.0 2>/dev/null | grep interface: | awk '{print $2}'");
    let result = execSync(cmd);
    ifacename = result.toString().split('\n')[0];

    if (!ifacename) {         // fallback - "first" external interface
      const sortObject = o => Object.keys(o).sort().reduce((r, k) => (r[k] = o[k], r), {});

      let ifaces = sortObject(os.networkInterfaces());

      for (let dev in ifaces) {
        if (ifaces.hasOwnProperty(dev)) {
          ifaces[dev].forEach(function (details) {
            if (details && details.internal == false) {
              ifacename = ifacename || dev;
            }
          })
        }
      }
    }
    if (ifacename) _default_iface = ifacename;
  }
  return _default_iface;
}

exports.getDefaultNetworkInterface = getDefaultNetworkInterface;

function networkInterfaceDefault(callback) {

  return new Promise((resolve, reject) => {
    process.nextTick(() => {
      if (_windows) {
        let error = new Error(NOT_SUPPORTED);
        if (callback) { callback(NOT_SUPPORTED) }
        reject(error);
      }

      let result = getDefaultNetworkInterface();
      if (callback) { callback(result) }
      resolve(result);
    });
  });
}

exports.networkInterfaceDefault = networkInterfaceDefault;

// --------------------------
// NET - interfaces

function networkInterfaces(callback) {

  return new Promise((resolve) => {
    process.nextTick(() => {
      let ifaces = os.networkInterfaces();
      let result = [];

      for (let dev in ifaces) {
        let ip4 = '';
        let ip6 = '';
        let mac = '';
        if (ifaces.hasOwnProperty(dev)) {
          ifaces[dev].forEach(function (details) {
            if (details.family == 'IPv4') {
              ip4 = details.address
            }
            if (details.family == 'IPv6') {
              ip6 = details.address
            }
            mac = details.mac
          });
          let internal = (ifaces[dev] && ifaces[dev][0]) ? ifaces[dev][0].internal : null;
          result.push({ iface: dev, ip4: ip4, ip6: ip6, mac: mac, internal: internal })
        }
      }
      if (callback) { callback(result) }
      resolve(result);
    });
  });
}

exports.networkInterfaces = networkInterfaces;

// --------------------------
// NET - Speed

function calcNetworkSpeed(iface, rx, tx, operstate) {
  let result = {
    iface: iface,
    operstate: operstate,
    rx: 0,
    tx: 0,
    rx_sec: -1,
    tx_sec: -1,
    ms: 0
  };

  if (_network[iface] && _network[iface].ms) {
    result.rx = rx;
    result.tx = tx;
    result.ms = Date.now() - _network[iface].ms;
    result.rx_sec = (rx - _network[iface].rx) / (result.ms / 1000);
    result.tx_sec = (tx - _network[iface].tx) / (result.ms / 1000);
    _network[iface].rx = rx;
    _network[iface].tx = tx;
    _network[iface].rx_sec = result.rx_sec;
    _network[iface].tx_sec = result.tx_sec;
    _network[iface].ms = Date.now();
    _network[iface].last_ms = result.ms;
  } else {
    result.rx = rx;
    result.tx = tx;
    if (!_network[iface]) _network[iface] = {};
    _network[iface].rx = rx;
    _network[iface].tx = tx;
    _network[iface].rx_sec = -1;
    _network[iface].tx_sec = -1;
    _network[iface].ms = Date.now();
    _network[iface].last_ms = 0;
  }
  return result;
}

function networkStats(iface, callback) {

  // fallback - if only callback is given
  if (util.isFunction(iface) && !callback) {
    callback = iface;
    iface = '';
  }

  return new Promise((resolve, reject) => {
    process.nextTick(() => {
      if (_windows) {
        let error = new Error(NOT_SUPPORTED);
        if (callback) { callback(NOT_SUPPORTED) }
        reject(error);
      }

      _default_iface = _default_iface || getDefaultNetworkInterface();
      iface = iface || _default_iface; // (_darwin ? 'en0' : 'eth0');

      let result = {
        iface: iface,
        operstate: 'unknown',
        rx: 0,
        tx: 0,
        rx_sec: -1,
        tx_sec: -1,
        ms: 0
      };

      let operstate = 'unknown';
      let rx = 0;
      let tx = 0;

      let cmd, lines, stats;
      if (!_network[iface] || (_network[iface] && !_network[iface].ms) || (_network[iface] && _network[iface].ms && Date.now() - _network[iface].ms >= 500)) {
        if (_linux) {
          if (fs.existsSync('/sys/class/net/' + iface)) {
            cmd =
              "cat /sys/class/net/" + iface + "/operstate; " +
              "cat /sys/class/net/" + iface + "/statistics/rx_bytes; " +
              "cat /sys/class/net/" + iface + "/statistics/tx_bytes; ";
            exec(cmd, function (error, stdout) {
              if (!error) {
                lines = stdout.toString().split('\n');
                operstate = lines[0].trim();
                rx = parseInt(lines[1]);
                tx = parseInt(lines[2]);

                result = calcNetworkSpeed(iface, rx, tx, operstate);

              }
              if (callback) { callback(result) }
              resolve(result);
            });
          } else {
            if (callback) { callback(result) }
            resolve(result);
          }
        }
        if (_darwin) {
          cmd = "ifconfig " + iface + " | grep 'status'";
          exec(cmd, function (error, stdout) {
            result.operstate = (stdout.toString().split(':')[1] || '').trim();
            result.operstate = (result.operstate || '').toLowerCase();
            result.operstate = (result.operstate == 'active' ? 'up' : (result.operstate == 'inactive' ? 'down' : 'unknown'));
            cmd = "netstat -bI " + iface;
            exec(cmd, function (error, stdout) {
              if (!error) {
                lines = stdout.toString().split('\n');
                // if there is less than 2 lines, no information for this interface was found
                if (lines.length > 1 && lines[1].trim() != '') {
                  // skip header line
                  // use the second line because it is tied to the NIC instead of the ipv4 or ipv6 address
                  stats = lines[1].replace(/ +/g, " ").split(' ');
                  rx = parseInt(stats[6]);
                  tx = parseInt(stats[9]);

                  result = calcNetworkSpeed(iface, rx, tx, operstate);
                }
              }
              if (callback) { callback(result) }
              resolve(result);
            });
          });
        }
      } else {
        result.rx = _network[iface].rx;
        result.tx = _network[iface].tx;
        result.rx_sec = _network[iface].rx_sec;
        result.tx_sec = _network[iface].tx_sec;
        result.ms = _network[iface].last_ms;
        if (callback) { callback(result) }
        resolve(result);
      }
    });
  });
}

exports.networkStats = networkStats;

// --------------------------
// NET - connections (sockets)

function networkConnections(callback) {

  return new Promise((resolve, reject) => {
    process.nextTick(() => {
      if (_windows) {
        let error = new Error(NOT_SUPPORTED);
        if (callback) { callback(NOT_SUPPORTED) }
        reject(error);
      }

      let result = [];
      if (_linux) {
        let cmd = "netstat -tuna | grep 'ESTABLISHED\\|SYN_SENT\\|SYN_RECV\\|FIN_WAIT1\\|FIN_WAIT2\\|TIME_WAIT\\|CLOSE\\|CLOSE_WAIT\\|LAST_ACK\\|LISTEN\\|CLOSING\\|UNKNOWN\\|VERBUNDEN'";
        exec(cmd, function (error, stdout) {
          if (!error) {
            let lines = stdout.toString().split('\n');
            lines.forEach(function (line) {
              line = line.replace(/ +/g, " ").split(' ');
              if (line.length >= 6) {
                let localip = line[3];
                let localport = '';
                let localaddress = line[3].split(':');
                if (localaddress.length > 1) {
                  localport = localaddress[localaddress.length - 1];
                  localaddress.pop();
                  localip = localaddress.join(':');
                }
                let peerip = line[4];
                let peerport = '';
                let peeraddress = line[4].split(':');
                if (peeraddress.length > 1) {
                  peerport = peeraddress[peeraddress.length - 1];
                  peeraddress.pop();
                  peerip = peeraddress.join(':');
                }
                let connstate = line[5];
                if (connstate == 'VERBUNDEN') connstate = 'ESTABLISHED';
                if (connstate) {
                  result.push({
                    protocol: line[0],
                    localaddress: localip,
                    localport: localport,
                    peeraddress: peerip,
                    peerport: peerport,
                    state: connstate
                  })
                }
              }
            });
            if (callback) {
              callback(result)
            }
            resolve(result);
          } else {
            cmd = "ss -tuna | grep 'ESTAB\\|SYN-SENT\\|SYN-RECV\\|FIN-WAIT1\\|FIN-WAIT2\\|TIME-WAIT\\|CLOSE\\|CLOSE-WAIT\\|LAST-ACK\\|LISTEN\\|CLOSING'";
            exec(cmd, function (error, stdout) {

              if (!error) {
                let lines = stdout.toString().split('\n');
                lines.forEach(function (line) {
                  line = line.replace(/ +/g, " ").split(' ');
                  if (line.length >= 6) {
                    let localip = line[4];
                    let localport = '';
                    let localaddress = line[4].split(':');
                    if (localaddress.length > 1) {
                      localport = localaddress[localaddress.length - 1];
                      localaddress.pop();
                      localip = localaddress.join(':');
                    }
                    let peerip = line[5];
                    let peerport = '';
                    let peeraddress = line[5].split(':');
                    if (peeraddress.length > 1) {
                      peerport = peeraddress[peeraddress.length - 1];
                      peeraddress.pop();
                      peerip = peeraddress.join(':');
                    }
                    let connstate = line[1];
                    if (connstate == 'ESTAB') connstate = 'ESTABLISHED';
                    if (connstate == 'TIME-WAIT') connstate = 'TIME_WAIT';
                    if (connstate) {
                      result.push({
                        protocol: line[0],
                        localaddress: localip,
                        localport: localport,
                        peeraddress: peerip,
                        peerport: peerport,
                        state: connstate
                      })
                    }
                  }
                });
              }
              if (callback) {
                callback(result)
              }
              resolve(result);
            })
          }
        })
      }
      if (_darwin) {
        let cmd = "netstat -nat | grep 'ESTABLISHED\\|SYN_SENT\\|SYN_RECV\\|FIN_WAIT1\\|FIN_WAIT2\\|TIME_WAIT\\|CLOSE\\|CLOSE_WAIT\\|LAST_ACK\\|LISTEN\\|CLOSING\\|UNKNOWN'";
        exec(cmd, function (error, stdout) {
          if (!error) {

            let lines = stdout.toString().split('\n');

            lines.forEach(function (line) {
              line = line.replace(/ +/g, " ").split(' ');
              if (line.length >= 6) {
                let localip = line[3];
                let localport = '';
                let localaddress = line[3].split('.');
                if (localaddress.length > 1) {
                  localport = localaddress[localaddress.length - 1];
                  localaddress.pop();
                  localip = localaddress.join('.');
                }
                let peerip = line[4];
                let peerport = '';
                let peeraddress = line[4].split('.');
                if (peeraddress.length > 1) {
                  peerport = peeraddress[peeraddress.length - 1];
                  peeraddress.pop();
                  peerip = peeraddress.join('.');
                }
                let connstate = line[5];
                if (connstate) {
                  result.push({
                    protocol: line[0],
                    localaddress: localip,
                    localport: localport,
                    peeraddress: peerip,
                    peerport: peerport,
                    state: connstate
                  })
                }
              }
            });
            if (callback) {
              callback(result)
            }
            resolve(result);
          }
        })
      }
    });
  });
}

exports.networkConnections = networkConnections;
