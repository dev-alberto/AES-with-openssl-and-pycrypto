#include <openssl/aes.h>

#include <openssl/evp.h>

#include <string>

#include <iostream>

#include <fstream>

using namespace std;

string iv = "BBBBBBBBBBBBBBBB";

int blockSize(EVP_CIPHER_CTX ctx_)

{

  return EVP_CIPHER_CTX_block_size(&ctx_);

}

string aes_decrypt(string input, string key, string mode)
{
  /* decription method, using mostly c++ only, long live strings. Receives input, key, encryption mode, returns decryption result. The steps are pretty much standard,
  taken directly out of the openssl documentation, but with a c++ twist */
  string output;

  EVP_CIPHER_CTX e_ctx;

  const EVP_CIPHER *cryptoAlgorithm;

  EVP_CIPHER_CTX_init(&e_ctx);

  if(mode == "ECB")

  {

    cryptoAlgorithm = EVP_aes_128_ecb();

    EVP_DecryptInit_ex(&e_ctx, cryptoAlgorithm, NULL,

                       (const unsigned char*)(key.data()),

                        NULL);

  }

  else{

    cryptoAlgorithm = EVP_aes_128_cfb();

    EVP_DecryptInit_ex(&e_ctx, cryptoAlgorithm, NULL,

                       (const unsigned char*)(key.data()),

                       (const unsigned char*)(iv.data()));

  }

  EVP_CIPHER_CTX_set_padding(&e_ctx, 1);

  //std::cout << blockSize(e_ctx) << '\n';

  output.resize(input.size() + blockSize(e_ctx));

  int resultLength, finale;

  EVP_DecryptUpdate(&e_ctx,

                   (unsigned char*)output.data(),

                   &resultLength,

                   (unsigned char*)input.data(),

                   input.size());

  //std::cout << output << '\n';
  //std::cout << output.size() << '\n';
//  output.resize(resultLength);
//  std::cout << resultLength << '\n';

//  output.resize(blockSize(e_ctx) + resultLength);

  EVP_DecryptFinal_ex(&e_ctx,

                      (unsigned char*)(output.data() + resultLength),

                      &finale);
//  std::cout << finale << '\n';
  //std::cout << "debug2" << '\n';
  //std::cout << output << '\n';
  output.resize(resultLength + finale); // for ECB purpouse
  //std::cout << output << '\n';
//  std::cout << resultLength + finale << '\n';

  return output;

}


string process_string(string key_)
{
  /* Make each key 16 bytes long, avoid nasty hex conversion */

  //std::cout << key_.size() << '\n';
  while (key_.size() < 16) {
    key_.append(" ");
  }
  //std::cout << key_.size() << '\n';
  return key_;
}

string read_file(string filename)
{
  /* helper function to read an whole file using an iterator and puting everything into a nice string. Probably a bad idea to use this if the file is large */

  ifstream t(filename.c_str());

  string str((istreambuf_iterator<char>(t)),

                 istreambuf_iterator<char>());

  return str;

}

string get_key(string pt, string ct, string mode)
{
    /* Core function, exaustively searches for the key, in the given document. Returns the key and prints the number of tries until the key is found/not found */

    string expected_output = read_file(pt); // Read plain text, compare each decription try with this
    string cipher_text = read_file(ct); //read cipher text. We will attemt to break this puppy
    string key;

    ifstream fin("word_dict.txt");

    int nr_of_tries = 0;

    while (fin >> key) {
      string output = aes_decrypt(cipher_text, process_string(key), mode);
      if(output == expected_output){
        cout << "Number of keys attemted until the right one was found: " <<nr_of_tries << '\n';
        return key;
      }
      ++nr_of_tries;
    }
    fin.close();
    cout << nr_of_tries << '\n';
    return "no key was found";
}

int main()
{
/*
  string input = read_file("plain.txt");
  //string key = "AAAAAAAAAAAAAAAA";
  string key2 = process_string("median");

  string ecb_file = read_file("enc_ecb.txt");
  string cfb_file = read_file("enc_cfb.txt");
  string plain_ecb = aes_decrypt(ecb_file, key2, "ECB");
  string plain_cfb = aes_decrypt(cfb_file, key2, "CFB");
  std::cout << plain_ecb << '\n';
  std::cout << plain_cfb << '\n';
*/
  cout <<"Key used for ECB encryption: " << get_key("plain.txt", "enc_ecb.txt", "ECB") << '\n';
  cout <<"Key used for CFB encryption: " <<get_key("plain.txt", "enc_cfb.txt", "CFB") << '\n';
  return 0;
}
