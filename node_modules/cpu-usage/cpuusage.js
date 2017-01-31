var libfs = require( 'fs' );

var readStat = function( cb ) {
	libfs.readFile( "/proc/stat", function( err, data ) {
		cb( data.toString( ) );
	} );
}

function cpuUsage( freq, cb ) {
	
	if( typeof freq == 'function' ) {
		cb = freq;
		freq = 1000;
	}
	
	var prevTotal = 0;
	var prevIdle = 0;
	var prevLoad = 0;

	setInterval( function( ) {
		readStat( function( data ) {
			var dRaw = data.split( ' ' );
			
			var d = [];
			var idx = 1;
			var count = 0;
			while( count < 4 ) {
				var t = parseInt( dRaw[ idx ] );
				if( t ) {
					count++;
					d.push( t );
				}
				idx++;
			}
			
			var idle = parseInt( d[ 3 ] );
			var total = parseInt( d[ 0 ] ) + parseInt( d[ 1 ] ) + parseInt( d[ 2 ] );
			var load = 0;
			
			if( prevTotal != 0 ) {
				load = Math.round( ( total - prevTotal ) / ( total + idle - prevTotal - prevIdle ) * 100 );
				
				if( cb )
					cb( load );
			}
			
			prevLoad = load;
			prevTotal = total;
			prevIdle = idle;
		} );
	}, freq || 1000 );
};

module.exports = cpuUsage;
