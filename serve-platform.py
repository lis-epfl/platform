#! /usr/bin/python3
import socket
import sys
import subprocess
import os

HOST = ''
PORT = 12345
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen(1)
    while True:
        conn, addr = s.accept()
        with conn:
            while True:
                data = conn.recv(1024)
                if not data: break
                dir_path = os.path.dirname(os.path.realpath(__file__))
                p = subprocess.Popen(['{}/platform'.format(dir_path)] + data.decode('utf-8').split(' '), stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
                while p.returncode is None:
                    try:
                        outs, errs = p.communicate(timeout=1)
                        if len(outs) > 0:
                            conn.sendall('\n'.join(["1:{}".format(l) for l in outs.split('\n')]).encode('utf-8'))
                        if len(errs) > 0:
                            conn.sendall('\n'.join(["2:{}".format(l) for l in errs.split('\n')]).encode('utf-8'))
                    except subprocess.TimeoutExpired:
                        pass
                break
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    connection = None
