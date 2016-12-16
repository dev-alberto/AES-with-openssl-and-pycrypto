import socket
import globals
import base64

import aes_ut

key_size = 16


def get_key_from_KM():
    """send to KM the type of connection we want used, receive back encrypted key. Decrypt said key """
    connection = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    connection.connect(("127.0.0.1", 1234))
    connection.send(globals.mode.encode())

    #receive encryped key from KM
    key = connection.recv(10000)
    print key

    #decrypt key
    key_decript = aes_ut.AESCipher(globals.K3, globals.iv, globals.mode)
    key = key_decript.decrypt(key)

    connection.close()
    return key


if __name__ == '__main__':
    key = get_key_from_KM()
    print key

    conn_to_B = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    #send encryption type to node B
    conn_to_B.connect(("127.0.0.1", 1235))
    conn_to_B.send(globals.mode.encode())

    #receive from node B the 'ok' for communication start
    ack = conn_to_B.recv(100).decode()
    if ack == 'ack':
        #use contex manager to read file and send the encrypted blocks
        with open("text.txt", "r") as txt:
            while True:
                buff = txt.read(16)
                print buff
                if not buff:
                    break
                aes = aes_ut.AESCipher(key, globals.iv, globals.mode)
                block = aes.encrypt(buff)
                #block_16_bytes = base64.b16enode(block)
                print("Encrypted: ", block)
                conn_to_B.send(block)
    conn_to_B.close()

