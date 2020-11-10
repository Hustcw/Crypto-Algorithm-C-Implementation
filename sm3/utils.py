from gmssl import sm3, func

def generate():
    fp = open('input.txt', 'w')
    message = 'this_is_a_test!!'
    assert len(message) == 16
    NUM = 1024 * 1024
    fp.write(message*NUM)
    fp.close()

def validate():
    def do_validate():
        msg = open("input.txt", 'rb').read()
        hashed = sm3.sm3_hash(func.bytes_to_list(msg))
        return hashed
    ground_truth = open("output.txt", 'rb').read()
    if do_validate() == ground_truth:
        print("SM3 Hash Success!")
    else:
        print(do_validate().hex())
        print(ground_truth.hex())
        print("SM3 Hash Failure!")


import argparse
parser = argparse.ArgumentParser(description='Process Operation')
parser.add_argument("--mode", help="work mode", type=str)

args = parser.parse_args()

if args.mode == 'validate':
    validate(args.key.strip(), args.iv.strip())
elif args.mode == 'generate':
    generate()
