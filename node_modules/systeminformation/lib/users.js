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
// 11. Users/Sessions
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
// array of users online = sessions

function parseUsers1(lines) {
  let result = [];
  let result_who = [];
  let result_w = {};
  let w_first = true;
  let w_header = [];
  let w_pos = [];
  let w_headerline = '';
  let who_line = {};

  let is_whopart = true;
  lines.forEach(function (line) {
    if (line == '---') {
      is_whopart = false;
    } else {
      let l = line.replace(/ +/g, " ").split(' ');

      // who part
      if (is_whopart) {
        result_who.push({
          user: l[0],
          tty: l[1],
          date: l[2],
          time: l[3],
          ip: (l && l.length > 4) ? l[4].replace(/\(/g, "").replace(/\)/g, "") : ''
        })
      } else {
        // w part
        if (w_first) {    // header
          w_header = l;
          w_headerline = line;
          w_header.forEach(function (item) {
            w_pos.push(line.indexOf(item))
          });
          w_first = false;
        } else {
          // split by w_pos
          result_w.user = line.substring(w_pos[0], w_pos[1] - 1).trim();
          result_w.tty = line.substring(w_pos[1], w_pos[2] - 1).trim();
          result_w.ip = line.substring(w_pos[2], w_pos[3] - 1).replace(/\(/g, "").replace(/\)/g, "").trim();
          result_w.command = line.substring(w_pos[7], 1000).trim();
          // find corresponding 'who' line
          who_line = result_who.filter(function (obj) {
            return (obj.user.substring(0, 8).trim() == result_w.user && obj.tty == result_w.tty)
          });
          if (who_line.length == 1) {
            result.push({
              user: who_line[0].user,
              tty: who_line[0].tty,
              date: who_line[0].date,
              time: who_line[0].time,
              ip: who_line[0].ip,
              command: result_w.command
            })
          }
        }
      }
    }
  });
  return result;
}

function parseUsers2(lines) {
  let result = [];
  let result_who = [];
  let result_w = {};
  let who_line = {};

  let is_whopart = true;
  lines.forEach(function (line) {
    if (line == '---') {
      is_whopart = false;
    } else {
      let l = line.replace(/ +/g, " ").split(' ');

      // who part
      if (is_whopart) {
        result_who.push({
          user: l[0],
          tty: l[1],
          date: ("" + new Date().getFullYear()) + '-' + ("0" + ("JANFEBMARAPRMAYJUNJULAUGSEPOCTNOVDEC".indexOf(l[2].toUpperCase()) / 3 + 1)).slice(-2) + '-' + ("0" + l[3]).slice(-2),
          time: l[4],
        })
      } else {
        // w part
        // split by w_pos
        result_w.user = l[0];
        result_w.tty = l[1];
        result_w.ip = (l[2] != '-') ? l[2] : '';
        result_w.command = l.slice(5, 1000).join(' ');
        // find corresponding 'who' line
        who_line = result_who.filter(function (obj) {
          return (obj.user == result_w.user && (obj.tty.substring(3, 1000) == result_w.tty || obj.tty == result_w.tty))
        });
        if (who_line.length == 1) {
          result.push({
            user: who_line[0].user,
            tty: who_line[0].tty,
            date: who_line[0].date,
            time: who_line[0].time,
            ip: result_w.ip,
            command: result_w.command
          })
        }
      }
    }
  });
  return result;
}

function users(callback) {

  return new Promise((resolve, reject) => {
    process.nextTick(() => {
      if (_windows) {
        let error = new Error(NOT_SUPPORTED);
        if (callback) { callback(NOT_SUPPORTED) }
        reject(error);
      }

      let result = [];

      // linux
      if (_linux) {
        exec("who --ips; echo '---'; w | tail -n +2", function (error, stdout) {
          if (!error) {
            // lines / split
            let lines = stdout.toString().split('\n');
            result = parseUsers1(lines);
            if (result.length == 0) {
              exec("who; echo '---'; w | tail -n +2", function (error, stdout) {
                if (!error) {
                  // lines / split
                  lines = stdout.toString().split('\n');
                  result = parseUsers1(lines);
                  if (callback) { callback(result) }
                  resolve(result);
                } else {
                  if (callback) { callback(result) }
                  resolve(result);
                }
              });
            } else {
              if (callback) { callback(result) }
              resolve(result);
            }
          } else {
            if (callback) { callback(result) }
            resolve(result);
          }
        });
      }

      if (_darwin) {
        exec("who; echo '---'; w -ih", function (error, stdout) {
          if (!error) {
            // lines / split
            let lines = stdout.toString().split('\n');
            result = parseUsers2(lines);

            if (callback) { callback(result) }
            resolve(result);
          } else {
            if (callback) { callback(result) }
            resolve(result);
          }
        });
      }

    });
  });
}

exports.users = users;
