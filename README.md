<p align="center">
    <img alt="logo" height="300" src="https://raw.github.com/hyperbolus/blackjack/master/blackjack.svg">
</p>
<p align="center">
<img alt="license" src="https://img.shields.io/github/license/hyperbolus/blackjack">
<img alt="version" src="https://img.shields.io/badge/version-v1.0.0-brightgreen">
<img alt="Discord" src="https://img.shields.io/discord/839698528170409996">
</p>

# Usage
Grab a build from the Releases section or compile with Visual Studio
```
Usage: Blackjack [options] executable

Positional arguments:
executable              The executable you wish to launch

Optional arguments:
-h --help               shows help message and exits [default: false]
-v --version            prints version information and exits [default: false]
-q --quiet              Don't output any messages [default: false]
-d --working-dir        Set the working directory to launch
-l --dll                DLL file to be injected (multiple)
-e --env                Start proccess with environment variable (multiple)
```
## Injecting DLLs
Using the `--dll` or `-l` flag you can create the process with the specified DLL injected. You can use it multiple times.
```shell
blackjack prog.exe -l mod-menu.dll -l crash-logger.dll
```

## Environment Variables
Using the `--env` or `-e` flag you can create the process with custom environment variables. You can use it multiple times.
```shell
blackjack prog.exe -e EDITOR=nano -e SHIRT=cool
```
## Note about 32 bit vs 64 bit
Make sure that the executable and the DLLs you load have the same bitness!
# License
Blackjack is released under the MIT License