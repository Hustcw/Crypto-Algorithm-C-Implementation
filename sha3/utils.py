from hashlib import sha3_256

def generate():
    fp = open('input.txt', 'w')
    message = 'this_is_a_test!!'
    assert len(message) == 16
    NUM = 1024
    fp.write(message*NUM)
    fp.close()

def validate():
    def do_validate():
        msg = open("input.txt", 'rb').read()
        hashed = sha3_256(msg).digest() # too too too slow!
        return hashed
    ground_truth = open("output.txt", 'rb').read()
    if do_validate() == ground_truth:
        print("SHA3 Hash Success!")
    else:
        print(do_validate().encode('hex'))
        print(ground_truth.encode('hex'))
        print("SHA3 Hash Failure!")


import argparse
parser = argparse.ArgumentParser(description='Process Operation')
parser.add_argument("--mode", help="work mode", type=str)

args = parser.parse_args()

if args.mode == 'validate':
    validate()
elif args.mode == 'generate':
    generate()
