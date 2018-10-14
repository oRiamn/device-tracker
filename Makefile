
docker-build: 
	docker-compose build

up: docker-build
	docker-compose up

exec: up
	./build/run
