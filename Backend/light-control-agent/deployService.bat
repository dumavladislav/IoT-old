
call mvn clean
call mvn install

REM Copy files to server
ssh dumavla@192.168.1.53 "rm -f -r light-control-agent && mkdir light-control-agent"
scp -r /c/dumavla/GitHub/IoT/Backend/light-control-agent/* dumavla@192.168.1.53:~/light-control-agent


REM docker build
REM ssh -t dumavla@192.168.1.53 "sudo docker build -t dumskyhome/light-control-agent:rev2 ~/light-control-agent/."
REM ssh -t dumavla@192.168.1.53 "sudo docker run --name light-control-agent-rev2  --restart unless-stopped -it -d dumskyhome/light-control-agent:rev2

ssh -t dumavla@192.168.1.53 "sudo docker stop light-control-agent-rev2 || true && sudo docker rm light-control-agent-rev2 || true && sudo docker build -t dumskyhome/light-control-agent:rev2 ~/light-control-agent/. && sudo docker run --name light-control-agent-rev2  --restart unless-stopped -it -d dumskyhome/light-control-agent:rev2"