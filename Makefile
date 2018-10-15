
.DEFAULT_GOAL := run

docker-build: 
	docker-compose build

build: docker-build
	docker-compose up

run: build
	./build/run
