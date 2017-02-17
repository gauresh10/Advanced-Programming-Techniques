// ECE4122/6122 RSA Encryption/Decryption assignment
// Fall Semester 2015

#include <iostream>
#include "RSA_Algorithm.h"
//#define debug 1
//#define check 1
using namespace std;
//gmp_randclass num(gmp_randinit_default);
mpz_class M,C,D;
int max_iter=10;
int count=0;
int main()
{
//int arr[7*10];
  // Instantiate the one and only RSA_Algorithm object
  RSA_Algorithm RSA;


  for(int sz=32;sz<=1024;sz=sz*2){


      for(int loop1= 0;loop1<max_iter;++loop1){


          RSA.GenerateRandomKeyPair(sz);

              for(int loop2= 0;loop2<max_it// ECE4122/6122 RSA Encryption/Decryption assignment
// Fall Semester 2015

#include <iostream>
#include "RSA_Algorithm.h"
//#define debug 1
//#define check 1
using namespace std;
//gmp_randclass num(gmp_randinit_default);
mpz_class M,C,D;
int max_iter=100;
int count=0;
int main()
{
//int arr[7*10];
  // Instantiate the one and only RSA_Algorithm object
  RSA_Algorithm RSA;
  
      
  for(int sz=32;sz<=1024;sz=sz*2){
  
  
      for(int loop1= 0;loop1<max_iter;++loop1){
         
          
          RSA.GenerateRandomKeyPair(sz);
          RSA.PrintNDE();   
              for(int loop2= 0;loop2<max_iter;++loop2){
              
                    //gmp_randclass r(gmp_randinit_default);
                    //r.seed(loop1*loop2+loop2);
                      //generate M LESS than n 
                    M=RSA.rng.get_z_bits((sz*2)-1);
                    while(M>=RSA.n){M=RSA.rng.get_z_bits((sz*2)-1);}
                   
          
                     RSA.PrintM(M);
                      
                      
                      
                      
                      C=RSA.Encrypt(M);
                      RSA.PrintC(C);
                      
                      D=RSA.Decrypt(C);
          
                      if(M==D){
                      #ifdef check
                      cout<< " D matches with M "<<endl;
                      #endif
                    
                      }
                      else{
                      #ifdef check
                      cout<< " Cannot decipher D "<<endl;
                      #endif
                      
                      }
                }
            }
        }
  #ifdef check
  cout<<" The total count is  "<<count<<endl;
  #endif
  // Loop from sz = 32 to 1024 inclusive
  // for each size choose 10 different key pairs
  // For each key pair choose 10 differnt plaintext 
  // messages making sure it is smaller than n.
  // If not smaller then n then choose another
  // For eacm message encrypt it using the public key (n,e).
  // After encryption, decrypt the ciphertext using the private
  // key (n,d) and verify it matches the original message.

  // your code here
}
  
er;++loop2){
                    gmp_randclass r(gmp_randinit_default);
                    r.seed(loop1*loop2+loop2);
                      //generate M LESS than n
                    M=r.get_z_bits((sz*2));
                    while(M>RSA.n){M=r.get_z_bits((sz*2));}


                     RSA.PrintM(M);




                      C=RSA.Encrypt(M);
                      RSA.PrintC(C);

                      D=RSA.Decrypt(C);

                      if(M==D){
                      #ifdef check
                      cout<< " D matches with M "<<endl;
                      #endif

                      }
                      else{
                      #ifdef check
                      cout<< " Cannot decipher D "<<endl;
                      #endif

                      }
                }
            }
        }
  #ifdef check
  cout<<" The total count is  "<<count<<endl;
  #endif
  // Loop from sz = 32 to 1024 inclusive
  // for each size choose 10 different key pairs
  // For each key pair choose 10 differnt plaintext
  // messages making sure it is smaller than n.
  // If not smaller then n then choose another
  // For eacm message encrypt it using the public key (n,e).
  // After encryption, decrypt the ciphertext using the private
  // key (n,d) and verify it matches the original message.

  // your code here
}
