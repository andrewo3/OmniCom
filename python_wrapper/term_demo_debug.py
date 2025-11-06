import socket, threading

sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
sock.bind(('0.0.0.0',1234))
sock.listen(5)

def process_outputs(client):
    buffer = ""
    while True:
        while not '\n' in buffer:
            data = client.recv(1024)
            if not data:
                print("[closed.]")
                client.close()
                return
            buffer += data.decode('utf-8')
        out, buffer = buffer.split("\n",1)
        print(out)
        

while True:
    (client,address) = sock.accept()
    print("[started.]")
    proc_thread = threading.Thread(target=process_outputs,args=[client])
    proc_thread.start()
    