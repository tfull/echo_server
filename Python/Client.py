# coding: utf-8

import sys
import socket
from contextlib import closing

def main(host, port):
    size = 4096

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    with closing(sock):
        sock.connect((host, port))
        try:
            while True:
                line = bytes(input(), "UTF-8")
                length = len(line)
                sock.sendall(line)
                l = 0
                line = b""
                while l < length:
                    tmp = sock.recv(size)
                    line += tmp
                    l += len(tmp)
                print(str(line, "UTF-8"))
        except EOFError:
            pass

if __name__ == '__main__':
    if len(sys.argv) < 3:
        sys.stderr.write("few arguments\n")
        sys.exit(1)
    else:
        main(sys.argv[1], int(sys.argv[2]))
