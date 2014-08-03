FROM dockerfile/python

# Start app dir
RUN 		mkdir /app

RUN 		apt-get install -y libeigen3-dev


# Load app dependencies
ADD 		. /app/src
RUN 		ls /usr/local/include

# Make it
WORKDIR 	/app/src
RUN 		make

