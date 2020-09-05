FROM php:7.3-fpm
WORKDIR /var/www/html

RUN apt-get -o Acquire::Check-Valid-Until=false update &&\
	apt-get -y install nginx &&\
	apt-get clean &&\
	docker-php-ext-install pdo_mysql mysqli
	
EXPOSE 80

COPY ./wrapper.sh /
CMD /wrapper.sh