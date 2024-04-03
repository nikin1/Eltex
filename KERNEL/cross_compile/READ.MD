Для начала нужно было установить zlib и libressl:
Код для конфигурации zlib
- ![zlib](zlib.png)
Код для конфигурации libressl записать забыл, но добавлю скрин получившегося каталога
- ![libressl](libressl.png)

Далее собираю конфиг openssh.
- ![openssh_conf1](openssh_conf1.png)
config собрался 
- ![openssh](openssh.png)

Далее
- make
- make install 
Но после ввода команды make install однако еще выходили ошибки, так что до конечной сборки еще не дошло. 
- ![error](error.png)


На сайте находил похожую ошибку
https://stackoverflow.com/questions/45274853/error-cross-compiling-openssh-embedded-linux

Но добавление флага LDFLAGS вызвало ошибку еще в конфиге
- ![openssh_conf](openssh_conf.png)

Поэтому проблема пока еще открыта


