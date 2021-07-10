#!/bin/sh

cd / && git clone https://github.com/wurstmeister/kafka-docker ~/kafka-docker
cp docker-compose-single-broker.yml ~/kafka-docker/docker-compose-single-broker.yml &

cd ~/kafka-docker && docker-compose -f docker-compose-single-broker.yml up &

docker exec -t kafka-docker_kafka_1 \
  kafka-topics.sh \
    --bootstrap-server :9092 \
    --create \
    --topic kafka-1 \
    --partitions 3 \
    --replication-factor 1 &

docker exec -t kafka-docker_kafka_1 kafka-console-consumer.sh --bootstrap-server localhost:9092 --topic kafka-1 > topic_kafka-1.txt & 

docker exec -it kafka-docker_kafka_1 kafka-console-producer.sh --broker-list localhost:9092 --topic kafka-1


