FROM ubuntu
COPY httpd /

RUN apt update &&\
	apt -y install python3

CMD ["/httpd"]
