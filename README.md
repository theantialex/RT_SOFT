# RT_SOFT
Команды:
$ sudo docker build -t open:v1 .
$ sudo docker run --net=host --env="DISPLAY" --volume="$HOME/.Xauthority:/root/.Xauthority:rw" open:v1
