# Description
just a simple discord music bot based on [DPP(D++)](https://github.com/brainboxdotcc/DPP) and [mpg123](https://www.mpg123.de)

# feature
Streaming the server local mp3 file to voice channel.

discord gui for easy use.

# Dependencie require
## DPP 
### install
the installation is based on your operation system, 

check the [installation page](https://dpp.dev/index.html) from DPP(D++) for further information.

## mpg123
### install 
#### Linux
Most Linux distributions already installed the mpg123, 

for some distrubution that doesn't have mpg123 package, check the distrubution pacakge installation.

#### MacOS
use the brew to fast install the package.

` brew install mpg123 `


# config 
put the discord bot Token in `src/BOT_TOKEN.h`

# usage 
after put the token, 

run

`mkdir build`

`cd build`

`cmake ..`

`cmake --build .`

after the build you will see your discord bot in build directory.

then run

`./{the executive file name}`

# known bug
it's a little bit of laggy when output pcm into the voicechannel.

# feature that may update
- [ ] support more audio type (maybe chage to ffmpeg to get more support)
- [ ] stop feature (still haven't get how to control the thread)
