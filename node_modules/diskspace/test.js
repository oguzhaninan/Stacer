var diskspace = require('./diskspace.js'); //use just diskspace if installed via NPM
var os = require('os');
var drive_letter = null;

if (os.type() == 'Windows_NT')
{
	drive_letter = 'C'; //Good Mount Point
	//drive_letter = 'K'; //Bad Mount Point - Well on my system
}
else if (os.type() == 'Darwin' || os.type() == 'Linux') //Mac OS or Linux
{
	drive_letter = '/';
}
else
{
	drive_letter = null;
}

diskspace.check(drive_letter, function (err, total, free, status)
{
    console.log('err: ' + err);
	console.log('Total: ' + total);
	console.log('Free: ' + free);
	console.log('Status: ' + status);
});