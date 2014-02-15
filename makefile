TARGET=client
OBJS=client.o sendToServer.o
normal: $(TARGET)

client : client.c sendToServer.c defines.h sendToServer.h
	gcc -g -Wall sendToServer.c client.c -o client

clean:
	$(RM) $(TARGET)
	$(RM) $(OBJS)
