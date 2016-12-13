const {app, ipcRenderer} = require('electron')

const os          = require('os')
const libCpuUsage = require('cpu-usage')
const diskspace   = require('diskspace')
const si          = require('systeminformation')
const exec        = require('exec')
const prog        = require('progressbar.js')

require('./libs/fast-search')
require('./libs/amaran.min')

/*
 * Properties
 */

var sudoPass = ""

var prop =
{
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

var commands =
{
  // Scan commands
  aptCacheScanning: "ls /var/cache/apt/archives/*.deb",
  crashReportsScanning : "ls /var/crash",
  systemLogsScanning: "ls /var/log/*.*",
  appCacheScanning: "ls " + os.homedir() + "/.cache",

  //
  getInstalledPackages: " dpkg --get-selections | cut -f1",
  getAllService: " service --status-all | tr -d [*] | tr -d ' '",
  removePackage: " apt-get remove --purge -y ",
  autostartApps: "ls " + os.homedir() + "/.config/autostart",
  removeAutostartApp: " rm "+ os.homedir() + "/.config/autostart/",
  removeAptCache: " rm /var/cache/apt/archives/",
  removeAppCache: " rm -r " + os.homedir() + "/.cache/",
  removeLogFile: " rm /var/log/",
  removeCrashFile: " rm /var/crash/"
}

var memInfo = ""
var dTotal  = ""
var dUsed   = ""

var down = 0
var up   = 0

/*
 * Functions
 */
function setCount( id  , count )
{
 $(id).parent("label").find("span").find("span").text("(" + count + ")")
}

function removeElement( element )
{
 $(element).parent("label").parent("li").remove()
}

function getFileName( element )
{
 return $(element).parent("label").find("span").text()
}

function getCheckeds( name )
{
 return $(".tdl-content input[name=" + name + "]:checked")
}

function getLength( name )
{
 return $(".tdl-content input[name=" + name + "]").length
}

function sudoCommand( command )
{
 return "echo " + sudoPass + " | sudo -S" + command
}

function selectAllCheckbox( allId , checkName )
{
   $("#" + allId).change(function() {
       if ($("#" + allId).is(':checked'))
           $("input[name=" + checkName + "]").prop("checked", true)
       else
           $("input[name=" + checkName + "]").prop("checked", false)
   })
}

function addListTitle( checkID , title )
{
  $("#cleaner-table > .tdl-content > ul")
  .append( '<li style="background-color: #293945;"><label>' +
           '<input type="checkbox" id="' + checkID + '"><i></i>' +
           '<span style="font-size:14px; color:#aeb5bf;">' +
           title + ' </span></label></li>' )
}

function addListElement( name , text )
{
  $("#cleaner-table > .tdl-content > ul")
  .append( '<li><label><input type="checkbox" name="' + name + '"><i></i><span>' +
          text + '</span></label></li>' )
}

function showMessage( message )
{
  $.amaran({
      'message': message,
      'position': 'top right'
  })
}

function addSystemClearAnimate( id )
{
  $( id ).parent("label").append( systemClearAnimate() ).find("i").hide()
  return  $( id ).parent("label")
}


function circleBar(id, clr, durat)
{
  var cBar = new prog.SemiCircle(id, {
    strokeWidth: 12,
    color: clr,
    trailColor: prop.trailColor,
    easing: 'easeInOut',
    duration: durat,
    svgStyle: {
      marginTop: '10px'
    },
    step: function(state, bar)
    {
      if(id == "#cpu-cont")
      {
        bar.setText(Math.round(bar.value() * 100) + "%")
      }
      else if(id == "#mem-cont")
      {
        bar.setText(memInfo)
      }
      else if(id == "#disk-cont")
      {
        bar.setText(dUsed + "/" + dTotal)
      }
    }
  });
  cBar.text.style.color = "#fff"
  cBar.text.style.paddingBottom = "20px"

  return cBar
}


//---- Page Functions -----//

function networkBars()
{

  /*
   * Get default network interface name
   */
  var defaultNetwork = "";
  si.networkInterfaceDefault(function ( name ) { defaultNetwork = name })

  /*
   * Network down speed
   */
  var downBar = new prog.Line("#down-bar", {
      strokeWidth: 5,
      easing: 'easeInOut',
      duration: prop.networkBarsDuration,
      color: prop.netwrokBarColor,
      trailColor: prop.trailColor,
      text: {
          style: {
              color: '#999',
              position: 'absolute',
              right: '0',
              marginTop: '5px'
          }
      },
      step: function(state, bar)
      {
          bar.setText(Math.abs(down).toString() + ' kb/s')
      }
  });

  /*
   * Network upload speed
   */
  var upBar = new prog.Line("#up-bar", {
      strokeWidth: 5,
      easing: 'easeInOut',
      duration: prop.networkBarsDuration,
      color: prop.netwrokBarColor,
      trailColor: prop.trailColor,
      text: {
          style: {
              color: '#999',
              position: 'absolute',
              right: '0',
              marginTop: '5px'
          }
      },
      step: function(state, bar)
      {
          bar.setText(Math.abs(up).toString() + ' kb/s')
      }
  });

  /*
   * Network down and upload speed setter
   */
  setInterval( function()
  {
      si.networkStats(defaultNetwork, function(data)
      {
          down = (data.rx_sec / 1024).toFixed(2)
          up   = (data.tx_sec / 1024).toFixed(2)
          downBar.animate(down / 1000)
          upBar.animate(up / 1000)
      })
  }, prop.networkBarsDuration);
}

function systemInformationBars()
{

  /*
   * Create the circle bars
   */
  var cpuBar  = circleBar("#cpu-cont", prop.cpuBarColor , prop.cpuDuration)

  var memBar  = circleBar("#mem-cont", prop.memBarColor , prop.memDuration)

  var diskBar = circleBar("#disk-cont",prop.diskBarColor , prop.diskDuration)

  /*
   * Cpu value setter
   */
  libCpuUsage(prop.cpuDuration, function(val) { cpuBar.animate(val / 100) });

  /*
   * Memory value setter
   */
  setInterval( function()
  {
    var totalMem = os.totalmem()
    var freeMem  = os.freemem()
    var usedMem  = totalMem - freeMem

    memInfo = (usedMem  / 1073741824).toFixed(2) + "GB/" +
              (totalMem / 1073741824).toFixed(2) + "GB"
    memBar.animate(usedMem / totalMem)
  }, prop.memDuration);

  /*
   * Disk value setter
   */
  function diskInfo()
  {
    diskspace.check('/', function(err, total, free, status)
    {
      var used = total-free
      dTotal = (total / 1000000000).toFixed(2) + "GB"
      dUsed  = (used  / 1000000000).toFixed(2) + "GB"
      diskBar.animate(used / total);
    });
  }
  diskInfo()
  setInterval(diskInfo, prop.diskDuration)


  //System info
  si.osInfo(function(sys)
  {
    $(".system-info ul").append($("<li>").append("Hostname: "     + os.hostname()))
    $(".system-info ul").append($("<li>").append("Platform: "     + os.platform() + os.arch()))
    $(".system-info ul").append($("<li>").append("Distribution: " + sys.distro ))
    $(".system-info ul").append($("<li>").append("Total RAM: "    + (os.totalmem() / 1073741824).toFixed(2) + " GB"))
    $(".system-info ul").append($("<li>").append("CPU Model: "    + os.cpus()[0].model))
    $(".system-info ul").append($("<li>").append("CPU Cores: "    + os.cpus().length))
  })
}

/*
 * Stacer Navigation Menu
 */
function navigationClicks()
{
  var active = "nav-active"
  $("#nav > li > label").click( function()
  {
    if( ! $(this).hasClass(active) )
    {
      var data = $(this).attr("id")
      $(".content").hide("fast")
      $('div[data-nav=' + data + ']').show("fast")

      $("#nav > li > label").removeClass( active )
      $(this).addClass( active )
    }
  })
}

/*
 * Dashboard Page
 */
function dashboardPage()
{
  systemInformationBars()

  networkBars()
}

/*
 * System Cleaner Page
 */
function systemCleanerPage()
{
  /*
   * System Scan Button Click
   */
  $("#system-scan-btn").click(function()
  {
    // Reset Scan Results
    setCount("#crash-rep-choose" , 0)
    setCount("#apt-cache-choose" , 0)
    setCount("#sys-logs-choose"  , 0)
    setCount("#app-cache-choose" , 0)

    // Clear List
    $("#cleaner-table > .tdl-content > ul").html("");

    /*
     * System apt cache scanning
     */
    if ($("#apt-cache-choose").is(":checked"))
    {
      exec( commands.aptCacheScanning , function(err, stdout, code)
        {
          addListTitle( "select-all-cache" , "Select All (Apt Caches)" )

          var i = 1
          stdout.split("\n").forEach(function( line )
          {
            if (line != "")
            {
              addListElement( "apt-cache-check" , line.replace("/var/cache/apt/archives/", "") )
              setCount("#apt-cache-choose" , i++)
            }
          })
          selectAllCheckbox('select-all-cache', 'apt-cache-check')
        })
    }

    /*
     * System crash reports scanning
     */
    if ($("#crash-rep-choose").is(":checked"))
    {
      exec( commands.crashReportsScanning , function(err, stdout, code)
        {
          addListTitle( "select-all-crash" , "Select All (Crash Reports)" )

          var i = 1
          stdout.split("\n").forEach(function( line )
          {
            if (line != "")
            {
              addListElement( "crash-check" , line )
              setCount("#crash-rep-choose" , i++)
            }
          })
          selectAllCheckbox('select-all-crash', 'crash-check')
      })
    }

    /*
     * System logs scanning
     */
    if ($("#sys-logs-choose").is(":checked"))
    {
      exec( commands.systemLogsScanning , function(err, stdout, code)
        {
          addListTitle( "select-all-logs" , "Select All (System Logs)" )

          var i = 1
          stdout.split("\n").forEach(function( line )
          {
              if (line != "")
              {
                  addListElement( "log-check" , line.replace("/var/log/", "") )
                  setCount("#sys-logs-choose" , i++)
              }
          })
          selectAllCheckbox('select-all-logs', 'log-check')
        })
    }

    /*
     * System app cache scanning
     */
    if ($("#app-cache-choose").is(":checked"))
    {
      exec( commands.appCacheScanning , function(err, stdout, code)
        {
          addListTitle( "select-all-app-cache" , "Select All (App Caches)" )

          var i = 1
          stdout.split("\n").forEach(function( line )
          {
              if (line != "")
              {
                  addListElement( "app-cache-check" , line )
                  setCount("#app-cache-choose" , i++)
              }
          })
          selectAllCheckbox('select-all-app-cache', 'app-cache-check')
      })
    }
  })

  /*
   * Clean Button Click
   */
  $("#clean-btn").click(function()
  {
    /*
     * App Cache Clean
     */
    var appCheckeds = getCheckeds("app-cache-check")
    var appLen      = getLength("app-cache-check")

    if( appCheckeds.length != 0 )
    {
      appCheckeds.each(function(c, check)
      {
        var appCacheFile = getFileName(this)
        exec(sudoCommand( commands.removeAppCache + appCacheFile ),
          function(err, stdout, code)
          {
              removeElement(check)
              setCount("#app-cache-choose" , --appLen)
          })
      })
    }

    /*
     * Apt Cache Clean
     */
    var aptCheckeds = getCheckeds("apt-cache-check")
    var aptLen      = getLength("apt-cache-check")

    if( aptCheckeds.length != 0 )
    {
      aptCheckeds.each(function(c, check)
      {
        var cacheFile = getFileName(this)
        exec(sudoCommand( commands.removeAptCache + cacheFile ),
          function(err, stdout, code)
          {
              removeElement(check)
              setCount("#apt-cache-choose" , --aptLen)
          })
      })
    }

    /*
     * Logs Clean
     */
    var logCheckeds = getCheckeds("log-check")
    var logLen      = getLength("log-check")

    if( logCheckeds.length != 0 )
    {
      logCheckeds.each(function(c, check)
      {
        var logFile = getFileName(this)
        exec(sudoCommand( commands.removeLogFile + logFile),
          function(err, stdout, code)
          {
              removeElement(check)
              setCount("#sys-logs-choose" , --logLen)
          })
      })
    }

    /*
     * System Crash Clean
     */
    var crashCheckeds = getCheckeds("crash-check")
    var crashLen      = getLength("log-check")

    if( crashCheckeds.length != 0 )
    {
      crashCheckeds.each(function(c, check)
      {
        var crashFile = getFileName(this)
        exec(sudoCommand( commands.removeCrashFile + crashFile),
          function(err, stdout, code)
          {
              removeElement(check)
              setCount("#crash-rep-choose" , --crashLen)
          })
      })
    }
  })
}

/*
 * Startup Apps Page
 */
function startupAppsPage()
{
  exec( commands.autostartApps , function(err, stdout, code)
    {
      stdout.split("\n").forEach( function(line)
      {
        if (line != "")
        {
          exec('cat ' + os.homedir() + '/.config/autostart/' + line + ' | grep Name=',
            function(err, stdout, code) {
              var appName = stdout.split("\n")[0].replace("Name=", "");
              $("#startup-apps-list")
              .append( $("<li>").append( appName , $("<a>").attr("name" , line)))
          })
        }
      })
    })

    setTimeout(function () {
      $("#startup-apps-list li a").click( function()
      {
        var _this      = $(this)
        var startupApp = _this.attr("name")
        var appName    = _this.parent("li").text()
        exec( sudoCommand( commands.removeAutostartApp + startupApp ),
        function(err, stdout, code)
        {
          _this.parent("li").remove()
          showMessage( appName + " is deleted." )
        })
      })
    }, 3000)
}

/*
 * Services Page
 */
function servicesPage()
{
  var isServ = false
  exec( commands.getAllService ,
    function(err, stdout, code)
    {
      var list = stdout.split("\n")
      var listCount = list.length
      list.forEach(function(line)
      {
        if (line != "")
        {
          var service = line.substring(1, line.length)
          var isRun   = line.substring(0, 1) == "+" ? "checked" : ""
          $("#system-service-list").append(
            '<li>' + service +
            '<input type="checkbox" class="switch" id="'+service+'" '+isRun+'/>'+
            '<label for="'+service+'"></label></li>')
        }
      })
      setTimeout(function () {
        $("#system-service-list li .switch").change(function()
        {
          if (!isServ)
          {
            isServ = true
            var _this = $(this)
            var task = _this.is(":checked") ? "start" : "stop"
            var service = _this.parent("li").text()
            exec( sudoCommand( " service " + service + " " + task ),
              function(err, stdout, code)
              {
                isServ = false
                showMessage( service + ' service ' + task + (_this.is(":checked") ? 'ed' : 'ped') )
              })
          }
          else
          {
            showMessage( "Another process continues." )
          }
        })
      }, 3000)

      $("#system-service-title span").text("System Services (" + listCount + ")")
      $('#system-service-search').fastLiveFilter('#system-service-list')
    })
}

/*
 * Uninstaller Page
 */
function uninstallerPage()
{
  var isInstalling = false;
  exec( commands.getInstalledPackages , function(err, stdout, code)
    {
      var list = stdout.split("\n")
      var listCount = list.length
      list.forEach(function(line)
      {
        if (line != "")
        {
          $("#installed-packages-list")
          .append( $("<li>").append( line , $("<a>") ) )
        }
      })

      setTimeout(function () {
        $("#installed-packages-list li a").click(function()
        {
          if (!isInstalling)
          {
            isInstalling = true
            var _this = $(this)
            _this.addClass("loader")
            var appName = _this.parent("li").text()
            exec( sudoCommand( commands.removePackage + appName),
              function(err, stdout, code)
              {
                _this.parent("li").remove()
                showMessage( appName + " package uninstalled." )
                isInstalling = false
                $("#installed-packages-title span").text("System Installed Packages (" + --listCount + ")")
            })
          }
          else
          {
            showMessage( "Another process continues." )
          }
        })
      }, 3000)

      $("#installed-packages-title span").text("System Installed Packages (" + listCount + ")")
      $('#packages-search').fastLiveFilter('#installed-packages-list')
    })
}

//---- Page Functions -----//

/*
 * Main
 */
$(document).ready(function()
{
  $("body").append($('<div>').load("./pages/login.html"))

  navigationClicks()

  dashboardPage()

  $("#system-cleaner-content").load( "./pages/system-cleaner.html",
  function() { systemCleanerPage() })

  $("#startup-apps-content").load( "./pages/startup-apps.html",
  function() { startupAppsPage() })

  $("#system-service-content").load( "./pages/system-services.html",
  function() { servicesPage() })

  $("#uninstaller-content").load( "./pages/uninstaller.html",
  function() { uninstallerPage() })

  /*
   * Close Button
   */
  $("#close-btn").click(function()
  {
    ipcRenderer.send('close-app')
  })

  /*
   * Minimize Button
   */
  $("#min-btn").click(function()
  {
    ipcRenderer.send('minimize-app')
  })

})
