import {
  spawnSync
} from 'child_process'
import {
  formatBytes
} from '../../utils/helpers'
import fs from 'fs'

export default (search = '', sort = []) => {
  search = search.toLowerCase()

  let ps = spawnSync('ps', ['ax', '-o', 'pid,rss,pmem,vsize,uname,pcpu,cmd', ...sort, '--no-headings'])

  if (ps.stderr.toString())
    logger.error('List Processes', ps.stderr.toString())

  if ( search )
    return ps.stdout.toString().split('\n')
      .filter(p => p.toLowerCase().indexOf(search) !== -1)
      .map(p => procParse(p.trim())).slice(0, -1)
  else
    return ps.stdout.toString().split('\n')
      .map(p => procParse(p.trim())).slice(0, -1)
}

function procParse(data) {
  let columns = data.split(/\s+/)

  return {
    pid: columns[0],
    rss: formatBytes(columns[1] * 1024),
    pmem: columns[2],
    vsize: formatBytes(columns[3] * 1024),
    uname: columns[4],
    pcpu: columns[5],
    cmd: columns.slice(6).join(' ')
  }
}