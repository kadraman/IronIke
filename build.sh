#!/bin/sh

cd src
make gbc
cd ..
java -jar $ZGB_HOME/ZGB/tools/Emulicious/Emulicious.jar bin/gbc/IronIke.gbc
