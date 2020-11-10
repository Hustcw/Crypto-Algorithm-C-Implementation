# Crypto-Algorithm-C-Implementation

## AES
reference: https://github.com/kokke/tiny-AES-c/blob/master/aes.c

- Usage:
```bash
python utils.py --mode generate # generate input date for testing
clang aes.c -O3 -o aes  && ./aes # compile
python utils.py --mode validate --key [key_in_hex] --iv [iv_in_hex]
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
reference: 
- http://gmssl.org/docs/sm4.html
- https://a-little-cat.github.io/2017/12/14/SM4.html
- https://drive.google.com/file/d/0B0o25hRlUdXcbzdjT0hrYkkwUjg/view
validate: https://pypi.org/project/gmssl/

## SM3 
reference: http://gmssl.org/docs/sm3.html

## SHA-3
reference: https://github.com/brainhub/SHA3IUF/blob/master/sha3.c