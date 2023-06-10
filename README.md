# File Encryption and Decryption

This is a C++ code snippet that demonstrates file encryption and decryption using the OpenSSL library. It provides functions to encrypt 
and decrypt files using the AES-256-CBC encryption algorithm. This README will guide you on how to execute the encryption code on a Linux system.

## Prerequisites

Before running the code, ensure that you have the following prerequisites installed on your Linux system:

- OpenSSL library.
- C++ compiler (e.g., g++).
- Make sure you have read and write permissions for the files you want to encrypt/decrypt.

# Execution Steps

Follow these steps to execute the encryption code on your Linux system:

1. Clone the repository or copy the encryption.cpp file to your local machine.

- `git clone https://github.com/AndresNeron/database-encryption.git`

2. Open a terminal and navigate to the directory containing the encryption.cpp file.

- `cd encryption`

3. Compress the files you want to encrypt into a single zip file. Replace file1.txt, file2.txt, etc., with the actual names of the files.

- 'zip -r files.zip file1.txt file2.txt'

In the current example, I downloaded some databases from `https://data.gov` and I zip it to encrypt it.

4. Open the encryption.cpp file in a text editor and make sure to modify the following variables according to your needs:

- const char* inputFile - Path to the input file you want to encrypt/decrypt.
- const char* outputFile - Path to the output file where the encrypted/decrypted data will be saved.
- const char* csvFile - Path to the CSV file containing the encryption key and initialization vector (IV). Make sure the CSV file follows the specified format (key and IV separated by a comma).

5. Save the changes to the encryption.cpp file.

6. In the terminal, compile the code using the following command:

- `g++ encryption.cpp -o encryption -lssl -lcrypto`

7. After successful compilation, run the program by executing the following command:

- `./encryption`

8. The code will execute the encryption process and display a success message upon completion. If you also want to perform decryption, uncomment the corresponding code block in the main() function and recompile the program.


// Uncomment this part if you want to decrypt
/*
try {
    decryptFile(inputFile, outputFile, key, iv);
    std::cout << "File decryption completed." << std::endl;
} catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
*/

9. Check the specified output file path to find the encrypted/decrypted file.

10. Congratulations! You have successfully executed the file encryption and decryption code on your Linux system. Then you can encrypt your `credentials.csv` using an asymmetric encryption algorithm such as RSA, to keep safe your credentials. I will explain that in other repository.

11. Feel free to modify and customize the code according to your specific requirements.

12. License

This code is released under the MIT License. You can find the full license text in the LICENSE file.
