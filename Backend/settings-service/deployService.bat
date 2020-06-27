
call mvn clean
call mvn install

REM Copy files to server
ssh dumavla@192.168.1.53 "rm -f -r settings-service && mkdir settings-service"
scp -r /c/dumavla/GitHub/IoT/Backend/settings-service/* dumavla@192.168.1.53:~/settings-service


REM docker build
REM ssh -t dumavla@192.168.1.53 "sudo docker build -t dumskyhome/settings-service:rev2 ~/settings-service/."
REM ssh -t dumavla@192.168.1.53 "sudo docker run --name settings-service-rev2  --restart unless-stopped -it -d dumskyhome/settings-service:rev2

ssh -t dumavla@192.168.1.53 "sudo docker stop settings-service-rev2 || true && sudo docker rm settings-service-rev2 || true && sudo docker build -t dumskyhome/settings-service:rev2 ~/settings-service/. && sudo docker run --name settings-service-rev2  --restart unless-stopped -it -d dumskyhome/settings-service:rev2"