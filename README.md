# RT_SOFT
1. make
2. Вставить модуль: 
insmod drv.ko
3. Посмотреть в /proc/devices, какой у драйвера chr_drv major_number
4. Создать файл устройства:
mknod /dev/drv c major_number 0
5. Для сборки теста:
gcc -o test test.c
