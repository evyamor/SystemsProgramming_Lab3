all:exec

exec: bcmp.c 
	 gcc -g -m32 -c -o bcmp.o bcmp.c
	gcc -g -m32 bcmp.o -o bcmp
	 rm bcmp.o

.PHONY: clean
clean:
	 rm -rf ./*.o bcmp

