all:./ser ./service/fhttp/fhttp

./ser:./service/service.c
	gcc ./service/service.c -o ./ser

./service/fhttp/fhttp:./service/fhttp/fhttp.c
	gcc ./service/fhttp/fhttp.c -o ./service/fhttp/fhttp

clean:
	rm ./ser ./service/fhttp/fhttp
