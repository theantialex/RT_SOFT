# RT_SOFT
Распознование объектов на видео + Docker  

Команды:  
$ sudo docker build -t open:v1 .  
$ sudo docker run --net=host --env="DISPLAY" --volume="$HOME/.Xauthority:/root/.Xauthority:rw" open:v1
