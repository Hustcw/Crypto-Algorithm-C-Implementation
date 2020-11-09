def generate():
    fp = open('input.txt', 'w')
    message = 'this_is_a_test!!'
    assert len(message) == 16
    NUM = 1024
    fp.write(message*NUM)
    fp.close()


''' test result '''
from Crypto.Cipher import AES
def validate(key, iv):
    key = bytes.fromhex(key)
    iv = bytes.fromhex(iv)
    def do_validate():
        msg = open("input.txt", 'rb').read()
        aes = AES.new(key, AES.MODE_CBC, iv)
        cipher = aes.encrypt(msg)
        return cipher

    ground_truth = do_validate()
    predict = open("output.txt", 'rb').read()
    if predict == ground_truth:
        print("Encoding Success!")
    else:
        print("Encoding Failure!")

import argparse

parser = argparse.ArgumentParser(description='Process Operation')

parser.add_argument("--mode", help="work mode", type=str)
parser.add_argument("--key", help="work mode", type=str)
parser.add_argument("--iv", help="work mode", type=str)

args = parser.parse_args()

if args.mode == 'validate':
    validate(args.key.strip(), args.iv.strip())
elif args.mode == 'generate':
    generate()
