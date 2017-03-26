import {
  homedir
} from 'os'

exports.commands = {
  aptCachePath: "/var/cache/apt/archives/",
  crashReportsPath: "/var/crash/",
  systemLogsPath: "/var/log/",
  appCachePath: homedir() + "/.cache/",
  autostartApps: homedir() + "/.config/autostart/",
  getInstalledPackages: "dpkg --get-selections | cut -f1",
  getAllService: "service --status-all | tr -d [*] | tr -d ' '",
  removePackage: "apt-get remove -y ",
  trashPath: '/home/oguzhan/.local/share/Trash/files',
  trashInfoPath: '/home/oguzhan/.local/share/Trash/info'
}

exports.prop = {
  // Durations
  networkBarsDuration: 600,
  upBarDuration: 600,
  // Colors
  networkBarColor: '#2196F3',
}