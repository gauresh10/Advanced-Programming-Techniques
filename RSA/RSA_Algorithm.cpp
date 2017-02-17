// RSA Assignment for ECE4122/6122 Fall 2015

#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
//#define debug 1
#include "RSA_Algorithm.h"

using namespace std;

// Implement the RSA_Algorithm methods here

// Constructor
RSA_Algorithm::RSA_Algorithm()
  : rng(gmp_randinit_default)
{
  // get a random seed for the random number generator
  int dr = open("/dev/random", O_RDONLY);
  if (dr < 0)
    {
      cout << "Can't open /dev/random, exiting" << endl;
      exit(0);
    }
  unsigned long drValue;
  read(dr, (char*)&drValue, sizeof(drValue));
  //cout << "drValue " << drValue << endl;
  rng.seed(drValue);
// No need to init n, d, or e.
}

// Fill in the remainder of the RSA_Algorithm methods

void RSA_Algorithm::GenerateRandomKeyPair(size_t sz){
mpz_class p,q,phi,sub_p,sub_q,gcd_var;
bool repflag=false;
     //generate p
      p=rng.get_z_bits(sz);
      while(mpz_probab_prime_p(p.get_mpz_t(), 100)==0){p=rng.get_z_bits(sz);}
      #ifdef debug
      cout<<"p is generated"<<endl;
      #endif
      
      //generate q
      q=rng.get_z_bits(sz);
      while(mpz_probab_prime_p(q.get_mpz_t(), 100)==0){q=rng.get_z_bits(sz);}
      #ifdef debug
       cout<<"q is generated"<<endl;
      #endif
      
      
     
      
     
     //generate n
      mpz_mul(n.get_mpz_t(),p.get_mpz_t(),q.get_mpz_t());
      
      //generate phi
      mpz_sub_ui(sub_p.get_mpz_t(),p.get_mpz_t(),1);
      mpz_sub_ui(sub_q.get_mpz_t(),q.get_mpz_t(),1);
      mpz_mul(phi.get_mpz_t(),sub_p.get_mpz_t(),sub_q.get_mpz_t());        
      #ifdef debug
      cout<<"phi is generated"<<endl;
      #endif
        
        
      //select d  
      
        
      while(repflag==false){
        d=rng.get_z_bits(sz*2);
       // while(mpz_probab_prime_p(d.get_mpz_t(), 100)==0){d=rng.get_z_bits(sz*2);}
            //select d if d < phi
          /*
            while(RSA.d.get_mpz_t()>=phi.get_mpz_t()){
            RSA.d=num.get_z_bits(sz*2);
            }
            */
            if(d<phi) 
            {
              mpz_gcd(gcd_var.get_mpz_t(),d.get_mpz_t(),phi.get_mpz_t());
              if(gcd_var==1 ) repflag=true;
              else repflag=false;
            }
            else repflag=false;
            
      }
        #ifdef debug
        cout<<"d is generated"<<endl;
        #endif
//        PrintND();
        
        //generate e
        int check_val=mpz_invert(e.get_mpz_t(),d.get_mpz_t(),phi.get_mpz_t());
        
        if(check_val!=0){
        #ifdef debug
        cout<<"e is generated"<<endl;
        #endif
//        PrintNE();
 
        }
        else{
        #ifdef debug
        cout<<"e not generated! break"<<endl;
        #endif
       // break;
        }
       
      
      

}


 mpz_class RSA_Algorithm::Encrypt(mpz_class M){
 
 mpz_powm(C.get_mpz_t(),M.get_mpz_t(),e.get_mpz_t(),n.get_mpz_t());
 return C;
 }
 
 
 
 mpz_class RSA_Algorithm::Decrypt(mpz_class C){
 
 mpz_powm(M.get_mpz_t(),C.get_mpz_t(),d.get_mpz_t(),n.get_mpz_t());
 return M;
   
   
   
   }

void RSA_Algorithm::PrintND()
{ // Do not change this, right format for the grading script
  cout << "n " << n << " d " << d << endl;
}

void RSA_Algorithm::PrintNE()
{ // Do not change this, right format for the grading script
  cout << "n " << n << " e " << e << endl;
}

void RSA_Algorithm::PrintNDE()
{ // Do not change this, right format for the grading script
  cout << "n " << n << " d " << d << " e " << e << endl;
}

void RSA_Algorithm::PrintM(mpz_class M)
{ // Do not change this, right format for the grading script
  cout << "M " << M << endl;
}

void RSA_Algorithm::PrintC(mpz_class C)
{ // Do not change this, right format for the grading script
  cout << "C " << C << endl;
}




