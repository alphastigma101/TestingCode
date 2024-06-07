# Asked ChatGpt to give me 20 hardest assignments in c/c++
1. **Triple Pointer Magic:**
   Write a function that accepts a triple pointer to an integer and modifies the value it points to.

2. **Memory Allocator:**
   Implement a custom memory allocator using malloc and free functions.

3. **Abstracted Geometry Classes:**
   Design a set of abstract classes representing geometric shapes like circles, rectangles, and triangles, and implement concrete subclasses for each.

4. **Static Singleton:**
   Implement a thread-safe singleton class using the static keyword.

5. **External Sorting:**
   Write a program to perform external sorting on a large dataset stored in a file, utilizing the extern keyword to manage memory efficiently.

6. **Struct-Based Database:**
   Create a simple in-memory database using structs to represent tables, rows, and columns, with functions to insert, delete, and query data.

7. **Graph Algorithms:**
   Implement graph algorithms such as Dijkstra's algorithm or Bellman-Ford algorithm to find shortest paths between nodes in a weighted graph.

8. **Parallel Sorting:**
   Develop a parallel sorting algorithm using threads or OpenMP to improve the efficiency of sorting large arrays.

9. **Database Indexing:**
   Design and implement a B-tree data structure to index a database table for efficient search and retrieval operations.

10. **Network Protocol Parser:**
    Write a program to parse network packets of a specific protocol using structs to represent packet headers and data fields.

11. **Image Processing:**
    Create a program to perform basic image processing tasks like convolution, edge detection, or histogram equalization using memory manipulation techniques.

12. **Custom STL Container:**
    Implement a custom container similar to std::vector or std::map with support for dynamic resizing, iterators, and generic element types.

13. **Game AI:**
    Develop an AI for a simple game (e.g., Tic-Tac-Toe or Chess) using algorithms like minimax with alpha-beta pruning or Monte Carlo Tree Search.

14. **Dynamic Code Generation:**
    Write a program that dynamically generates machine code at runtime to perform a specific computation, leveraging the flexibility of function pointers.

15. **Operating System Kernel Module:**
    Create a kernel module for a Unix-like operating system to add a new system call or device driver, requiring knowledge of kernel-level programming and memory management.

16. **Template Metaprogramming:**
    Explore template metaprogramming techniques to implement compile-time computations or optimizations, such as calculating factorials or generating Fibonacci sequences.

17. **Network Routing Algorithm:**
    Design and implement a routing algorithm for a computer network, considering factors like latency, bandwidth, and network topology.

18. **Blockchain Implementation:**
    Develop a simplified version of a blockchain system, including data structures for blocks, transactions, and a consensus mechanism like Proof of Work.

19. **Compiler Frontend:**
    Build a basic compiler frontend to parse and analyze source code written in a custom programming language, generating intermediate representations for further processing.

20. **Quantum Computing Simulation:**
    Create a simulation of a quantum computing system using C/C++ to model quantum gates, qubits, and algorithms like Shor's algorithm or Grover's algorithm.
##
# Asked chat gpt to give me the top 20 hardest cryptography 
##
elow are stub functions for implementing 20 challenging cryptographic algorithms in C:

AES (Advanced Encryption Standard)
c
Copy code
#include <stdint.h>

void aes_encrypt(const uint8_t *plaintext, const uint8_t *key, uint8_t *ciphertext);
void aes_decrypt(const uint8_t *ciphertext, const uint8_t *key, uint8_t *plaintext);
RSA (Rivest-Shamir-Adleman)
c
Copy code
#include <openssl/rsa.h>

void rsa_encrypt(const uint8_t *plaintext, size_t plaintext_len, RSA *key, uint8_t *ciphertext);
void rsa_decrypt(const uint8_t *ciphertext, size_t ciphertext_len, RSA *key, uint8_t *plaintext);
SHA-256 (Secure Hash Algorithm 256-bit)
c
Copy code
#include <openssl/sha.h>

void sha256(const uint8_t *data, size_t len, uint8_t *hash);
Elliptic Curve Cryptography (ECC)
c
Copy code
#include <openssl/ec.h>

void ecc_encrypt(const uint8_t *plaintext, size_t plaintext_len, EC_KEY *key, uint8_t *ciphertext);
void ecc_decrypt(const uint8_t *ciphertext, size_t ciphertext_len, EC_KEY *key, uint8_t *plaintext);
Blowfish
c
Copy code
#include <openssl/blowfish.h>

void blowfish_encrypt(const uint8_t *plaintext, size_t len, const uint8_t *key, uint8_t *ciphertext);
void blowfish_decrypt(const uint8_t *ciphertext, size_t len, const uint8_t *key, uint8_t *plaintext);
Twofish
c
Copy code
#include <twofish.h>

void twofish_encrypt(const uint8_t *plaintext, size_t len, const uint8_t *key, uint8_t *ciphertext);
void twofish_decrypt(const uint8_t *ciphertext, size_t len, const uint8_t *key, uint8_t *plaintext);
Camellia
c
Copy code
#include <openssl/camellia.h>

void camellia_encrypt(const uint8_t *plaintext, size_t len, const uint8_t *key, uint8_t *ciphertext);
void camellia_decrypt(const uint8_t *ciphertext, size_t len, const uint8_t *key, uint8_t *plaintext);
Chaos-based Cryptography
c
Copy code
void chaos_encrypt(const uint8_t *plaintext, size_t len, const uint8_t *key, uint8_t *ciphertext);
void chaos_decrypt(const uint8_t *ciphertext, size_t len, const uint8_t *key, uint8_t *plaintext);
McEliece Cryptosystem
c
Copy code
void mceliece_encrypt(const uint8_t *plaintext, size_t len, const uint8_t *key, uint8_t *ciphertext);
void mceliece_decrypt(const uint8_t *ciphertext, size_t len, const uint8_t *key, uint8_t *plaintext);
Serpent
c
Copy code
#include <openssl/serpent.h>

void serpent_encrypt(const uint8_t *plaintext, size_t len, const uint8_t *key, uint8_t *ciphertext);
void serpent_decrypt(const uint8_t *ciphertext, size_t len, const uint8_t *key, uint8_t *plaintext);
RC4 (Rivest Cipher 4)
c
Copy code
#include <openssl/rc4.h>

void rc4_encrypt(const uint8_t *plaintext, size_t len, const uint8_t *key, uint8_t *ciphertext);
void rc4_decrypt(const uint8_t *ciphertext, size_t len, const uint8_t *key, uint8_t *plaintext);
MD5 (Message Digest Algorithm 5)
c
Copy code
#include <openssl/md5.h>

void md5(const uint8_t *data, size_t len, uint8_t *hash);
Curve25519
c
Copy code
#include <openssl/curve25519.h>

void curve25519_key_exchange(const uint8_t *private_key, const uint8_t *public_key, uint8_t *shared_secret);
Diffie-Hellman Key Exchange
c
Copy code
#include <openssl/dh.h>

void dh_key_exchange(const uint8_t *public_key, size_t pub_len, const uint8_t *private_key, uint8_t *shared_secret);
HMAC (Hash-based Message Authentication Code)
c
Copy code
#include <openssl/hmac.h>

void hmac(const uint8_t *data, size_t len, const uint8_t *key, size_t key_len, uint8_t *hash);
Whirlpool
c
Copy code
#include <openssl/whrlpool.h>

void whirlpool(const uint8_t *data, size_t len, uint8_t *hash);
GOST Cryptography
c
Copy code
#include <openssl/gost.h>

void gost_encrypt(const uint8_t *plaintext, size_t len, const uint8_t *key, uint8_t *ciphertext);
void gost_decrypt(const uint8_t *ciphertext, size_t len, const uint8_t *key, uint8_t *plaintext);
RIPEMD-160
c
Copy code
#include <openssl/ripemd.h>

void ripemd160(const uint8_t *data, size_t len, uint8_t *hash);
DSA (Digital Signature Algorithm)
c
Copy code
#include <openssl/dsa.h>

void dsa_sign(const uint8_t *message, size_t len, DSA *key, uint8_t *signature);
int dsa_verify(const uint8_t *message, size_t len, const uint8_t *signature, size_t sig_len, DSA *key);
Bcrypt
c
Copy code
#include <bcrypt.h>

void bcrypt_hash(const char *password, const char *salt, uint8_t *hash);
##
# Asked chatgpt to give me the top 20 hardest subsitution ciphers
##
Hill Cipher: A polygraphic substitution cipher based on linear algebra.
Playfair Cipher: A digraph substitution cipher where pairs of letters are encrypted based on their positions in a key matrix.
Bifid Cipher: A fractionation cipher where each letter of the plaintext is first converted to its coordinates in a grid before being encrypted.
Trifid Cipher: An extension of the Bifid cipher where each letter is first converted to its coordinates in a 3D grid.
ADFGVX Cipher: A transposition and fractionation cipher involving a 6x6 grid of letters (ADFGVX) and a keyword for columnar transposition.
Amsco Cipher: A columnar transposition cipher where the columns have different lengths.
Straddle Checkerboard Cipher: A variation of the Playfair cipher with additional digits or symbols.
Grille Cipher: A paper-and-pencil cipher where a sheet with holes is placed over the plaintext to reveal the ciphertext.
Nihilist Cipher: Similar to the Playfair cipher but includes a numeric key to generate the grid.
Patristocrat Cipher: A Playfair-like cipher where the key matrix is generated from a keyword and padding letters.
Running Key Cipher: A variant of the Vigenère cipher where the key is as long as the plaintext and is not repeated.
Gronsfeld Cipher: Similar to the Vigenère cipher but uses a numeric key instead of a keyword.
Beaufort Cipher: A variant of the Vigenère cipher where encryption is performed by subtracting the key letter from the plaintext letter.
Porta Cipher: A variant of the Vigenère cipher with different alphabets for encryption and decryption.
Gromark Cipher: A variant of the Vigenère cipher where the keyword determines the length of the plaintext chunks.
Chaocipher: A cipher with dynamic alphabets that change with each character encrypted.
One-Time Pad: A theoretically unbreakable cipher where each plaintext character is encrypted using a unique, randomly generated key.
Beale Cipher: A form of book cipher where the key is a passage from a book.
ADFGX Cipher: Similar to the ADFGVX cipher but with only two letters (ADFGX) instead of six.
Zigzag Cipher: A simple transposition cipher where the plaintext is written diagonally on a grid and then read off row by row.

##
# Asked chatgpt to create 20 advance assignments in HDL/VHDL
##
1. **Sequential Circuit Design:**
   Design a sequential circuit using VHDL to implement a finite state machine (FSM) with specific behavior or functionality.

2. **Memory Controller Implementation:**
   Implement a memory controller module in VHDL for interfacing with external memory devices such as RAM or ROM.

3. **UART Communication Module:**
   Develop a UART (Universal Asynchronous Receiver-Transmitter) communication module in VHDL for serial communication between devices.

4. **Floating Point Arithmetic Unit:**
   Design and implement a floating-point arithmetic unit in VHDL for performing operations like addition, subtraction, multiplication, and division.

5. **Digital Filter Design:**
   Create a digital filter using VHDL to process and filter digital signals, such as low-pass, high-pass, or band-pass filters.

6. **SPI Interface Controller:**
   Design an SPI (Serial Peripheral Interface) interface controller module in VHDL for communication with SPI-compatible devices.

7. **Processor Design:**
   Develop a simple microprocessor or CPU core architecture in VHDL, including instruction set architecture (ISA) and control unit.

8. **FPGA-Based Accelerator:**
   Implement a hardware accelerator module in VHDL to offload computationally intensive tasks from a CPU to an FPGA (Field-Programmable Gate Array).

9. **Real-Time Clock Module:**
   Design a real-time clock (RTC) module in VHDL to keep track of time and provide time-related functionalities in digital systems.

10. **Ethernet MAC Layer:**
    Create an Ethernet Media Access Control (MAC) layer module in VHDL for Ethernet communication and packet handling.

11. **Signal Processing Module:**
    Develop a signal processing module in VHDL for performing operations like FFT (Fast Fourier Transform) or DFT (Discrete Fourier Transform) on digital signals.

12. **Video Codec Implementation:**
    Implement a video codec module in VHDL for encoding and decoding digital video streams, supporting formats like H.264 or MPEG.

13. **UART-to-USB Converter:**
    Design a UART-to-USB converter module in VHDL to enable communication between UART-based devices and USB hosts.

14. **Digital PLL Design:**
    Develop a digital Phase-Locked Loop (PLL) module in VHDL for generating stable clock signals and frequency synthesis.

15. **High-Speed SerDes Interface:**
    Create a High-Speed Serializer/Deserializer (SerDes) interface module in VHDL for high-speed data transmission between chips or boards.

16. **FIFO Buffer Implementation:**
    Implement a First-In-First-Out (FIFO) buffer module in VHDL for data buffering and synchronization in digital systems.

17. **Audio Processing Unit:**
    Design an audio processing unit in VHDL for processing digital audio signals, including operations like filtering, equalization, and audio effects.

18. **PCIe Controller Core:**
    Develop a PCIe (Peripheral Component Interconnect Express) controller core in VHDL for PCIe communication and data transfer between devices.

19. **Wireless Communication Module:**
    Create a wireless communication module in VHDL for implementing wireless protocols like Bluetooth, Wi-Fi, or Zigbee.

20. **ASIC Verification Testbench:**
    Build an ASIC (Application-Specific Integrated Circuit) verification testbench in VHDL for verifying the functionality and performance of ASIC designs.
###
# Asked Chatgpt to give me the top 20 hardest Assembly challenges: 
###
1. Implementing a Virtual Machine: Write an assembly program to emulate a simple virtual machine with its own instruction set architecture (ISA).

2. Custom Operating System Kernel: Develop a basic operating system kernel in assembly language with features like multitasking, memory management, and file system support.

3. Optimized Matrix Multiplication: Write assembly code to perform matrix multiplication with optimizations for cache usage and SIMD (Single Instruction, Multiple Data) instructions.

4. Implementing a Compiler: Create an assembly program that translates a high-level language (like C or Pascal) into machine code.

5. Cryptography Algorithms in Assembly: Implement cryptographic algorithms like AES, SHA-256, or RSA in assembly language for maximum efficiency.

6. Low-Level Device Driver: Write an assembly program to interface with a hardware device directly, such as a network card or graphics card.

7. Memory Allocator: Develop a custom memory allocator in assembly language with features like dynamic memory allocation and deallocation.

8. Signal Processing Algorithms: Implement signal processing algorithms (e.g., FFT or FIR filters) in assembly for real-time processing efficiency.

9. Custom File System: Create a custom file system in assembly language with features like file allocation tables (FAT) or indexed allocation.

10. Game Engine Components: Write assembly code for critical components of a game engine, such as collision detection, physics calculations, or rendering pipelines.

11. Implementing an Interpreter: Develop an assembly language interpreter for a scripting language like Python or JavaScript.

12. Real-Time Operating System (RTOS): Create a real-time operating system kernel in assembly language for embedded systems or IoT devices.

13. Data Compression Algorithms: Implement data compression algorithms like Lempel-Ziv-Welch (LZW) or Huffman coding in assembly language for maximum compression efficiency.

14. Custom CPU Emulator: Develop an emulator for a custom CPU architecture in assembly language, including instruction decoding and execution.

15. Graphics Rendering Pipeline: Write assembly code for a graphics rendering pipeline, including vertex transformations, rasterization, and pixel shading.

16. Audio Processing Algorithms: Implement audio processing algorithms like resampling or audio effects (e.g., reverb or equalization) in assembly language.

17. Digital Signal Processing (DSP): Write assembly code for digital signal processing tasks like noise reduction, echo cancellation, or speech recognition.

18. Optimizing Compiler Optimizations: Develop assembly code to perform compiler optimizations such as loop unrolling, function inlining, or register allocation.

19. Binary Analysis and Reverse Engineering Tools: Create assembly-based tools for analyzing binary executables, such as disassemblers, debuggers, or binary patching utilities.

20. Custom Networking Protocol Stack: Implement a custom networking protocol stack in assembly language, including layers like Ethernet, IP, TCP, and UDP.

##
# Misc:
##

* - Sources:
	* **playing with certs:**
		- *https://www.imperialviolet.org/2013/08/01/ctpilot.html*

