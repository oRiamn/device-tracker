FROM debian:9-slim
RUN apt-get update && apt-get install -y \
    g++ make \
    libxi-dev
    
COPY src /app/
RUN mkdir /app/build/
WORKDIR /app/src

CMD make