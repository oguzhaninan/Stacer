var fs = require('fs')

var glob = require("glob")

export default () => {
  let processesList = []
  glob('/proc/[0-9]*', (er, procs) => {
    // get all process ids
    procs.forEach(pid => {
      fs.readFile(`${pid}/stat`, 'utf-8', (err, data) => {
        processesList.push(procStat(data));
      })
    })
  })

  return processesList
}

function procStat(data) {
  var columns = data.split(' ')

  return {
    pid: columns[0],
    comm: columns[1],
    state: columns[2],
    ppid: columns[3],
    pgrp: columns[4],
    session: columns[5]
  }
}