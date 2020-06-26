FROM openjdk:8-jdk-alpine
#RUN addgroup -S spring && adduser -S spring -G spring
#USER spring:spring

COPY ./target/ /usr/src/authorization-service/
WORKDIR /usr/src/authorization-service

RUN apk add tzdata
RUN cp /usr/share/zoneinfo/Europe/Moscow /etc/localtime
RUN echo "Europe/Moscow" >  /etc/timezone

ENTRYPOINT ["java", "-jar", "./authorization-service-0.0.1-SNAPSHOT.jar"]