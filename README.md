# RT_SOFT
Docker + Firefox  

Команды:  
$ sudo docker build -t firefox:v1 .  
$ sudo docker run --net=host --env="DISPLAY" --volume="$HOME/.Xauthority:/root/.Xauthority:rw" firefox:v1

