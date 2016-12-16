import socket
import aes_ut
import globals

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(("127.0.0.1", 1234))
s.listen(1)


def encrypt(key):
    return "encrypted " + key


while True:
    (connection, address) = s.accept()
    data = connection.recv(100).decode("UTF-8")
    #after a request for the type of encryption has been Received, send all encryped key, so that secure communication can commence
    if data == "ECB":
        print("Received: ", data)
        aes_ecb = aes_ut.AESCipher(globals.K3, globals.iv, "ECB")
        enc_key = aes_ecb.encrypt(globals.K1)
        connection.sendall(enc_key.encode())
    else:
        print("Received: ", data)
        aes_cfb = aes_ut.AESCipher(globals.K3, globals.iv, "CFB")
        enc_key = aes_cfb.encrypt(globals.K2)
        connection.sendall(enc_key.encode())
    connection.close()
