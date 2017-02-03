const {app, ipcRenderer} = require('electron')

const fs          = require('fs')
const os          = require('os')
const libCpuUsage = require('cpu-usage')
const diskspace   = require('diskspace')
const si          = require('systeminformation')
const { spawn, spawnSync }   = require('child_process')
const prog        = require('progressbar.js')
const sudo        = require('sudo-prompt')

var { prop, commands } = require('./src/config')
require('./libs/fast-search')
require('./libs/amaran.min')

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

function Command( command )
{
 return 'bash -c "' + command + '"'
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
          bar.setText(Math.abs(down).toString() + ' kB/s')
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
          bar.setText(Math.abs(up).toString() + ' kB/s')
      }
  });

  /*
   * Network down and upload speed setter
   */
  setInterval( () =>
  {
      si.networkStats(defaultNetwork, (data) =>
      {
          down = (data.rx_sec / 1024).toFixed(2)
          up   = (data.tx_sec / 1024).toFixed(2)
          downBar.animate(down / 2000)
          upBar.animate(up / 2000)
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
  function prettySize( size ){
    return ( size / ( Math.pow(1024, 3) ) ).toFixed(2)
  }

  setInterval( function()
  {
    si.mem( ( ram ) => 
    {
      var usedMem  = ram.total - ram.available
      var totalMem = ram.total


      memInfo = prettySize(usedMem) + ' / ' + prettySize(totalMem) + "GB"
      memBar.animate(usedMem / totalMem)
    })

  }, prop.memDuration);

  /*
   * Disk value setter
   */
  function diskInfo()
  {
    diskspace.check('/', function(err, total, free, status)
    {
      var used = total-free
      dUsed  = (used  / 1000000000).toFixed(1)
      dTotal = (total / 1000000000).toFixed(1) + "GB"
      diskBar.animate( used / total );
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
    $(".system-info ul").append($("<li>").append("Total RAM: "    + prettySize( os.totalmem() ) + " GB"))
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
      fs.readdir( commands.aptCacheScanning , 'utf8', (err, files) =>
      {
        addListTitle( "select-all-cache" , "Select All (Apt Caches)" )

        if ( ! err )
        {
          var i = 1
          files.filter( ( file ) => file.endsWith('.deb') )
              .forEach( ( file ) => 
          {          
            addListElement( "apt-cache-check" , file )
            setCount("#apt-cache-choose" , i++)
          })

          selectAllCheckbox('select-all-cache', 'apt-cache-check')
        } 
        else
        {
            console.log(err);
        }
      })
    }

    /*
     * System crash reports scanning
     */
    if ($("#crash-rep-choose").is(":checked"))
    {
      fs.readdir( commands.crashReportsScanning , 'utf8', (err, files) =>
      {
        addListTitle( "select-all-crash" , "Select All (Crash Reports)" )
        
        if ( ! err )
        {
          var i = 1
          files.forEach( ( file ) =>
          {          
            addListElement( "crash-check" , file )
            setCount("#crash-rep-choose" , i++)
          })

          selectAllCheckbox('select-all-crash', 'crash-check')
        } 
        else
        {
          console.log(err);
        }
      })
    }

    /*
     * System logs scanning
     */
    if ($("#sys-logs-choose").is(":checked"))
    {
      fs.readdir( commands.systemLogsScanning , 'utf8', (err, files) =>
      {
        addListTitle( "select-all-logs" , "Select All (System Logs)" )        
        
        if ( ! err )
        {
          var i = 1
          files.forEach( ( file ) =>
          {          
            addListElement( "log-check" , file )
            setCount("#sys-logs-choose" , i++)
          })

          selectAllCheckbox('select-all-logs', 'log-check')
        } 
        else
        {
          console.log(err);
        }
      })
    }

    /*
     * System app cache scanning
     */
    if ($("#app-cache-choose").is(":checked"))
    {
      fs.readdir( commands.appCacheScanning , 'utf8', (err, files) =>
      {
        addListTitle( "select-all-app-cache" , "Select All (App Caches)" )      
        
        if ( ! err )
        {
          var i = 1
          files.forEach( ( file ) =>
          {          
            addListElement( "app-cache-check" , file )
            setCount("#app-cache-choose" , i++)
          })

          selectAllCheckbox('select-all-app-cache', 'app-cache-check')
        } 
        else
        {
          console.log(err);
        }
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
      var filesToRemove = ''
      appCheckeds.each(function(c, check)
      {
        const appCacheFile = getFileName(check)

        filesToRemove += ' rm -r ' + commands.appCacheScanning + appCacheFile + ';'

        removeElement(check)
        setCount("#app-cache-choose" , --appLen)
      })

      sudo.exec( Command(filesToRemove) , {name: 'Stacer'},
            (error, stdout, stderr) =>
        {
          
        })
    }

    /*
     * Apt Cache Clean
     */
    var aptCheckeds = getCheckeds("apt-cache-check")
    var aptLen      = getLength("apt-cache-check")

    if( aptCheckeds.length != 0 )
    {
      var filesToRemove = ''
      aptCheckeds.each( (c, check) =>
      {
        const cacheFile = getFileName(check)
        
        filesToRemove += ' rm ' + commands.aptCacheScanning + cacheFile + ';'

        setCount("#apt-cache-choose" , --aptLen)
        removeElement(check)
      })

      sudo.exec( Command(filesToRemove) , {name: 'Stacer'},
            (error, stdout, stderr) =>
        {
          
        })
    }

    /*
     * Logs Clean
     */
    var logCheckeds = getCheckeds("log-check")
    var logLen      = getLength("log-check")

    if( logCheckeds.length != 0 )
    {
      var filesToRemove = ''
      logCheckeds.each(function(c, check)
      {
        const logFile = getFileName(check)

        filesToRemove += ' rm -r ' + commands.systemLogsScanning + logFile + ';'

        removeElement(check)
        setCount("#sys-logs-choose" , --logLen)
      })

      sudo.exec( Command(filesToRemove) , {name: 'Stacer'},
            (error, stdout, stderr) =>
        {
          
        })
    }

    /*
     * System Crash Clean
     */
    var crashCheckeds = getCheckeds("crash-check")
    var crashLen      = getLength("crash-check")

    if( crashCheckeds.length != 0 )
    {
      var filesToRemove = ''
      crashCheckeds.each(function(c, check)
      {
        const crashFile = getFileName(check)

        filesToRemove += ' rm ' + commands.crashReportsScanning + crashFile + ';'

        removeElement(check)
        setCount("#crash-rep-choose" , --crashLen)        
      })

      sudo.exec( Command(filesToRemove) , {name: 'Stacer'},
            (error, stdout, stderr) =>
        {
          
        })
    }
  })
}

/*
 * Startup Apps Page
 */
function startupAppsPage()
{
  try
  {
    fs.readdir( commands.autostartApps, (err, files) => {

      files.filter( ( file ) => file.endsWith('.desktop') )
          .forEach(( file ) =>
      {
        try
        {
          var data = fs.readFileSync(commands.autostartApps + '/' + file).toString()
        
          var appName = data.match(/\Name=.*/g)[0].replace('Name=', '')
          $('#startup-apps-list').append( $("<li>").append( appName , $("<a>").attr("name" , file) ) )
        }
        catch(err){}      
      })

      $("#startup-apps-list li a").click( function()
      {
        var _this      = $(this)
        var startupApp = _this.attr("name")
        var appName    = _this.parent("li").text()

        try
        {
          fs.unlink( commands.autostartApps +  startupApp )

          _this.parent("li").remove()
          showMessage( appName + " is deleted." )
        }
        catch(er){}

      })
    })
  }
  catch (error) {
    console.log(error)
  }
  
}

/*
 * Services Page
 */
function servicesPage()
{
  var isServ = false

  const services = spawn('bash', ['-c', commands.getAllService])
  
  services.stdout.on('data', ( data ) => {
    data = data.toString().split('\n').filter( ( s ) => s != '')

    const serviceCount = data.length
  
    data.forEach( ( serv ) => 
    {
      var service = serv.substring(1)
      var isRun   = serv.substring(0, 1) == "+" ? "checked" : ""

      $("#system-service-list").append(
        '<li>' + service +
        '<input type="checkbox" class="switch" id="'+service+'" '+isRun+'/>'+
        '<label for="'+service+'"></label></li>')
    })

    $("#system-service-list li .switch").change(function()
    {
      if ( ! isServ)
      {
        isServ = true
        var _this = $(this)
        var task = _this.is(":checked") ? "start" : "stop"
        var service = _this.parent("li").text()
        
        sudo.exec( Command( "service " + service + " " + task ) , {name: 'Stacer'},
                  (error, stdout, stderr) =>
        {
          if(stderr)
            showMessage( "Operation not successful." )
          else
            showMessage( service + ' service ' + task + (_this.is(":checked") ? 'ed' : 'ped') )
          
          isServ = false
        })

        $("#system-service-title span").text("System Services (" + serviceCount + ")")
      }
      else
      {
        showMessage( "Another process continues." )
      }
    })

    $('#system-service-search').fastLiveFilter('#system-service-list')
  })
  
}

/*
 * Uninstaller Page
 */
function uninstallerPage()
{
  var isInstalling = false

  const packages = spawn('bash', ['-c', commands.getInstalledPackages])

  packages.stdout.on('data', (data) => {
    data = data.toString().split('\n').filter( ( s ) => s != '')

    var packagesCount = data.length

    data.forEach( ( package ) =>
    {
      $("#installed-packages-list")
        .append( $("<li>").append( package , $("<a>") ) )
      
    })

    $("#installed-packages-list li a").click(function()
    {
      if ( ! isInstalling)
      {
        isInstalling = true
        var _this = $(this)
        _this.addClass("loader")
        var appName = _this.parent("li").text()


        sudo.exec( Command( commands.removePackage + appName ) , {name: 'Stacer'},
              (error, stdout, stderr) =>
        {
          if(stderr)
            showMessage( "Operation not successful." )
          else
          {
            _this.parent("li").remove()
            showMessage( appName + " package uninstalled." )
            $("#installed-packages-title span").text("System Installed Packages (" + --packagesCount + ")")
          }
          
          isInstalling = false
        })             
      }
      else
      {
        showMessage( "Another process continues." )
      }
    })

    $("#installed-packages-title span").text("System Installed Packages (" + packagesCount + ")")
    $('#packages-search').fastLiveFilter('#installed-packages-list')
  })

      
}

//---- Page Functions -----//

/*
 * Main
 */
$( document ).ready(function()
{
  navigationClicks()

  dashboardPage()

  $("#system-cleaner-content").load( "./pages/system-cleaner.html", () => { systemCleanerPage() })

  $("#startup-apps-content").load( "./pages/startup-apps.html", () => { startupAppsPage() })

  $("#system-service-content").load( "./pages/system-services.html", () => { servicesPage() })

  $("#uninstaller-content").load( "./pages/uninstaller.html", () => { uninstallerPage() })
 
  setTimeout( ( ) => {
    $('#loading').remove()
  }, 3500);
  
})
