import {
  homedir
} from 'os'
import { spawnSync } from 'child_process'

let uname = spawnSync('uname', ['-a']).stdout.toString().toLowerCase()
var systemOs = uname.indexOf('ubuntu') !== -1 ? 'ubuntu' : 'arch'

let conf = {}

conf.crashReportsPath = "/var/crash/"
conf.systemLogsPath = "/var/log/"
conf.appCachePath = homedir() + "/.cache/"
conf.autostartApps = homedir() + "/.config/autostart/"
conf.getAllService = "systemctl list-unit-files --state=enabled,disabled --type=service | grep .service | cut -d ' ' -f1 | sed -e 's/.service//g'"
conf.trashPath = homedir() + "/.local/share/Trash/files",
conf.trashInfoPath = homedir() + "/.local/share/Trash/info"

switch (systemOs) {
  case 'ubuntu': {
    conf.pkgCachePath = "/var/cache/apt/archives/"
    conf.getInstalledPackages = "dpkg --get-selections | cut -f 1"
    conf.removePackage = "apt-get remove -y "
  }    
  break;
  case 'arch': {
    conf.pkgCachePath = "/var/cache/pacman/pkg/"
    conf.getInstalledPackages = "pacman -Q | cut -d ' ' -f 1"
    conf.removePackage = "pacman -R --noconfirm "
  }
  break;
}

exports.commands = conf