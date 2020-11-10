# Crypto-Algorithm-C-Implementation

## AES
reference: https://github.com/kokke/tiny-AES-c/blob/master/aes.c

- Usage:
```bash
python utils.py --mode generate # generate input data for testing
clang aes.c -O3 -o aes  && ./aes # compile
python utils.py --mode validate --key [key_in_hex] --iv [iv_in_hex] # validate the correctness of implementation
```
- Output like this:
```bash
(crypto) ➜  aes git:(main) ✗ python utils.py --mode generate 
(crypto) ➜  aes git:(main) ✗ ./aes
key: 2b7e151628aed2a6abf7158809cf4f3c
 iv: 471f48486393fd769d85d7015c07856c
Encryption speed: 354.11 Mps
decryption succeeded!
(crypto) ➜  aes git:(main) ✗ python utils.py --mode validate --key 2b7e151628aed2a6abf7158809cf4f3c --iv 471f48486393fd769d85d7015c07856c
Encoding Success!
```

## SM4
reference: http://gmssl.org/docs/sm4.html

- Usage:
```bash
python3 utils.py --mode generate
clang sm4.c -O3 -o sm4 && ./sm4
python3 utils.py --mode validate --key [key_in_hex] --iv [iv_in_hex]
```
- Output
```bash
➜  sm4 git:(main) ✗ python3 utils.py --mode generate
➜  sm4 git:(main) ✗ clang sm4.c -O3 -o sm4
➜  sm4 git:(main) ✗ ./sm4
key: 2b7e151628aed2a6abf7158809cf4f3c
 iv: d370143ed71bbc2f811d0f3b3e4da16a
Encryption speed: 482.63 Mps
decryption succeeded!
➜  sm4 git:(main) ✗ python3 utils.py --mode validate --key 2b7e151628aed2a6abf7158809cf4f3c --iv d370143ed71bbc2f811d0f3b3e4da16a
Encryption Success!
```
## SM3 
reference: http://gmssl.org/docs/sm3.html

## SHA-3
reference: https://github.com/brainhub/SHA3IUF/blob/master/sha3.c