
<p align="center">
⚠️ <b>This project has been abandoned. There will be no further releases </b>⚠️
</p>

<p align="center">
    <img src="https://raw.githubusercontent.com/oguzhaninan/Stacer/native/screenshots/header.png" width="800">    
</p>
<p align="center">
  <b>Linux System Optimizer and Monitoring</b>   <br>
  <a href="https://www.patreon.com/oguzhaninan">
	<img alt="Patreon" src="https://c5.patreon.com/external/logo/become_a_patron_button.png" height="50" />
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
		<img width="65px" src="https://oguzhaninan.github.io/Stacer-Web/images/sites/site0.png"/>
	</a>      
    <a href="http://blog.desdelinux.net/optimizar-debian-ubuntu-linux-mint-derivados-stacer/">
		<img width="155px" src="http://i.imgur.com/eV1WxYZ.png"/>
	</a>
	<a href="http://www.techrepublic.com/article/how-to-install-stacer-for-quick-linux-system-optimization/">
		<img width="150px" src="https://static1.squarespace.com/static/55afdc11e4b000b229a97849/t/55b13f1de4b044f556a02524/1437679408913/"/>
	</a>
	</a>
</p>

### Required Packages
- curl, systemd

### PPA Repository (for ubuntu)
1. `sudo add-apt-repository ppa:oguzhaninan/stacer -y`   
2. `sudo apt-get update`   
3. `sudo apt-get install stacer -y`


### Arch Linux (AUR)
1. Install the stacer package with a AUR helper of your choice eg.
2. `yay -Syyu stacer`
3. `paru -S stacer`
4. `pacaur -a stacer`

### Debian x64

1. Download `stacer_1.1.0_amd64.deb` from the [Stacer releases page](https://github.com/oguzhaninan/Stacer/releases).
2. Run `sudo dpkg -i stacer*.deb` on the downloaded package.
3. Launch Stacer using the installed `stacer` command.

### Debian sid / Ubuntu 20.04+

1. Run as root `apt install stacer`

### Fedora
1. Download `stacer_1.1.0_amd64.rpm` from the [Stacer releases page](https://github.com/oguzhaninan/Stacer/releases).
2. Run `sudo rpm --install stacer*.rpm --nodeps --force` on the downloaded package.
3. Launch Stacer using the installed `stacer` command.

### Fedora (with DNF)
1. Run: `sudo dnf install stacer`
2. Launch Stacer using the installed `stacer` command.

## Build from source with CMake (Qt Version Qt 5.x)
1. `mkdir build && cd build`
2. `cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/qt/path/bin ..`
3. `make -j $(nproc)`
4. `output/bin/stacer`

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

## Contributors

### Code Contributors

This project exists thanks to all the people who contribute. [[Contribute](CONTRIBUTING.md)].
<a href="https://github.com/oguzhaninan/Stacer/graphs/contributors"><img src="https://opencollective.com/Stacer/contributors.svg?width=890&button=false" /></a>

### Financial Contributors

Become a financial contributor and help us sustain our community. [[Contribute](https://opencollective.com/Stacer/contribute)]

#### Individuals

<a href="https://opencollective.com/Stacer"><img src="https://opencollective.com/Stacer/individuals.svg?width=890"></a>

#### Organizations

Support this project with your organization. Your logo will show up here with a link to your website. [[Contribute](https://opencollective.com/Stacer/contribute)]

<a href="https://opencollective.com/Stacer/organization/0/website"><img src="https://opencollective.com/Stacer/organization/0/avatar.svg"></a>
<a href="https://opencollective.com/Stacer/organization/1/website"><img src="https://opencollective.com/Stacer/organization/1/avatar.svg"></a>
<a href="https://opencollective.com/Stacer/organization/2/website"><img src="https://opencollective.com/Stacer/organization/2/avatar.svg"></a>
<a href="https://opencollective.com/Stacer/organization/3/website"><img src="https://opencollective.com/Stacer/organization/3/avatar.svg"></a>
<a href="https://opencollective.com/Stacer/organization/4/website"><img src="https://opencollective.com/Stacer/organization/4/avatar.svg"></a>
<a href="https://opencollective.com/Stacer/organization/5/website"><img src="https://opencollective.com/Stacer/organization/5/avatar.svg"></a>
<a href="https://opencollective.com/Stacer/organization/6/website"><img src="https://opencollective.com/Stacer/organization/6/avatar.svg"></a>
<a href="https://opencollective.com/Stacer/organization/7/website"><img src="https://opencollective.com/Stacer/organization/7/avatar.svg"></a>
<a href="https://opencollective.com/Stacer/organization/8/website"><img src="https://opencollective.com/Stacer/organization/8/avatar.svg"></a>
<a href="https://opencollective.com/Stacer/organization/9/website"><img src="https://opencollective.com/Stacer/organization/9/avatar.svg"></a>
