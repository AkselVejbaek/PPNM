SDK = /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk

Out.txt: hello
	./hello > Out.txt

hello: hello.cc
	c++ -isysroot $(SDK) -I$(SDK)/usr/include/c++/v1 -o hello hello.cc

clean:
	rm -f Out.txt hello