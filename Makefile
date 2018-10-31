
.DEFAULT_GOAL := run

docker-build: 
	docker build -t device-tracker-builder . --label device.tracker=true

build: docker-build
	docker-compose up

run: build
	# define identifiers with -w option
	./build/run -w 15 -w 16
clean:
	docker stop $(docker ps -f label=device.tracker -q) || true
	docker rm $(docker ps -f label=device.tracker -q) || true
	docker rmi $(docker images -f label=device.tracker -q) || true
