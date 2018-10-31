
.DEFAULT_GOAL := run

containers    := $$(docker ps -f label=device.tracker -q)
images        := $$(docker images -f label=device.tracker -q)

docker-build: 
	docker build -t device-tracker-builder . --label device.tracker=true

build: docker-build
	docker-compose up

run: build
	# define identifiers with -w option
	./build/run -w 15 -w 16
clean:
	docker stop $(containers) || true
	docker rm -f $(containers) || true
	docker rmi -f $(images) || true
	
