# coding: utf-8

import sys
import socket
import contextlib
import threading

def loop(conn):
    size = 4096

    with contextlib.closing(conn):
        while True:
            buf = conn.recv(size)
            if not buf:
                break
            conn.sendall(buf)

    print("disconnected")

def main(port):
    host = '127.0.0.1'
    backlog = 10

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    with contextlib.closing(sock):
        sock.bind((host, port))
        sock.listen(backlog)

        try:
            while True:
                conn, address = sock.accept()
                print("connected")
                threading.Thread(target=loop, args=(conn,)).start()
        except KeyboardInterrupt:
            sys.stderr.write("[End]\n")

if __name__ == '__main__':
    if len(sys.argv) < 2:
        sys.stderr.write("few arguments\n")
        sys.exit(1)
    else:
        main(int(sys.argv[1]))
