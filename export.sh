echo [$PWD]

export PATH=$PWD/xtensa-lx106-elf/bin:$PATH
echo 'PATH=' $PATH
export IDF_PATH=$PWD/ESP8266_RTOS_SDK
echo 'IDF_PATH=' $IDF_PATH

rm $PWD/xtensa-lx106-elf/bin/xtensa-lx106-elf-cc
ln -s xtensa-lx106-elf-gcc $PWD/xtensa-lx106-elf/bin/xtensa-lx106-elf-cc

