UAlbertaBot with Brood War Strategy Server (https://github.com/njustesen/broodwar_strategy_server)

Author of UAlbertaBot:  David Churchill (dave.churchill@gmail.com)
Author of Brood War Strategy Server extension by Niels Justesen (njustesen@gmail.com)

Please check out the [Wiki](https://github.com/davechurchill/ualbertabot/wiki) for full instructions and documentation!

## Setup project i visual studo
- Follow instructions on [Wiki](https://github.com/davechurchill/ualbertabot/wiki)
- Install boost
- In project settings -> Configuration Settings -> C/C++ -> General -> Additional Include Directories: C:\libs\boost_1_60_0
- Linker -> Additional Library Directories: C:\libs\boost_1_60_0\stage\lib

## How to run (Only in debug mode)
- Setup and run Brood War Strategy Server on local machine on port 8000. (Guide on https://github.com/njustesen/broodwar_strategy_server)
- Run StarCraft from ChaosLauncher similar to https://github.com/davechurchill/ualbertabot/wiki/Installation-Instructions#how-to-run - but it must be in Debug mode to work with Brood War Strategy Server
- You can run several games without restarting the Brood War Strategy Server

## Known bugs
- The game might crash when Interceptors or Scarabs are used
