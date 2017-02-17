// Grad student portion of the RSA assignment
// Fall 2015 ECE6122

#include <iostream>

#include "RSA_Algorithm.h"
//#define debug 1

using namespace std;
 mpz_class temp,mod_res,p,q,phi,sub_p,sub_q;
int main(int argc, char** argv)
{ // Arguments are as follows:
  //argv[1] = n;
  //argv[2] = e;  // n and e are the public key
  //argv[3] = first 6 characters of encrypted message
  //argv[4] = next 6 characters .. up to argv[12] are the lsat 6 characters
  // The number of arguments will always be exacty 12, and each argument past the
  // public key contain 6 ascii characters of the encrypted message.
  // Each of the 32bit values in the argv[] array are right justified in the
  // low order 48 bits of each unsigned long.  The upper 16 bits are always
  // zero, which insures the each unsigned long is < n (64 bits) and therefore
  // the RSA encryption will work.

  // Below is an example of the BreakRSA and command line arguments:

// ./BreakRSA  2966772883822367927 2642027824495698257  817537070500556663 1328829247235192134 
// 1451942276855579785 2150743175814047358 72488230455769594 1989174916172335943 962538406513796755 
// 1069665942590443121 72678741742252898 1379869649761557209

//   The corect output from the above is:
//   HelloTest  riley CekwkABRIZFlqmWTanyXLogFgBUENvzwHpEHRCZIKRZ
//
//   The broken (computed) private key for the above is 4105243553



  // Our one and only RSA_Algorithm object
  RSA_Algorithm rsa;
  
  // First "break" the keys by factoring n and computing d
  // Set the keys in the rsa object afer calculating d
  rsa.n = mpz_class(argv[1]);
  rsa.e = mpz_class(argv[2]);
  
 
  
  //Take square root
  mpz_sqrt(temp.get_mpz_t(),rsa.n.get_mpz_t());
  #ifdef debug
  cout<<"Temp val is "<<temp<<endl;
  #endif

  //n mod temp
  mpz_mod(mod_res.get_mpz_t(),rsa.n.get_mpz_t(),temp.get_mpz_t());


  //find a factor that is completely divisible
  while(mod_res!=0){
  temp=temp-1;
  mpz_mod(mod_res.get_mpz_t(),rsa.n.get_mpz_t(),temp.get_mpz_t());
  }
  
  //we get p
  p=temp;
  
  #ifdef debug
  cout<<"p val is "<<p<<endl;
  #endif  
  
  //we get q
  mpz_divexact(q.get_mpz_t(),rsa.n.get_mpz_t(),p.get_mpz_t());
  
  #ifdef debug
  cout<<"q val is "<<q<<endl;
  #endif  
  //get phi
  mpz_sub_ui(sub_p.get_mpz_t(),p.get_mpz_t(),1);
  mpz_sub_ui(sub_q.get_mpz_t(),q.get_mpz_t(),1);
  mpz_mul(phi.get_mpz_t(),sub_p.get_mpz_t(),sub_q.get_mpz_t());  
  
  #ifdef debug
  cout<<"phi val is "<<phi<<endl;
  #endif  
  //get d
  int check_val=mpz_invert(rsa.d.get_mpz_t(),rsa.e.get_mpz_t(),phi.get_mpz_t());
  if(check_val!=0){
  
    #ifdef debug
    cout<<"d is "<<rsa.d<<endl;
    #endif
  // Set rsa.d to the calculated private key d
  // rsa.d = mpz_class(// broken d value here) 
  for (int i = 3; i < 13; ++i)
    { // Decrypt each set of 6 characters
      mpz_class c(argv[i]);
      mpz_class m = rsa.Decrypt(c);
      //  use the get_ui() method in mpz_class to get the lower 48 bits of the m
      unsigned long ul = m.get_ui();
     // Now print the 6 ascii values in variable ul.  
      // As stated above the 6 characters
      // are in the low order 48 bits of ui.
      for (int j = 0; j < 6; ++j)
        {
          unsigned char ch = ul >> (48 - j * 8 - 8);
          cout << ch;
        }
    }
    }
    else{
    #ifdef debug
    cout<<"d not generated! break"<<endl;
    #endif
    }
  cout << endl;
}

