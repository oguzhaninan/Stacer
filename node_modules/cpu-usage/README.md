# Nodejs Cpu Usage (for linux)

Module read file /proc/stat and calculate Cpu Usage.

Declaration:

```
/*
 * frequency - how often CpuLoad will be calculated (optional, default = 1000 (= 1 second))
 * callback - function that will be called when a new value is calculated for CpuLoad 
 */

function cpuUsage( [ frequency, ] callback );
```

Eg of use:

```
var libCpuUsage = require( './cpuusage.js' );

libCpuUsage( 1000, function( load ) {
	process.stdout.write( "\r" + load + "%   " );
} );
```

