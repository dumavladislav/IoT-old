
call mvn clean
call mvn install

REM Copy files to server
ssh dumavla@192.168.1.53 "rm -f -r gps-agent && mkdir gps-agent"
scp -r /c/dumavla/GitHub/IoT/Backend/gps-agent/* dumavla@192.168.1.53:~/gps-agent


REM docker build
REM ssh -t dumavla@192.168.1.53 "sudo docker build -t dumskyhome/gps-agent:rev2 ~/gps-agent/."
REM ssh -t dumavla@192.168.1.53 "sudo docker run --name gps-agent-rev2  --restart unless-stopped -it -d dumskyhome/gps-agent:rev2

ssh -t dumavla@192.168.1.53 "sudo docker stop gps-agent-rev2 || true && sudo docker rm gps-agent-rev2 || true && sudo docker build -t dumskyhome/gps-agent:rev2 ~/gps-agent/. && sudo docker run --name gps-agent-rev2  --restart unless-stopped -it -d dumskyhome/gps-agent:rev2"