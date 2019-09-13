# encode
small tool to do base64 and url encoding
## usage
`./bin/encode [OPTIONS]`

this tool does url and base64 encoding/decoding, and can read from stdin or from a file. default behavior is to read from stdin, and encode the input to base64. use the options to change the behavior.

## options
* `-f` filename to read as input, will read from stdin if not specified.
* `-d` decode input
* `-e` encode input
* `-s`scheme to use, either `url` or `b64`

## examples
```
$ ./bin/encode -f ./lib/libclass.a > encoded_lib.txt
$ cat encoded_lib.txt | ./bin/encode -d > ./lib/libclass.a
$ make clean && make
<... make prints some stuff, everything compiles fine>
```
the tool can handle null bytes, but only if they're read from a file.
