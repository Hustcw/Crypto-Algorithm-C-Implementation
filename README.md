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
Decryption succeeded!
(crypto) ➜  aes git:(main) ✗ python utils.py --mode validate --key 2b7e151628aed2a6abf7158809cf4f3c --iv 471f48486393fd769d85d7015c07856c
Encoding Success!
```

## SM4
reference: 
- http://gmssl.org/docs/sm4.html
- https://a-little-cat.github.io/2017/12/14/SM4.html
- https://drive.google.com/file/d/0B0o25hRlUdXcbzdjT0hrYkkwUjg/view
validate: https://pypi.org/project/gmssl/

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
Decryption succeeded!
➜  sm4 git:(main) ✗ python3 utils.py --mode validate --key 2b7e151628aed2a6abf7158809cf4f3c --iv d370143ed71bbc2f811d0f3b3e4da16a
Encryption Success!
```
## SM3 
reference: 
- http://gmssl.org/docs/sm3.html
- https://gitee.com/snowlandltd/snowland-smx-python
- SM3密码杂凑算法

- Usage:
```bash
python3 utils.py --mode generate
clang sm3.c -O3 -o sm3 && ./sm3
python3 utils.py --mode validate
```
- Output
```bash
(crypto) ➜  sm3 git:(main) ✗ python utils.py --mode generate
(crypto) ➜  sm3 git:(main) ✗ clang sm3.c -O3 -o sm3 && ./sm3 
Hash speed: 838.93 Mps # can be faster when data is larger
SM3: 7dbc2fd6 258d9dc1 4a3b314e 0fc363c7 eda3da7e 32e51d43 1b48c15a 5fd044a8 %                                                                                  
(crypto) ➜  sm3 git:(main) ✗ python utils.py --mode validate 
SM3 Hash Success!
(crypto) ➜  sm3 git:(main) ✗
```
## SHA-3
reference: https://github.com/brainhub/SHA3IUF/blob/master/sha3.c