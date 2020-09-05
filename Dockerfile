FROM php:7.3-fpm
WORKDIR /var/www/html

RUN apt-get -o Acquire::Check-Valid-Until=false update &&\
	apt-get -y install nginx &&\
	apt-get clean

EXPOSE 80

COPY ./wrapper.sh /
CMD /wrapper.sh