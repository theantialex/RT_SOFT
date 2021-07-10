# RT_SOFT
Kafka + Docker: broker + producer + consumer  

Команды:  
chxmod +x start.sh  
./start.sh

start.sh:
1) Запускает Kafka в Docker
2) Cоздает topic kafka-1
3) Cоздает consumer, который подписан на этот topic. Пришедшие сообщения перенаправляются в файл topic-kafka-1.txt
4) В открывшимся теминале создается producer - сообщения, которые введете в терминал попадут в topic kafka-1 (и, соответсвенно в файл  topic-kafka-1.txt)

