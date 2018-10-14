FROM alpine:3.8
RUN apk add --update alpine-sdk
RUN apk add --no-cache xinput libxi-dev
COPY src /app/
WORKDIR /app/src

CMD make
