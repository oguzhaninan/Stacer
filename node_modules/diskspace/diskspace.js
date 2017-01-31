'use strict';
var osType = require('os').type();
var exec = require('child_process').exec;
var execFile = require('child_process').execFile;
var path = require('path');

function check(drive, callback)
{
	var total = 0;
	var free = 0;
	var status = null;

	if (!drive)
	{
		status = 'NOTFOUND';
		var error = new Error('Necessary parameter absent');

		return callback
					? callback(error, total, free, status)
					: console.error(error);
	}

	if (osType === 'Windows_NT') //Windows
	{

		if(drive.length <= 3)
			drive = drive.charAt(0);

		execFile(path.join(__dirname, 'drivespace.exe'),["drive-"+ drive], function (error, stdout, stderr)
		{
			if (error)
			{
				status = 'STDERR';
			}
			else
			{
				var disk_info = stdout.trim().split(',');

				total = disk_info[0];
				free = disk_info[1];
				status = disk_info[2];

				if (status === 'NOTFOUND')
				{
					error = new Error('Drive not found');
				}

			}

			callback ? callback(error, total, free, status)
						 : console.error(stderr);
		});
	}
	else
	{
		exec("df -k '" + drive.replace(/'/g,"'\\''") + "'", function(error, stdout, stderr)
		{
			if (error)
			{
				if (stderr.indexOf("No such file or directory") != -1)
				{
					status = 'NOTFOUND';
				}
				else
				{
					status = 'STDERR';
				}

				callback ? callback(error, total, free, status)
						 : console.error(stderr);
			}
			else
			{
				var lines = stdout.trim().split("\n");

				var str_disk_info = lines[lines.length - 1].replace( /[\s\n\r]+/g,' ');
				var disk_info = str_disk_info.split(' ');

				total = disk_info[1] * 1024;
				free = disk_info[3] * 1024;
				status = 'READY';

				callback && callback(null, total, free, status);
			}
		});
	}
}

// Export public API
module.exports = {
	check: check,
};
