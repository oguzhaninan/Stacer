
<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/electron/Screenshots/Header.png" width="800">    
</p>
<p align="center">
  <b>Linux System Optimizer and Monitoring</b>
</p>

<p align="center">
    <a href="https://github.com/sindresorhus/awesome-electron">
		<img alt="Awesome"  src="https://cdn.rawgit.com/sindresorhus/awesome/d7305f38d29fed78fa85652e3a63e154dd8e8829/media/badge.svg"/>
	</a>    
	<a href="https://sourceforge.net/projects/stacer/files/" rel="nofollow">
		<img alt="Download Stacer" src="https://img.shields.io/sourceforge/dt/stacer.svg"/>
	</a>
	<a href="http://www.kernel.org">
		<img alt="Platform (GNU/Linux)" src="https://img.shields.io/badge/platform-GNU/Linux-blue.svg"/>
	</a>
</p>

<p align="center">
    <a href="https://sourceforge.net/projects/stacer/files/" >
		<img src="https://a.fsdn.com/con/app/sf-download-button" />
	</a>
</p>

## Reviews
<p align="left">
    <a href="http://www.omgubuntu.co.uk/2017/01/stacer-system-optimizer-for-ubuntu">
		<img width="70px" src="http://www.omgubuntu.co.uk/wp-content/themes/omgubuntu-theme-3.6.1/images/logo.png"/>
	</a>        
    <a href="http://www.diolinux.com.br/2017/02/stacer-um-programa-para-otimizar-o-ubuntu.html">
		<img width="170px" src="http://coletivo.semanadolinux.com.br/images/site/diolinux.png"/>
	</a>    
    <a href="https://www.genbeta.com/linux/stacer-una-app-todo-en-uno-que-te-deja-monitorizar-y-optimizar-el-sistema-en-ubuntu">
		<img width="160px" src="http://seanfee.com/wp-content/uploads/2014/04/Logo_genbeta.jpg"/>
	</a>    
    <a href="http://computerhoy.com/noticias/software/stacer-herramienta-que-te-permite-optimizar-ubuntu-57974">
		<img width="130px" src="http://computerhoy.com/sites/computerhoy.com/themes/computerhoy/logo-puntocom.png"/>
	</a>    
    <a href="http://www.dobreprogramy.pl/Stacer-program-do-optymalizacji-Ubuntu-ktory-wyglada-jakby-uciekl-z-Windowsa,News,78941.html">
		<img width="160px" src="http://storage.dobreprogramy.pl/artykuly/9urodziny/logoDP-napis.png"/>
	</a>
    <a href="http://blog.desdelinux.net/optimizar-debian-ubuntu-linux-mint-derivados-stacer/">
		<img width="160px" src="http://i.imgur.com/eV1WxYZ.png"/>
	</a>
	<a href="http://www.techrepublic.com/article/how-to-install-stacer-for-quick-linux-system-optimization/">
		<img width="160px" src="https://static1.squarespace.com/static/55afdc11e4b000b229a97849/t/55b13f1de4b044f556a02524/1437679408913/"/>
	</a>
    <a href="https://www.fossmint.com/stacer-ubuntu-system-optimizer/">
		<img width="160px" src="https://www.fossmint.com/wp-content/uploads/2017/02/logo.png"/>
	</a>
    <a href="http://www.2daygeek.com/stacer-linux-system-optimizer-and-monitoring-tool/">
		<img width="160px" src="http://www.2daygeek.com/wp-content/uploads/2015/12/2day-geek-new-logo-final.png"/>
	</a>
    <a href="http://www.noobslab.com/2017/03/stacer-sytem-optimizer-must-have.html">
		<img width="160px" src="http://2.bp.blogspot.com/-Pnf0jwJPpEg/VzJxbWzYgCI/AAAAAAAAHhA/5WJys7XD_1QFHF-CfEp6Bo7VrVaubN3sgCK4B/s1600/nl-logo-1.gif"/>
	</a>
</p>

## Installing

### Required for Ubuntu 14

Run `sudo apt-get install systemd -f`

### AppImage

1. Download `stacer-1.0.7-x86_64.AppImage` from the [Stacer releases page](https://github.com/oguzhaninan/Stacer/releases).
2. Make it executable `sudo chmod a+x stacer*.AppImage`
3. Run `./stacer*.AppImage`

### Debian Linux x64

1. Download `stacer_1.0.7_amd64.deb` from the [Stacer releases page](https://github.com/oguzhaninan/Stacer/releases).
2. Run `sudo dpkg --install stacer_1.0.7_amd64.deb` on the downloaded package.
3. Launch Stacer using the installed `stacer` command.

### Debian Linux x86

1. Download `stacer_1.0.7_i386.deb` from the [Stacer releases page](https://github.com/oguzhaninan/Stacer/releases).
2. Run `sudo dpkg --install stacer_1.0.7_i386.deb` on the downloaded package.
3. Launch Stacer using the installed `stacer` command.

### Fedora

1. Download `stacer-1.0.7.rpm` from the [Stacer releases page](https://github.com/oguzhaninan/Stacer/releases).
2. Run `sudo rpm --install stacer-1.0.7.rpm` on the downloaded package.
3. Launch Stacer using the installed `stacer` command.

## Build from source

1. `git clone https://github.com/oguzhaninan/Stacer.git`
2. `cd Stacer`
3. `npm install && npm run build && npm start`

## Uninstalling
- Run `sudo dpkg -r stacer`

## Screenshots

### Dashboard
<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/electron/Screenshots/Screenshot-v1.0.7-1.png" width="700">
</p>

### System Cleaner
<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/electron/Screenshots/Screenshot-v1.0.7-2.png" width="700">
</p>

### Startup Apps
<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/electron/Screenshots/Screenshot-v1.0.7-3.png" width="700">
</p>

### Startup Apps - Add App
<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/electron/Screenshots/Screenshot-v1.0.7-4.png" width="700">
</p>

### Services
<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/electron/Screenshots/Screenshot-v1.0.7-5.png" width="700">
</p>

### Processes
<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/electron/Screenshots/Screenshot-v1.0.7-6.png" width="700">
</p>

### Uninstaller
<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/electron/Screenshots/Screenshot-v1.0.7-7.png" width="700">
</p>

### Resources
<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/electron/Screenshots/Screenshot-v1.0.7-8.png" width="700">
</p>

### Resources
<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/electron/Screenshots/Screenshot-v1.0.7-9.png" width="700">
</p>

### Settings
<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/electron/Screenshots/Screenshot-v1.0.7-10.png" width="700">
</p>
