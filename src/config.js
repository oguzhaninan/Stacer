exports.commands =
{
  aptCacheScanning:      "/var/cache/apt/archives/",
  crashReportsScanning : "/var/crash/",
  systemLogsScanning:    "/var/log/",
  appCacheScanning:       os.homedir() + "/.cache/",
  autostartApps:          os.homedir() + "/.config/autostart/",
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
   diskDuration: 10000,
   // Colors
   netwrokBarColor: '#2bab51',
   cpuBarColor:  '#2bab51',
   memBarColor:  '#ff9939',
   diskBarColor: '#dc175d',
   trailColor:   '#202b33'
}