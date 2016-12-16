#include <iostream>

#include <fstream>

#include <openssl/md5.h>

#include <openssl/sha.h>

using namespace std;


string read_file(string filename)
{

  ifstream t(filename.c_str());

  string str((istreambuf_iterator<char>(t)),

                 istreambuf_iterator<char>());

  return str;

}


string compute_MD5(string path){
  /* Takes a string and returns a string, representing the md5 hash of the parameter */
  string content = read_file(path);

  string output;

  output.resize(16); // size of md5 hash

  MD5_CTX md5_ctx;

  //start hashing
  MD5_Init(&md5_ctx);

  MD5_Update(&md5_ctx, (unsigned char*)content.data(), content.size());

  MD5_Final( (unsigned char*)output.data(), &md5_ctx);

  return output;
}

string compute_SHA256(string path) {
/* Takes a string and returns a string, representing the sha256 hash of the parameter */
  string content = read_file(path);

  string output;

  output.resize(32); //size of sha256

  SHA256_CTX sha256_ctx;

  SHA256_Init(&sha256_ctx);

  SHA256_Update(&sha256_ctx, (unsigned char*)content.data(), content.size());

  SHA256_Final( (unsigned char*)output.data(), &sha256_ctx);

  return output;
}

int compare_files(string hash1, string hash2)
{
  /* compares byte by byte 2 hashes, returns number of similaraties. Bith hashes need to be of the same type, or at least size in this case */
  int similaraties = 0;
  if(hash1.size() != hash2.size())
  {
    cout << "Can only compare the same type of hash" << '\n';
    return -1;
  }
  for (int i = 0; i < hash1.size(); ++i)
  {
    if(hash1.at(i) == hash2.at(i)){
      ++similaraties;
    }
  }
  return similaraties;
}

int main(int argc, char* argv[])
{
  string file1_md5 = compute_MD5(argv[1]);
  string file2_md5 = compute_MD5(argv[2]);
  string file1_sha256 = compute_SHA256(argv[1]);
  string file2_sha256 = compute_SHA256(argv[2]);

  cout << compare_files(file1_md5, file2_md5) << '\n';
  cout << compare_files(file1_sha256, file2_sha256) << '\n';

  return 0;
}
