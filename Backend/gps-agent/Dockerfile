FROM openjdk:8-jdk-alpine
#RUN addgroup -S spring && adduser -S spring -G spring
#USER spring:spring

COPY ./target/ /usr/src/gps-agent/
WORKDIR /usr/src/gps-agent

RUN apk add tzdata
RUN cp /usr/share/zoneinfo/Europe/Moscow /etc/localtime
RUN echo "Europe/Moscow" >  /etc/timezone

ENTRYPOINT ["java", "-jar", "./gps-agent-0.0.1-SNAPSHOT.jar"]