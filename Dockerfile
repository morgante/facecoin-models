FROM dockerfile/python

# Start app dir
RUN 		mkdir /app

RUN 		apt-get update
RUN 		apt-get install -y imagemagick --fix-missing

# Add requirements
ADD 		./requirements.txt /requirements.txt
RUN 		pip install -r /requirements.txt

# Load app dependencies
ADD 		. /app/src
RUN 		ls /usr/local

# Make it
WORKDIR 	/app/src
RUN 		make
RUN 		mv anneal /app/anneal
RUN 		chmod 777 /app/anneal
RUN 		/app/anneal /app/src/face.png /app/src/btc.png face.obj

# Server
EXPOSE 		5000

# Run it
WORKDIR		/app/src

CMD ["python server.py"]