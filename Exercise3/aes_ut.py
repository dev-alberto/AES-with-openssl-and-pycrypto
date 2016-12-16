import base64
from Crypto.Cipher import AES

#padding is not done by default
pad = lambda s: s + (16 - len(s) % 16) * chr(16 - len(s) % 16)
unpad = lambda s: s[0:-ord(s[-1])]


class AESCipher:
    """ helper class, used to encrypt and decrypt data"""
    def __init__(self, key, iv, mode):
        self.key = key
        self.iv = iv
        self.mode = mode

    def return_cypher_type(self):
        """ returns type of instance needed for encryption or decription"""
        if self.mode == 'CFB':
            return AES.new(self.key, AES.MODE_CFB, self.iv)
        elif self.mode == 'ECB':
            return AES.new(self.key, AES.MODE_ECB)
        else:
            return None

    def encrypt(self, raw):
        raw = pad(raw)
        cipher = self.return_cypher_type()
        return base64.b64encode(self.iv + cipher.encrypt(raw))
       # return base64.b64encode(cipher.encrypt(raw))
        #return cipher.encrypt(pad(raw)

    def decrypt(self, enc):
        enc = base64.b64decode(enc)
        cipher = self.return_cypher_type()
        return unpad(cipher.decrypt(enc[16:]))
