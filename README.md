
<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/native/screenshots/header.png" width="800">    
</p>
<p align="center">
  <b>Linux System Optimizer and Monitoring</b>   <br>
  <a href="https://www.patreon.com/oguzhaninan">
	<img alt="Patreon" src="https://c5.patreon.com/external/logo/become_a_patron_button.png" height="50" />
</a>
	
<a href="https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=PDMEZXLTSRLHE">
	<img src="https://avaazimages.s3.amazonaws.com/paypal_donate_button.jpg" height="50"  />
</a>
</p>

<p align="center">
	<a href="https://sourceforge.net/projects/stacer/files/" rel="nofollow">
		<img alt="Download Stacer" src="https://img.shields.io/sourceforge/dt/stacer.svg"/>
	</a>
	<a href="http://www.kernel.org">
		<img alt="Platform (GNU/Linux)" src="https://img.shields.io/badge/platform-GNU/Linux-blue.svg"/>
	</a>
	<a href="https://github.com/oguzhaninan/Stacer/releases">
		<img alt="Github All Releases" src="https://img.shields.io/github/downloads/oguzhaninan/stacer/total.svg"/>
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
		<img width="155px" src="http://seanfee.com/wp-content/uploads/2014/04/Logo_genbeta.jpg"/>
	</a>
    <a href="http://www.dobreprogramy.pl/Stacer-program-do-optymalizacji-Ubuntu-ktory-wyglada-jakby-uciekl-z-Windowsa,News,78941.html">
		<img width="155px" src="http://storage.dobreprogramy.pl/artykuly/9urodziny/logoDP-napis.png"/>
	</a>
    <a href="http://blog.desdelinux.net/optimizar-debian-ubuntu-linux-mint-derivados-stacer/">
		<img width="155px" src="http://i.imgur.com/eV1WxYZ.png"/>
	</a>
	<a href="http://www.techrepublic.com/article/how-to-install-stacer-for-quick-linux-system-optimization/">
		<img width="150px" src="https://static1.squarespace.com/static/55afdc11e4b000b229a97849/t/55b13f1de4b044f556a02524/1437679408913/"/>
	</a>
	</a>
</p>


#### Building from Source???
<a href="#build-source">Click Here.</a>

### Required Packages
###### Required Binaries
- wget, curl, grep, systemd

###### Required Libraries
- libpcre++0v5, libpcre++0v5-dev _(if building)_

### PPA Repository (for ubuntu)
1. `sudo add-apt-repository ppa:oguzhaninan/stacer`   
2. `sudo apt-get update`   
3. `sudo apt-get install stacer`

### Debian x64

1. Download `stacer_1.0.9_amd64.deb` from the [Stacer releases page](https://github.com/oguzhaninan/Stacer/releases).
2. Run `sudo dpkg -i stacer*.deb` on the downloaded package.
3. Launch Stacer using the installed `stacer` command.

### Fedora x64

1. Download `stacer_1.0.9_amd64.rpm` from the [Stacer releases page](https://github.com/oguzhaninan/Stacer/releases).
2. Run `sudo rpm --install stacer*.rpm --nodeps --force` on the downloaded package.
3. Launch Stacer using the installed `stacer` command.

<a target="_self" id="build-source">&nbsp;</a>
## Build from source with CMake (Qt Version Qt 5.x)
##### First off...
If you don't have [libsdbusplus](https://github.com/openbmc/sdbusplus) installed on your machine, execute:
```
build-aux/build-sddp.sh
```

##### Afterwards...
1. mkdir build && cd build
2. cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/qt/path/bin ..
3. make -j `nproc`
4. output/bin/stacer

## Screenshots

<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/native/screenshots/Screenshot-1.0.9-1.png" width="700">
</p>

<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/native/screenshots/Screenshot-1.0.9-2.png" width="700">
</p>

<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/native/screenshots/Screenshot-1.0.9-3.png" width="700">
</p>

<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/native/screenshots/Screenshot-1.0.9-4.png" width="700">
</p>

<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/native/screenshots/Screenshot-1.0.9-5.png" width="700">
</p>

<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/native/screenshots/Screenshot-1.0.9-6.png" width="700">
</p>

<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/native/screenshots/Screenshot-1.0.9-7.png" width="700">
</p>

<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/native/screenshots/Screenshot-1.0.9-8.png" width="700">
</p>

<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/native/screenshots/Screenshot-1.0.9-9.png" width="700">
</p>

<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/native/screenshots/Screenshot-1.0.9-10.png" width="700">
</p>

<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/native/screenshots/Screenshot-1.0.9-11.png" width="700">
</p>

<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/native/screenshots/Screenshot-1.0.9-12.png" width="700">
</p>

<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/native/screenshots/Screenshot-1.0.9-13.png" width="700">
</p>

<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/native/screenshots/Screenshot-1.0.9-14.png" width="700">
</p>

<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/native/screenshots/Screenshot-1.0.9-15.png" width="700">
</p>

<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/native/screenshots/Screenshot-1.0.9-16.png" width="700">
</p>
