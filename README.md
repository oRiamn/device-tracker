# Device Tracker

> Node.js xinput api implement for linux

Besides allowing raw access to the xinput device, it also provides a small wrapper class which emits events based on controller input.

## Requirements

[xinput](https://packages.debian.org/search?keywords=xinput) for linux.
[Docker](https://docs.docker.com/install/) for building application.

## Usage 

Clone the project.

For identify your device id folow this command :

```BASH
xinput --list
```

Edit **./MakeFile** for define input identifiers to watch. 
You can use *-w* option multiple times.
For example : 
```BASH
./build/run -w ID1 -w ID2
```

After that build and run with this command : 

```BASH
make run
```