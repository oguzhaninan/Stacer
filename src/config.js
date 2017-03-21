
import { homedir } from 'os'

exports.commands = {
  aptCachePath:         "/var/cache/apt/archives/",
  crashReportsPath :    "/var/crash/",
  systemLogsPath:       "/var/log/",
  appCachePath:          homedir() + "/.cache/",
  autostartApps:         homedir() + "/.config/autostart/",
  getInstalledPackages: "dpkg --get-selections | cut -f1",
  getAllService:        "service --status-all | tr -d [*] | tr -d ' '",
  removePackage:        "apt-get remove --purge -y "
}

exports.prop = {
   // Durations
   networkBarsDuration: 600,
   upBarDuration: 600,
   cpuDuration: 1200,
   memDuration: 1500,
   diskDuration: 5000,
   // Colors
   networkBarColor: '#2196F3',
   cpuBarColor:  '#4CAF50',
   memBarColor:  '#F44336',
   diskBarColor: '#E91E63',
   trailColor:   '#202b33'
}
