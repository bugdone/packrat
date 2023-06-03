This project only made sense back in the day when there were no better alternatives. See [packrat.js](https://github.com/bugdone/packrat.js)

### About ###
Packrat downloads the latest CSGO matchmaking demos for a set of steam accounts.

### Dependencies ###
On Windows you need the [Visual C++ Redistributable Packages for VS 2013 x86](http://download.microsoft.com/download/2/E/6/2E61CFA4-993B-4DD4-91DA-3737CD5CD6E3/vcredist_x86.exe)

### Disclamers ###
C++ code is based on https://bitbucket.org/ACB/boiler hence GPL3.

### Usage ###
Edit __packrat.ini__ to configure the path to steam binary, demo directory and steam accounts.

If you don't have any accounts configured, it will download the demos for the currently logged in account if steam is running. Otherwise, running packrat __kills__ the current steam process, launches steam logged in with each user and downloads the latest 8 demos (if they aren't present in that directory). Since steam pops up windows (log in window, steam window) when it starts even with _-silent_ (which doesn't seem to work at all on linux) the most convenient way to use it is to set it up in cron/windows scheduled tasks (ie. run every night at 4:00).

It will __NOT__ check if the downloaded demos are corrupted.

Since packrat generates the same files that the csgo client does when downloading a demo from the Watch menu (x.dem and x.dem.info), the demos should show up in the Watch menu.

### Build dependencies ###
* [Steamworks SDK](https://partner.steamgames.com/)

* [protobuf](https://developers.google.com/protocol-buffers/docs/downloads)

* cmake

### Building on Linux ###
Download Steamworks SDK (you need to create a free account) and extract it somewhere. Set the environment variable __STEAMWORKS_SDK__ to the path where you extracted it. Cmake will use it to figure out the paths to libs and headers.

For protobuf a package named something like _protobuf-dev_ should be available on your official distribution repository.

Once the dependencies are set, run the usual __mkdir build && cd build && cmake .. && make__

The broiler binary needs to find steam_appid.txt into its directory or the directory from where it is run.
