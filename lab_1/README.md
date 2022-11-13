# Lab 1 - fork() demonstration

Demonstration of system call fork().  

fork() creates a new process by duplicating the calling process.
In the output you will see the [CURRENT] process. After calling fork(), [PARENT] and [CHILD] processes will be created.  

There are also handlers for SIGINT and SIGTERM signals. You can send a signal to the process by using the command:
```
kill -[SIGNAL] PID
```
or by using a task manager (e.g. htop).

## Build:
```
make build
```

## Run:
```
make run
```

## Clean:
```
make clean
```