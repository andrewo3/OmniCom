import socket, threading, pygame,time
import numpy as np
import sys
from os.path import abspath
sys.path.append(abspath("build/lib.macosx-10.9-universal2-cpython-312"))
sys.path.append(abspath("build\\lib.win-amd64-cpython-38"))
import pyNES

sock = socket.socket()
sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 2**15)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, 4096)
sock.bind(('127.0.0.1',6502))
sock.listen(4)
connections = []

def cThread():
    global connections
    connected = True
    fails = 0
    recv = b'\0'
    window_dim = [256,240]
    pygame.init()
    window = pygame.display.set_mode(window_dim,pygame.RESIZABLE)
    nes_surf = pygame.Surface((240,256))
    #--socket stuff
    frame = b''
    recv_amt = 2**17
    while True:
        remove = []
        sendback = bytearray()
        for conn in connections:
            s = conn[0]
            address = conn[1]
            frame = bytearray()
            total_bytes = 256 * 240 * 3
            
            recv_data = None
            # Receive data in larger chunks
            while len(frame) < total_bytes:
                remaining_bytes = total_bytes - len(frame)
                recv_size = min(remaining_bytes, recv_amt)  # Adjust buffer size as needed
                recv_data = s.recv(recv_size)
                if not recv_data:
                    break  # No more data to receive
                frame.extend(recv_data)
            if not recv_data:
                remove.append(conn)
        for conn in remove:
            conn[0].close()
            print("connection with",conn[1],"closed.")
        
ct = threading.Thread(target = cThread,daemon = True)
ct.start()

def accept_conns():       
    while True:
        (client,address) = sock.accept()
        connections.append((client,address))
        print("Received connection from",address)
        

acc_thread = threading.Thread(target=accept_conns,daemon=True)
acc_thread.start()
while True:
    pass