import {
    homedir
} from 'os'
import {
    spawnSync
} from 'child_process'

export default () => {
    // Set app configs
    localStorage.crashReportsPath = "/var/crash/"
    localStorage.systemLogsPath = "/var/log/"
    localStorage.appCachePath = homedir() + "/.cache/"
    localStorage.autostartApps = homedir() + "/.config/autostart/"
    localStorage.getAllService = "systemctl list-unit-files --state=enabled,disabled --type=service | grep .service | cut -d ' ' -f1 | sed -e 's/.service//g' 2> /dev/null"
    localStorage.trashPath = homedir() + "/.local/share/Trash/files"
    localStorage.trashInfoPath = homedir() + "/.local/share/Trash/info"

    // Package management settings
    var pm = localStorage.packageManagement

    if (!pm) {
        var dpkg = spawnSync('dpkg', ['--version'])
        var rpm = spawnSync('rpm', ['--version'])
        var pacman = spawnSync('pacman', ['--version'])

        if (!dpkg.error)
            localStorage.packageManagement = 'dpkg'
        else if (!rpm.error)
            localStorage.packageManagement = 'rpm'
        else if (!pacman.error)
            localStorage.packageManagement = 'pacman'
        else
            localStorage.packageManagement = 'dpkg'

        switch (localStorage.packageManagement) {
            case 'dpkg':
                {
                    localStorage.pkgCachePath = "/var/cache/apt/archives/"
                    localStorage.getInstalledPackages = "dpkg --get-selections | grep -v deinstall | cut -f 1 2> /dev/null"
                    localStorage.removePackage = "dpkg -r "
                }
                break;
            case 'rpm':
                {
                    localStorage.pkgCachePath = ""
                    localStorage.getInstalledPackages = "rpm -qa 2> /dev/null"
                    localStorage.removePackage = "rpm --erase "
                }
                break;
            case 'pacman':
                {
                    localStorage.pkgCachePath = "/var/cache/pacman/pkg/"
                    localStorage.getInstalledPackages = "pacman -Q | cut -d ' ' -f 1 2> /dev/null"
                    localStorage.removePackage = "pacman -R --noconfirm "
                }
                break;
        }
    }
}