# Spirit Catcher (Color)

An action platformer for the Ninteno GameBoy Color written using [CrossZGB](https://github.com/gbdk-2020/CrossZGB/releases)

## Building amd Running

_Note: although the project should compile on Windows/Linux it has been devloped on WSL._

Download CrossZGB from [https://github.com/gbdk-2020/CrossZGB/releases](https://github.com/gbdk-2020/CrossZGB/releases) 
and extract to a directory, e.g.  `/usr/local/lib/CrossZGB`.

Then execute the following :

```
./setenv.sh
./build.sh
```

The ROM will be in the `bin\gbc` directory.

You can install the ROM created above in any Emulator, for testing development is is recommend to use
(Emulicious)[https://emulicious.net/]. First install a Java JDK, e.g.

```
apt install openjdk-17-jdk
```

Then extract Emulicious to a directory in the CrossZGB installation, e.g.  `/usr/local/lib/CrossZGB/env/Emulicious`.
There is an example `run.sh` which you can then use to rum the ROM in Emulicious.

----

Kevin A. Lee (kadraman)