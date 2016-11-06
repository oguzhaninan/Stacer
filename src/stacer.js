const {
    remote
} = require('electron')
const {
    BrowserWindow
} = remote
const os = require('os')
const {
    ipcRenderer
} = require('electron')
const libCpuUsage = require('cpu-usage')
const diskspace = require('diskspace')
const si = require('systeminformation')
const exec = require('exec')
const prog = require('./src/progressbar.min')
require('./src/fast-search');
require('./src/amaran.min')

var down = 0,
    up = 0
var downBar = new prog.Line("#down-bar", {
    strokeWidth: 5,
    easing: 'easeInOut',
    duration: 600,
    color: '#2bab51',
    trailColor: '#202b33',
    svgStyle: {
        width: '100%',
        height: '100%'
    },
    text: {
        style: {
            color: '#999',
            position: 'absolute',
            right: '0',
            top: '30px',
            padding: 0,
            margin: 0,
            transform: null
        },
        autoStyleContainer: false
    },
    step: (state, bar) => {
        bar.setText(down + 'kb/s')
    }
});

var upBar = new prog.Line("#up-bar", {
    strokeWidth: 5,
    easing: 'easeInOut',
    duration: 600,
    color: '#2bab51',
    trailColor: '#202b33',
    svgStyle: {
        width: '100%',
        height: '100%'
    },
    text: {
        style: {
            color: '#999',
            position: 'absolute',
            right: '0',
            top: '30px',
            padding: 0,
            margin: 0,
            transform: null
        },
        autoStyleContainer: false
    },
    step: (state, bar) => {
        bar.setText(up + 'kb/s')
    }
});

setInterval(() => {
    si.networkStats('enp3s0', (data) => {
        down = (data.rx_sec / 1024).toFixed(2)
        up = (data.tx_sec / 1024).toFixed(2)
        downBar.animate(down / 700)
        upBar.animate(up / 700)
    })
}, 1500);

var cpuBar = new prog.SemiCircle("#cpu-cont", {
    strokeWidth: 12,
    color: '#2bab51',
    trailColor: '#202b33',
    trailWidth: 12,
    easing: 'easeInOut',
    duration: 1000,
    svgStyle: null,
    text: {
        value: '',
        alignToBottom: true
    },
    from: {
        color: '#2bab51'
    },
    to: {
        color: '#2bab51'
    },
    step: (state, bar) => {
        bar.path.setAttribute('stroke', state.color);
        var value = Math.round(bar.value() * 100);
        if (value === 0)
            bar.setText('');
        else
            bar.setText(value + "%");

        bar.text.style.color = "#fff"
        bar.text.style.paddingBottom = "20px"
    }
});
cpuBar.text.style.fontSize = '16px';

var memInfo = ""
var memBar = new prog.SemiCircle("#mem-cont", {
    strokeWidth: 12,
    color: '#ff9939',
    trailColor: '#202b33',
    trailWidth: 12,
    easing: 'easeInOut',
    duration: 1000,
    svgStyle: null,
    text: {
        value: '',
        alignToBottom: true
    },
    from: {
        color: '#ff9939'
    },
    to: {
        color: '#ff9939'
    },
    step: (state, bar) => {
        bar.path.setAttribute('stroke', state.color);
        bar.setText(memInfo)
        bar.text.style.color = "#fff" //"#3d3a4b";
        bar.text.style.paddingBottom = "20px"
    }
});
memBar.text.style.fontSize = '16px';

var dTotal = "",
    dUsed = ""
var diskBar = new prog.SemiCircle("#disk-cont", {
    strokeWidth: 12,
    color: '#dc175d', //'#fb292c',
    trailColor: '#202b33',
    trailWidth: 12,
    easing: 'easeInOut',
    duration: 1000,
    svgStyle: null,
    text: {
        value: '',
        alignToBottom: true
    },
    from: {
        color: '#dc175d'
    }, //'#fb292c'},
    to: {
        color: '#dc175d'
    }, //'#af0305'},
    step: (state, bar) => {
        bar.path.setAttribute('stroke', state.color)
        bar.setText(dUsed + "/" + dTotal)
        bar.text.style.color = "#fff" //"#3d3a4b";
        bar.text.style.paddingBottom = "20px"
    }
});
diskBar.text.style.fontSize = '16px';

//Cpu
libCpuUsage(1000, (val) => {
    cpuBar.animate(val / 100);
});

//Memory
setInterval(() => {
    memInfo = ((os.totalmem() - os.freemem()) / 1073741824).toFixed(2) + "GB/" +
        (os.totalmem() / 1073741824).toFixed(2) + "GB"
    memBar.animate((os.totalmem() - os.freemem()) / os.totalmem())
}, 1000);

//Disk
function diskInfo() {
    diskspace.check('/', function(err, total, free, status) {
        dTotal = (total / 1073741824).toFixed(2) + "GB"
        dUsed = (free / 1073741824).toFixed(2) + "GB"
        diskBar.animate((total - free) / total);
    });
}
diskInfo()
setInterval(diskInfo, 10000)

//System info
si.osInfo((sys) => {
    $(".system-info ul").append("<li>Hostname: " + os.hostname() + "</li>")
    $(".system-info ul").append("<li>Platform: " + os.platform() + os.arch() + "</li>")
    $(".system-info ul").append("<li>Distribution: " + sys.distro + "</li>")
    $(".system-info ul").append("<li>Total RAM: " + (os.totalmem() / 1073741824).toFixed(2) + " GB</li>")
    $(".system-info ul").append("<li>CPU Model: " + os.cpus()[0].model + "</li>")
    $(".system-info ul").append("<li>CPU Cores: " + os.cpus().length + "</li>")
})

$(document).ready(function() {

		$("#sudoPassBtn").click(function () {
				if($("#sudoPass").val() != ""){
					sudoPass = $("#sudoPass").val()
					$("#sudoPop").remove()
				}
		})

    $(document).keypress(function(e) {
      if(e.which == 13) {
        sudoPass = $("#sudoPass").val()
        $("#sudoPop").remove()
      }
    });

    var isInstalling = false;
    setTimeout(function() {
        exec('dpkg --get-selections | cut -f1',
            function(err, stdout, code) {
                var list = stdout.split("\n")
                var listCount = list.length
                list.forEach((line) => {
                    if (line != "") {
                        $("#installed-packages-list").append(
                            '<li>' + line + '<a></a></li>');
                    }
                })
                $("#installed-packages-list li a").click(function() {
                    if (!isInstalling) {
                        isInstalling = true;
                        var _this = $(this)
                        _this.addClass("loader")
                        var appName = _this.parent("li").text()
                        $("#installed-packages-title span").text("System Installed Packages (" + --listCount + ")")
                        exec('echo ' + sudoPass + ' | sudo -S apt-get remove --purge -y ' + appName,
                            function(err, stdout, code) {
                                _this.parent("li").remove();
                                $.amaran({
                                    'message': appName + ' package uninstalled.',
                                    'position': 'top right'
                                });
                                isInstalling = false;
                            }
                        )
                    } else {
                        $.amaran({
                            'message': 'Another process continues.',
                            'position': 'top right'
                        });
                    }
                })
                $("#installed-packages-title span").text("System Installed Packages (" + listCount + ")")
                $('#packages-search').fastLiveFilter('#installed-packages-list');
            }
        )
    }, 2000);

    var isServ = false;
    setTimeout(function() {
        exec("service --status-all | tr -d [*] | tr -d ' '",
            function(err, stdout, code) {
                var list = stdout.split("\n")
                var listCount = list.length
                list.forEach((line) => {
                    if (line != "") {
                        var service = line.substring(1, line.length)
                        var isRun   = line.substring(0, 1) == "+" ? "checked" : ""
                        $("#system-service-list").append(
                            '<li>' + service +
                            '<input type="checkbox" class="switch" id="'+service+'" '+isRun+'/>'+
                            '<label for="'+service+'"></label></li>');
                    }
                })
                $("#system-service-list li .switch").change(function() {
                    if (!isServ) {
                        isServ = true;
                        var _this = $(this)
                        var task = _this.is(":checked") ? "start" : "stop"
                        var service = _this.parent("li").text()
                        exec('echo ' + sudoPass + ' | sudo -S service ' + service + ' ' + task,
                            function(err, stdout, code) {
                                isServ = false;
                                $.amaran({
                                    'message': service + ' service ' + task + (_this.is(":checked") ? 'ed' : 'ped'),
                                    'position': 'top right'
                                });
                            }
                        )
                    } else {
                        $.amaran({
                            'message': 'Another process continues.',
                            'position': 'top right'
                        });
                    }
                })
                $("#system-service-title span").text("System Services (" + listCount + ")")
                $('#system-service-search').fastLiveFilter('#system-service-list');
            }
        )
    }, 1000);

    exec('ls ' + os.homedir() + '/.config/autostart',
        function(err, stdout, code) {
            var appName = ''
            stdout.split("\n").forEach((line) => {
                if (line != "") {
                    exec('cat ' + os.homedir() + '/.config/autostart/' + line + ' | grep Name=',
                        function(err, stdout, code) {
                            appName = stdout.split("\n")[0].replace("Name=", "");
                            $("#startup-apps-list").append(
                                '<li>' + appName + '<a name="'+line+'"></a></li>');
                        }
                      )
                }
            })
            setTimeout(function () {
              $("#startup-apps-list li a").click(function() {
                    var _this = $(this)
                    var startupApp = _this.attr("name")
                    exec('echo ' + sudoPass + ' | sudo -S rm '+ os.homedir() + '/.config/autostart/' + startupApp,
                        function(err, stdout, code) {
                            _this.parent("li").remove()
                            $.amaran({
                                'message': appName + ' is deleted.',
                                'position': 'top right'
                            });
                        }
                    )
              })}, 3000)
        })

    function select_all_checkbox(allId, checkName) {
        $("#" + allId).change(function() {
            if ($("#" + allId).is(':checked'))
                $("input[name=" + checkName + "]").prop("checked", true)
            else
                $("input[name=" + checkName + "]").prop("checked", false)
        })
    }

    $("#nav > li > label").click(function() {
        var data = $(this).attr("id");
        $(".content").hide("fast");
        $('div[data-nav=' + data + ']').show("fast");

        $("#nav > li > label").removeClass("nav-active")
        $(this).addClass("nav-active");

    })

    $("#system-scan-btn").click(function() {
        //Table clear
        $("#cleaner-table > .tdl-content > ul").html("");
        //System apt cache searching...
        if ($("#apt-cache-choose").is(":checked")) {
            exec('ls /var/cache/apt/archives/*.deb',
                function(err, stdout, code) {

                    $("#cleaner-table > .tdl-content > ul").append(
                        '<li style="background-color: #293945;"><label><input type="checkbox" id="select-all-cache" checked><i></i>' +
                        '<span style="font-size:14px; color:#aeb5bf;">' +
                        'Select All (Apt Caches) </span></label></li>');

                    var i = 1;
                    stdout.split("\n").forEach((line) => {
                        if (line != "") {
                            $("#cleaner-table > .tdl-content > ul").append(
                                '<li><label><input type="checkbox" name="apt-cache-check" checked><i></i><span>' +
                                line.replace("/var/cache/apt/archives/", "") +
                                '</span></label></li>');

                            $("#apt-cache-choose").parent("LABEL").find("span").find("span").text("(" + (i++) + ")")
                        }
                    })
                    select_all_checkbox('select-all-cache', 'apt-cache-check')
                })
        }
        //System crash reports searching...
        if ($("#crash-rep-choose").is(":checked")) {
            exec('ls /var/crash',
                function(err, stdout, code) {

                    $("#cleaner-table > .tdl-content > ul").append(
                        '<li style="background-color: #293945;"><label><input type="checkbox" id="select-all-crash" checked><i></i>' +
                        '<span style="font-size:14px; color:#aeb5bf;">' +
                        'Select All (Crash Reports) </span></label></li>');

                    var i = 1;
                    stdout.split("\n").forEach((line) => {
                        if (line != "") {
                            $("#cleaner-table > .tdl-content > ul").append(
                                '<li><label><input type="checkbox" name="crash-check" checked><i></i><span>' +
                                line +
                                '</span></label></li>');

                            $("#crash-rep-choose").parent("LABEL").find("span").find("span").text("(" + (i++) + ")")
                        }
                    })
                    select_all_checkbox('select-all-crash', 'crash-check')
                })
        }
        //System logs searching...
        if ($("#sys-logs-choose").is(":checked")) {
            exec('ls /var/log/*.*',
                function(err, stdout, code) {
                    $("#cleaner-table > .tdl-content > ul").append(
                        '<li style="background-color: #293945;"><label><input type="checkbox" id="select-all-logs" checked><i></i>' +
                        '<span style="font-size:14px; color:#aeb5bf;">' +
                        'Select All (System Logs) </span></label></li>');

                    var i = 1;
                    stdout.split("\n").forEach((line) => {
                        if (line != "") {
                            $("#cleaner-table > .tdl-content > ul").append(
                                '<li><label><input type="checkbox" name="log-check" checked><i></i><span>' +
                                line.replace("/var/log/", "") +
                                '</span></label></li>');

                            $("#sys-logs-choose").parent("LABEL").find("span").find("span").text("(" + (i++) + ")")
                        }
                    })
                    select_all_checkbox('select-all-logs', 'log-check')
                })
        }
        //System app cache searching...
        if ($("#app-cache-choose").is(":checked")) {
            exec('ls ' + os.homedir() + '/.cache',
                function(err, stdout, code) {

                    $("#cleaner-table > .tdl-content > ul").append(
                        '<li style="background-color: #293945;"><label><input type="checkbox" id="select-all-app-cache" checked><i></i>' +
                        '<span style="font-size:14px; color:#aeb5bf;">' +
                        'Select All (App Caches) </span></label></li>');

                    var i = 1;
                    stdout.split("\n").forEach((line) => {
                        if (line != "") {
                            $("#cleaner-table > .tdl-content > ul").append(
                                '<li><label><input type="checkbox" name="app-cache-check" checked><i></i><span>' +
                                line +
                                '</span></label></li>');

                            $("#app-cache-choose").parent("LABEL").find("span").find("span").text("(" + (i++) + ")")
                        }
                    })
                    select_all_checkbox('select-all-app-cache', 'app-cache-check')
                })
        }

    })

    $("#clean-btn").click(function() {

        var appCheckeds = $(".tdl-content input[name=app-cache-check]:checked");
        var apLen = $(".tdl-content input[name=app-cache-check]").length;
        appCheckeds.each(function(c, check) {
            var appCacheFile = $(this).parent("LABEL").find("SPAN").text()
            exec('echo ' + sudoPass + ' | sudo -S rm -r ' + os.homedir() + '/.cache/' + appCacheFile,
                function(err, stdout, code) {
                    $(check).parent("LABEL").parent("li").remove();
                    $("#app-cache-choose").parent("LABEL").find("span").find("span").text("(" + (--apLen) + ")")
                })
        });

        var aptCheckeds = $(".tdl-content input[name=apt-cache-check]:checked");
        var aLen = $(".tdl-content input[name=apt-cache-check]").length;
        aptCheckeds.each(function(c, check) {
            var cacheFile = $(this).parent("LABEL").find("SPAN").text()
            exec('echo ' + sudoPass + ' | sudo -S rm /var/cache/apt/archives/' + cacheFile,
                function(err, stdout, code) {
                    $(check).parent("LABEL").parent("li").remove();
                    $("#apt-cache-choose").parent("LABEL").find("span").find("span").text("(" + (--aLen) + ")")
                })
        });

        var logCheckeds = $(".tdl-content input[name=log-check]:checked");
        var lLen = $(".tdl-content input[name=log-check]").length;
        logCheckeds.each(function(c, check) {
            var logFile = $(this).parent("LABEL").find("SPAN").text()
            exec('echo ' + sudoPass + ' | sudo -S rm /var/log/' + logFile,
                function(err, stdout, code) {
                    $(check).parent("LABEL").parent("li").remove();
                    $("#sys-logs-choose").parent("LABEL").find("span").find("span").text("(" + (--lLen) + ")")
                })
        });

        var crashCheckeds = $(".tdl-content input[name=crash-check]:checked");
        var cLen = $(".tdl-content input[name=crash-check]").length;
        crashCheckeds.each(function(c, check) {
            var crashFile = $(this).parent("LABEL").find("SPAN").text()
            exec('echo ' + sudoPass + ' | sudo -S rm /var/crash/' + crashFile,
                function(err, stdout, code) {
                    $(check).parent("LABEL").parent("li").remove();
                    $("#crash-rep-choose").parent("LABEL").find("span").find("span").text("(" + (--cLen) + ")")
                })
        });
    });


    //Close Btn
    $("#close-btn").click(function() {
        ipcRenderer.send('close-app')
    })

    //Minimize Btn
    $("#min-btn").click(function() {
        BrowserWindow.getFocusedWindow().minimize()
    })

    $("#cpu-cont svg, #mem-cont svg, #disk-cont svg").css("margin-top", "10px");

})





//
