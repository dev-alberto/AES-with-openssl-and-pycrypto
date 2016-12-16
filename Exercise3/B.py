import socket
import aes_ut
import globals
import base64

def get_key_from_KM():
    """Establish connection and receive encrypted key, than decrypt it and return the result"""
    connection = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    connection.connect(("127.0.0.1", 1234))
    connection.send(globals.mode.encode())

    key = connection.recv(10000)
    key_decript = aes_ut.AESCipher(globals.K3, globals.iv, globals.mode)
    key = key_decript.decrypt(key)
    connection.close()
    return key


if __name__ == '__main__':
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(("127.0.0.1", 1235))
    s.listen(1)

    while True:
        #receive encryption type from node A
        (connection_to_A, A_address) = s.accept()
        connection_to_A.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
        encryption_type = connection_to_A.recv(100).decode()
        print encryption_type

        #send to KM encryption type and receive back encrypted key, proceed than to decode said key
        key = get_key_from_KM()
        print key

        #give the 'ok', communication can indeed start
        connection_to_A.send("ack".encode())
        while True:
            text = connection_to_A.recv(64)  # .decode('utf8')
            #print text
            if not text:
                break
            aes = aes_ut.AESCipher(key, globals.iv, globals.mode)
            block = aes.decrypt(text)
            print block
