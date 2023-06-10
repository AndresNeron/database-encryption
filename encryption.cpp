#include <iostream>
#include <fstream>
#include <cstring>
#include <openssl/evp.h>
#include <openssl/conf.h>
#include <openssl/err.h>

// Error Function
void handleOpenSSLErrors()
{
    unsigned long errCode;
    while ((errCode = ERR_get_error()) != 0)
    {
        char* err = ERR_error_string(errCode, nullptr);
        std::cerr << "OpenSSL error: " << err << std::endl;
    }
    throw std::runtime_error("OpenSSL error occurred.");
}

void extractKeyAndIVFromCSV(const char* filename, unsigned char* key, unsigned char* iv)
{
    std::ifstream csvFile(filename);
    if (!csvFile)
    {
        std::cerr << "Failed to open CSV file." << std::endl;
        return;
    }

    std::string line;
    if (std::getline(csvFile, line))
    {
        // Extract the key and IV from the line
        if (line.length() >= 32 + 1 + 16)
        {
            std::string keyStr = line.substr(0, 32);
            std::string ivStr = line.substr(33, 16);

            // Copy the key and IV to the provided buffers
            std::memcpy(key, keyStr.c_str(), 32);
            std::memcpy(iv, ivStr.c_str(), 16);
        }
        else
        {
            std::cerr << "Invalid key and IV format in CSV file." << std::endl;
        }
    }
    else
    {
        std::cerr << "Failed to read from CSV file." << std::endl;
    }

    csvFile.close();
}

// Encryption Function
void encryptFile(const char* inputFile, const char* outputFile, const unsigned char* key, const unsigned char* iv)
{
	// Initialize the OpenSSL library
	OpenSSL_add_all_algorithms();
	ERR_load_crypto_strings();
	OPENSSL_init_crypto(0, NULL);

	// Set up the encryption algorithm 
	const EVP_CIPHER* cipher = EVP_aes_256_cbc();
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv);

	// Open the input and output files
	std::ifstream inFile(inputFile, std::ios::binary);
	if (!inFile)
	{
		std::cerr << "Failed to open input file." << std::endl;
		EVP_CIPHER_CTX_free(ctx);
		return;
	}

	std::ofstream outFile(outputFile, std::ios::binary);
	if (!outFile)
	{
		std::cerr << "Failed to open output file." << std::endl;
		inFile.close();
		EVP_CIPHER_CTX_free(ctx);
	}

	// Encrypt the file
	const int bufferSize = 4096;
	unsigned char buffer[bufferSize];
	int bytesRead = 0;
	int bytesWritten = 0;

	while ((bytesRead = inFile.read(reinterpret_cast<char*>(buffer), bufferSize).gcount()) > 0)
	{
		if (!EVP_EncryptUpdate(ctx, buffer, &bytesWritten, buffer, bytesRead))
		{
			handleOpenSSLErrors();
			inFile.close();
			outFile.close();
			EVP_CIPHER_CTX_free(ctx);
			return;
		}

		outFile.write(reinterpret_cast<const char*>(buffer), bytesWritten);
	}

	// Finalize the encryption process
	if (EVP_EncryptFinal_ex(ctx, buffer, &bytesWritten) != 1)
	{
		handleOpenSSLErrors();
		inFile.close();
		outFile.close();
		EVP_CIPHER_CTX_free(ctx);
		return;
	}

	outFile.write(reinterpret_cast<const char*>(buffer), bytesWritten);

	// Clean up
	inFile.close();
	outFile.close();
	EVP_CIPHER_CTX_free(ctx);
}

// Decryption Function
void decryptFile(const char* inputFile, const char* outputFile, const unsigned char* key, const unsigned char* iv)
{
    // Initialize the OpenSSL library
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();
    OPENSSL_init_crypto(0, NULL);

    // Set up the decryption algorithm, in this case AES256.
    const EVP_CIPHER* cipher = EVP_aes_256_cbc();
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv);

    // Open the input and output files
    std::ifstream inFile(inputFile, std::ios::binary);
    if (!inFile)
    {
        std::cerr << "Failed to open input file." << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return;
    }

    std::ofstream outFile(outputFile, std::ios::binary);
    if (!outFile)
    {
        std::cerr << "Failed to open output file." << std::endl;
        inFile.close();
        EVP_CIPHER_CTX_free(ctx);
        return;
    }

    // Decrypt the file
    const int bufferSize = 4096;
    unsigned char buffer[bufferSize];
    int bytesRead = 0;
    int bytesWritten = 0;

    while ((bytesRead = inFile.read(reinterpret_cast<char*>(buffer), bufferSize).gcount()) > 0)
    {
        if (!EVP_DecryptUpdate(ctx, buffer, &bytesWritten, buffer, bytesRead))
        {
            handleOpenSSLErrors();
            inFile.close();
            outFile.close();
            EVP_CIPHER_CTX_free(ctx);
            return;
        }

        outFile.write(reinterpret_cast<const char*>(buffer), bytesWritten);
    }

    // Handle the final block separately
	if (!EVP_DecryptFinal_ex(ctx, buffer, &bytesWritten))
	{
	    handleOpenSSLErrors();
	    inFile.close();
	    outFile.close();
	    EVP_CIPHER_CTX_free(ctx);
	    return;
	}

	outFile.write(reinterpret_cast<const char*>(buffer), bytesWritten);

    // Finalize the decryption process
    if (!EVP_DecryptFinal_ex(ctx, buffer, &bytesWritten))
    {
        handleOpenSSLErrors();
        inFile.close();
        outFile.close();
        EVP_CIPHER_CTX_free(ctx);
        return;
    }

    outFile.write(reinterpret_cast<const char*>(buffer), bytesWritten);

    // Clean up
    inFile.close();
    outFile.close();
    EVP_CIPHER_CTX_free(ctx);
}

// main function
int main(){

	// Pointers to the file that will be encrypted and the output path.
	//const char* inputFile = "/path/to/input.zip"
	//const char* outputFile = "path/to/output.zip"

	
	// Pointer to the .csv file with the credentials (key and iv separated with a comma)
    	const char* csvFile = "/path/to/credentials.csv";
    	unsigned char key[32];
    	unsigned char iv[16];

    	// Extract key and iv from the CSV file
    	extractKeyAndIVFromCSV(csvFile, key, iv);

	// Encryption try
	try{
		encryptFile(inputFile, outputFile, key, iv);
		std::cout << "File encryption sucessfully." << std::endl;
	}

	catch (const std::exception &e){
		std::cerr << "Error: " << e.what() << std::endl;
	}
	

	// ---------------------------------------------------------------

	// Uncomment this part if you want to decrypt.
	/*	
	try{
		decryptFile(inputFile, outputFile, key, iv);
		std::cout << "File decryption completed." << std::endl;
	}

	*/

	return 0;
}
