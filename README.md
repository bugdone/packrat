### About ###
Packrat downloads the latest CSGO matchmaking demos for a set of steam accounts.

### Windows build ###
You can find a windows build [here](https://bitbucket.org/bugdone/packrat/downloads/packrat_win32.zip).

### Disclamers ###
Use at your own risk.

C++ code is based on https://bitbucket.org/ACB/boiler hence GPL3.

### Usage ###
Edit __packrat.ini__ to configure the path to steam binary, demo directory and steam users.

Running packrat __kills__ the current steam process, launches steam logged in with each user and downloads the latest 8 demos (if they aren't present in that directory). Since steam pops up windows (log in window, steam window) when it starts even with _-silent_ (which doesn't seem to work at all on linux) the most convenient way to use it is to set it up in cron/windows scheduled tasks (ie. run every night at 4:00).

It will __NOT__ check if the downloaded demos are corrupted.

Since packrat generates the same files that the csgo client does when downloading a demo from the Watch menu (x.dem and x.dem.info), the demos should show up in the Watch menu (but it doesn't always happen for some reason).

### Build dependencies ###
* [Steamworks SDK](https://partner.steamgames.com/)

* [protobuf](https://developers.google.com/protocol-buffers/docs/downloads)

* cmake