
call mvn clean
call mvn install

REM Copy files to server
ssh dumavla@192.168.1.53 "rm -f -r authorization-service && mkdir authorization-service"
scp -r /c/dumavla/GitHub/IoT/Backend/authorization-service/* dumavla@192.168.1.53:~/authorization-service


REM docker build
REM ssh -t dumavla@192.168.1.53 "sudo docker build -t dumskyhome/authorization-service:rev2 ~/authorization-service/."
REM ssh -t dumavla@192.168.1.53 "sudo docker run --name authorization-service-rev2  --restart unless-stopped -it -d dumskyhome/authorization-service:rev2

ssh -t dumavla@192.168.1.53 "sudo docker stop authorization-service-rev2 && sudo docker rm authorization-service-rev2 && sudo docker build -t dumskyhome/authorization-service:rev2 ~/authorization-service/. && sudo docker run --name authorization-service-rev2  --restart unless-stopped -it -d dumskyhome/authorization-service:rev2"