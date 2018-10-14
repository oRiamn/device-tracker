FROM alpine:3.8
RUN apk add --no-cache \
    make g++ \
    libxi-dev
    
COPY src /app/
WORKDIR /app/src

CMD make
