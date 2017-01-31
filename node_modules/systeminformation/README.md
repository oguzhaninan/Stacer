# systeminformation

Simple system and OS information library for [node.js][nodejs-url]

  [![NPM Version][npm-image]][npm-url]
  [![NPM Downloads][downloads-image]][downloads-url]
  [![Git Issues][issues-img]][issues-url]
  [![deps status][daviddm-img]][daviddm-url]
  [![MIT license][license-img]][license-url]

## Quick Start

Collection of 30+ functions to retrieve detailed hardware, system and OS information (linux and OSX only)

### Installation

```bash
$ npm install systeminformation --save
```

### Usage

All functions (except `version` and `time`) are implemented as asynchronous functions. Here a small example how to use them:

```
var si = require('systeminformation');

// callback style
si.cpu(function(data) {
	console.log('CPU-Information:');
	console.log(data);
})

// promises style - new in version 3
si.cpu()
	.then(data => console.log(data))
	.catch(error => console.error(error));

```

## News and Changes

### Latest Activity

- Version 3.13.0: added shell (returns standard shell)
- Version 3.12.0: refactoring and extended currentLoad (better OSX coverage and added irq load).
- Version 3.11.0: blockDevices now also for OSX and also extended (+ label, model, serial, protocol).
- Version 3.10.0: added blockDevices (list of disks, partitions, raids and roms).
- Version 3.9.0: extended networkInterfaces (added MAC address).
- Version 3.8.0: added dockerContainerProcesses (array of processes inside a docker container).
- Version 3.7.0: extended docker stats.
- Version 3.6.0: added versions (kernel, ssl, node, npm, pm2, ...).
- Version 3.5.0: added graphics info (controller and display).
- Version 3.4.0: rewritten currentLoad and CPU load for processes (linux). This is now much more accurate.
- Version 3.3.0: added process list. Get full process list including details like cpu and mem usage, status, command, ...
- Version 3.2.0: added battery support. If a battery is installed, you get information about status and current capacity level
- Version 3.1.0: added [Docker][docker-url] support. Now you can scan your docker containers and get their stats
- Version 3.0.0: added DisksIO - overall diskIO and IOPS values for all mounted volumes

### Changelog

You can find all changes here: [detailed changelog][changelog-url]

## Core concept

[Node.js][nodejs-url] comes with some basic OS information, but I always wanted a little more. So I came up to write this
little library. This library is still work in progress. Version 3 comes with further improvements. First it
requires now node.js version 4.0 and above. Another big change is, that all functions now return promises. You can use them
like before with callbacks OR with promises (see example in this documentation). I am sure, there is for sure room for improvement.
I was only able to test it on several Debian, Raspbian, Ubuntu distributions as well as OS X (Mavericks, Yosemite, El Captain).
Since version 2 nearly all functionality is available on OS X/Darwin platforms. But be careful, this library will definitely NOT work on Windows platforms!

If you have comments, suggestions & reports, please feel free to contact me!

I also created a little command line tool called [mmon][mmon-github-url]  (micro-monitor), also available via [github][mmon-github-url] and [npm][mmon-npm-url]


## Reference

### Sections

This library is splitted in several sections:

1. General
2. System (HW)
3. Operating System
4. CPU
5. Memory
6. File System
7. Network
8. Processes
9. Users
10. Internet
11. Docker
12. GetAll

### Function Reference and OS Support

| Function        | Linux           | OS X  | Comments |
| -------------- | ------ | ------ | ------- |
| si.version() | X | X | library version (no callback/promise) |
| si.time() | X | X | time information (no callback/promise) |
| - current | X | X | local time |
| - uptime | X | X | uptime |
| si.system(cb) | X | X | hardware information |
| - manufacturer | X | X | e.g. 'MSI' |
| - model | X | X | model/product e.g. 'MS-7823' |
| - version | X | X | version e.g. '1.0' |
| - serial | X | X | serial number |
| - uuid | X | X | UUID |
| si.osInfo(cb) | X | X | OS information |
| - platform   | X | X | 'Linux' or 'Darwin' |
| - distro | X | X |  |
| - release | X | X |  |
| - codename | | X |  |
| - kernel | X | X | kernel release - same as os.release() |
| - arch | X | X | same as os.arch() |
| - hostname | X | X | same as os.hostname() |
| - logofile | X | X | e.g. 'apple', 'debian', 'fedora', ... |
| si.versions(cb) | X | X | Version information (kernel, ssl, node, ...) |
| si.shell(cb) | X | X | standard shell |
| si.cpu(cb) | X | X | CPU information|
| - manufacturer | X | X | e.g. 'Intel(R)' |
| - brand | X | X | e.g. 'Core(TM)2 Duo' |
| - speed | X | X | in GHz e.g. '3.40' |
| - cores | X | X | # cores |
| si.cpuCurrentspeed(cb) | X | X | current CPU speed (in GHz)|
| - avg | X | X | avg CPU speed (all cores) |
| - min | X | X | min CPU speed (all cores) |
| - max | X | X | max CPU speed (all cores) |
| si.cpuTemperature(cb) | X | | CPU temperature (if sensors is installed) |
| - main | X | | main temperature |
| - cores | X | | array of temperatures |
| - max | X | | max temperature |
| si.mem(cb) | X | X | Memory information|
| - total | X | X | total memory |
| - free | X | X | not used |
| - used | X | X | used (incl. buffers/cache) |
| - active | X | X | used actively (excl. buffers/cache)  |
| - buffcache | X | X | used by buffers+cache |
| - available | X | X | potentially available (total - active) |
| - swaptotal | X | X |  |
| - swapused | X | X |  |
| - swapfree | X | X |  |
| si.battery(cb) | X | X | battery information |
| - hasbattery | X | X | indicates presence of battery |
| - ischarging | X | X | indicates if battery is charging |
| - maxcapacity | X | X | max capacity of battery |
| - currentcapacity | X | X | current capacity of battery |
| - percent | X | X | charging level in percent |
| si.graphics(cb) | X | X | arrays of graphics controllers and displays |
| - controllers[0].model | X | X | graphics controller model |
| - controllers[0].vendor | X | X | e.g. ATI |
| - controllers[0].bus | X | X | on which bus (e.g. PCIe) |
| - controllers[0].vram | X | X | VRAM size (in MB) |
| - controllers[0].vramDynamic | X | X | true if dynamicly allocated ram |
| - displays[0].model | X | X | Monitor/Display Model |
| - displays[0].main | X | X | true if main monitor |
| - displays[0].builtin | X | X | true if built in monitor |
| - displays[0].connection | X | X | e.g. DisplayPort or HDMI |
| - displays[0].resolutionx | X | X | pixel horizontal |
| - displays[0].resolutiony | X | X | pixel vertical |
| - displays[0].depth | X | X | color depth in bits |
| - displays[0].sizex | X | X | size in mm horizontal |
| - displays[0].sizey | X | X | size in mm vertical |
| si.fsSize(cb) | X | X | returns array of mounted file systems |
| - [0].fs | X | X | name of file system |
| - [0].type | X | X | type of file system |
| - [0].size | X | X | sizes in Bytes |
| - [0].used | X | X | used in Bytes |
| - [0].use | X | X | used in % |
| - [0].mount | X | X | mount point |
| si.blockDevices(cb) | X | X | returns array of disks, partitions,<br>raids and roms |
| - [0].name | X | X | name |
| - [0].type | X | X | type |
| - [0].fstype | X | X | file system type (e.g. ext4) |
| - [0].mount | X | X | mount point |
| - [0].size | X | X | size in bytes |
| - [0].physical | X | X | physical type (HDD, SSD, CD/DVD) |
| - [0].uuid | X | X | UUID |
| - [0].label | X | X | label |
| - [0].model | X | X | model |
| - [0].serial | X |   | serial |
| - [0].protocol | X | X | protocol (SATA, PCI-Express, ...) |
| si.fsStats(cb) | X | X | current transfer stats |
| - rx | X | X | bytes read since startup |
| - wx | X | X | bytes written since startup |
| - tx | X | X | total bytes read + written since startup |
| - rx_sec | X | X | bytes read / second (* see notes) |
| - wx_sec | X | X | bytes written / second (* see notes) |
| - tx_sec | X | X | total bytes reads + written / second  |
| - ms | X | X | interval length (for per second values) |
| si.disksIO(cb) | X | X | current transfer stats |
| - rIO | X | X | read IOs on all mounted drives |
| - wIO | X | X | write IOs on all mounted drives |
| - tIO | X | X | write IOs on all mounted drives |
| - rIO_sec | X | X | read IO per sec (* see notes) |
| - wIO_sec | X | X | write IO per sec (* see notes) |
| - tIO_sec | X | X | total IO per sec (* see notes) |
| - ms | X | X | interval length (for per second values) |
| si.networkInterfaces(cb) | X | X | array of network interfaces |
| - [0].iface | X | X | interface name |
| - [0].ip4 | X | X | ip4 address |
| - [0].ip6 | X | X | ip6 address |
| - [0].mac | X | X | MAC address |
| - [0].internal | X | X | true if internal interface |
| si.networkInterfaceDefault(cb) | X | X | get name of default network interface |
| si.networkStats(iface,cb) | X | X | current network stats of given interface<br>iface parameter is optional<br>defaults to first external network interface|
| - iface | X | X | interface |
| - operstate | X | X | up / down |
| - rx | X | X | received bytes overall |
| - tx | X | X | transferred bytes overall|
| - rx_sec | X | X | received bytes / second (* see notes) |
| - tx_sec | X | X | transferred bytes per second (* see notes) |
| - ms | X | X | interval length (for per second values) |
| si.networkConnections(cb) | X | X | current network network connections<br>returns an array of all connections|
| - [0].protocol | X | X | tcp or udp |
| - [0].localaddress | X | X | local address |
| - [0].localport | X | X | local port |
| - [0].peeraddress | X | X | peer address |
| - [0].peerport | X | X | peer port |
| - [0].state | X | X | like ESTABLISHED, TIME_WAIT, ... |
| si.currentLoad(cb) | X | X | CPU-Load |
| - avgload | X | X | average load  |
| - currentload | X | X | CPU-Load in % |
| - currentload_user | X | X | CPU-Load User in % |
| - currentload_nice | X | X | CPU-Load Nice in % |
| - currentload_system | X | X | CPU-Load System in % |
| - currentload_irq | X | X | CPU-Load System in % |
| si.fullLoad(cb) | X | X | CPU-full load since bootup in % |
| si.services('mysql, apache2', cb) | X | X | pass comma separated string of services |
| - [0].name | X | X | name of service |
| - [0].running | X | X | true / false |
| - [0].pcpu | X | X | process % CPU |
| - [0].pmem | X | X | process % MEM |
| si.processes(cb) | X | X | # running processes |
| - all | X | X | # of all processes |
| - running | X | X | # of all processes running |
| - blocked | X | X | # of all processes blocked |
| - sleeping | X | X | # of all processes sleeping |
| - list | X | X | # list of all processes incl. details |
| si.processLoad('apache2',cb) | X | X | detailed information about given process |
| - proc | X | X | process name |
| - pid | X | X | PID |
| - cpu | X | X | process % CPU |
| - mem | X | X | process % MEM |
| si.users(cb) | X | X | array of users online |
| - [0].user | X | X | user name |
| - [0].tty | X | X | terminal |
| - [0].date | X | X | login date |
| - [0].time | X | X | login time |
| - [0].ip | X | X | ip address (remote login) |
| - [0].command | X | X | last command or shell |
| si.inetChecksite(url, cb) | X | X | response-time (ms) to fetch given URL |
| - url | X | X | given url |
| - ok | X | X | status code OK (2xx, 3xx) |
| - status | X | X | status code |
| - ms | X | X | response time in ms |
| si.inetLatency(host, cb) | X | X | response-time (ms) to external resource<br>host parameter is optional (default 8.8.8.8)|
| si.dockerContainers(all, cb) | X | X | returns array of active/all docker containers |
| - [0].id | X | X | ID of container |
| - [0].name | X | X | name of container |
| - [0].image | X | X | name of image |
| - [0].imageID | X | X | ID of image |
| - [0].command | X | X | command |
| - [0].created | X | X | creation time |
| - [0].state | X | X | created, running, exited |
| - [0].ports | X | X | array of ports |
| - [0].mounts | X | X | array of mounts |
| si.dockerContainerStats(id, cb) | X | X | statistics for a specific container |
| - id | X | X | Container ID |
| - mem_usage | X | X | memory usage in bytes |
| - mem_limit | X | X | memory limit (max mem) in bytes |
| - mem_percent | X | X | memory usage in percent |
| - cpu_percent | X | X | cpu usage in percent |
| - pids | X | X | number of processes |
| - netIO.rx | X | X | received bytes via network |
| - netIO.wx | X | X | sent bytes via network |
| - blockIO.r | X | X | bytes read from BlockIO |
| - blockIO.w | X | X | bytes written to BlockIO |
| - cpu_stats | X | X | detailed cpu stats |
| - percpu_stats | X | X | detailed per cpu stats |
| - memory_stats | X | X | detailed memory stats |
| - networks | X | X | detailed network stats per interface |
| si.dockerContainerProcesses(id, cb) | X | X | array of processes inside a container |
| - [0].pid_host | X | X | process ID (host) |
| - [0].ppid | X | X | parent process ID |
| - [0].pgid | X | X | process group ID |
| - [0].user | X | X | effective user name |
| - [0].ruser | X | X | real user name |
| - [0].group | X | X | effective group name |
| - [0].rgroup | X | X | real group name |
| - [0].stat | X | X | process state |
| - [0].time | X | X | accumulated CPU time |
| - [0].elapsed | X | X | elapsed running time |
| - [0].nice | X | X |  nice value |
| - [0].rss | X | X | resident set size |
| - [0].vsz | X | X | virtual size in Kbytes |
| - [0].command | X | X | command and arguments |
| si.dockerAll(cb) | X | X | list of all containers including their stats<br>and processes in one single array |
| si.getStaticData(cb)  | X | X | all static data at once |
| si.getDynamicData(srv,iface,cb) | X | X | all dynamic data at once |
| si.getAllData(srv,iface,cb) | X | X | all data at once |

### cb: Asynchronous Function Calls (callback)

Remember: all functions (except `version` and `time`) are implemented as asynchronous functions! There are now two ways to consume them:

**Callback Style**

```
var si = require('systeminformation');

si.networkStats('eth1', function(data) {
	console.log('Network Interface Stats (eth1):');
	console.log('- is up: ' + data.operstate);
	console.log('- RX bytes overall: ' + data.rx);
	console.log('- TX bytes overall: ' + data.tx);
	console.log('- RX bytes/sec: ' + data.rx_sec);
	console.log('- TX bytes/sec: ' + data.tx_sec);
})
```

### Promises

**Promises Style** is new in version 3.0.

When omitting callback parameter (cb), then you can use all function in a promise oriented way. All functions (exept of `version` and `time`) are returning a promis, that you can consume:

```
si.networkStats('eth1')
	.then(data => {
		console.log('Network Interface Stats (eth1):');
		console.log('- is up: ' + data.operstate);
		console.log('- RX bytes overall: ' + data.rx);
		console.log('- TX bytes overall: ' + data.tx);
		console.log('- RX bytes/sec: ' + data.rx_sec);
		console.log('- TX bytes/sec: ' + data.tx_sec);
	})
	.catch(error => console.error(error));

```

### *: Additional Notes

In `fsStats`, `disksIO` and `networkStats` the results per second values (rx_sec, IOPS, ...) are calculated beginning
with the second call of the function. It is determined by calculating the difference of transferred bytes / IOs
divided by the time between two calls of the function.

## Known Issues

There is one major things, that I was still not able to solve:

For OS X, I did not find a reliable way to get the CPU temperature. All suggestions I found did not work on current version of OS X on different machines (intel platform). So if anyone has an idea, this would be helpful.

I am happy to discuss any comments and suggestions. Please feel free to contact me if you see any possibility of improvement!

## Comments

If you have ideas or comments, please do not hesitate to contact me.


Happy monitoring!

Sincerely,

Sebastian Hildebrandt, [+innovations](http://www.plus-innovations.com)

## Credits

Written by Sebastian Hildebrandt [sebhildebrandt](https://github.com/sebhildebrandt)

#### Contributers

- Guillaume Legrain [glegrain](https://github.com/glegrain)
- Riccardo Novaglia [richy24](https://github.com/richy24)
- Quentin Busuttil [Buzut](https://github.com/Buzut)
- lapsio [lapsio](https://github.com/lapsio)

## Copyright Information

Linux is a registered trademark of Linus Torvalds, OS X is a registered trademark of Apple Inc.,
Windows is a registered trademark of Microsoft Corporation. Node.js is a trademark of Joyent Inc.,
Intel is a trademark of Intel Corporation, Raspberry Pi is a trademark of the Raspberry Pi Foundation,
Debian is a trademark of the Debian Project, Ubuntu is a trademark of Canonical Ltd., Docker is a trademark of Docker, Inc.
All other trademarks are the property of their respective owners.

## License [![MIT license][license-img]][license-url]

>The [`MIT`][license-url] License (MIT)
>
>Copyright &copy; 2014-2016 Sebastian Hildebrandt, [+innovations](http://www.plus-innovations.com).
>
>Permission is hereby granted, free of charge, to any person obtaining a copy
>of this software and associated documentation files (the "Software"), to deal
>in the Software without restriction, including without limitation the rights
>to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
>copies of the Software, and to permit persons to whom the Software is
>furnished to do so, subject to the following conditions:
>
>The above copyright notice and this permission notice shall be included in
>all copies or substantial portions of the Software.
>
>THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
>IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
>FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
>AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
>LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
>OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
>THE SOFTWARE.
>
>Further details see [LICENSE](LICENSE) file.


[npm-image]: https://img.shields.io/npm/v/systeminformation.svg?style=flat-square
[npm-url]: https://npmjs.org/package/systeminformation
[downloads-image]: https://img.shields.io/npm/dm/systeminformation.svg?style=flat-square
[downloads-url]: https://npmjs.org/package/systeminformation

[license-url]: https://github.com/sebhildebrandt/systeminformation/blob/master/LICENSE
[license-img]: https://img.shields.io/badge/license-MIT-blue.svg?style=flat-square
[npmjs-license]: https://img.shields.io/npm/l/systeminformation.svg?style=flat-square
[changelog-url]: https://github.com/sebhildebrandt/systeminformation/blob/master/CHANGELOG.md

[nodejs-url]: https://nodejs.org/en/
[docker-url]: https://www.docker.com/

[daviddm-img]: https://img.shields.io/david/sebhildebrandt/systeminformation.svg?style=flat-square
[daviddm-url]: https://david-dm.org/sebhildebrandt/systeminformation

[issues-img]: https://img.shields.io/github/issues/sebhildebrandt/systeminformation.svg?style=flat-square
[issues-url]: https://github.com/sebhildebrandt/systeminformation/issues

[mmon-npm-url]: https://npmjs.org/package/mmon
[mmon-github-url]: https://github.com/sebhildebrandt/mmon
