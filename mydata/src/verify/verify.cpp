#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <string.h>
#include <math.h>
#include "/usr/local/include/pbc/pbc.h"
#include "/usr/include/json/json.h"
#include "/usr/local/include/pbc/pbc_test.h"
using namespace std;
const string configPath = "../../data/config/config";
const string PPPath = "../../data/setup_data/PP";
const string MKPath = "../../data/setup_data/MK";
const string DdPath = "../../data/extract_data/Dd";
const string wwPath = "../../data/extract_data/ww";
const string SsPath = "../../data/sign_data/Ss";
const string AAPath = "../../data/setup_data/AA";
const int degree = 5;
const int n=10;
const int m=10;

unsigned char* transfer(element_t t){

  int leng = element_length_in_bytes(t);
  unsigned char *p = new unsigned char[leng + 1];
  int writeLength = element_to_bytes_compressed(p,t);
  return p;
}

string readFile(string path){
	stringstream ss;
	fstream readPathData(path.c_str());
	ss << readPathData.rdbuf();
	string data = ss.str();
	ss.clear();
	ss.str("");
	readPathData.close();
	return data;
}
//从字符串中读取JSON  
string readStrJson(string str1,string name)   
{  
    //字符串  
  string str = str1;
  Json::Reader reader;  
  Json::Value root;
  //从字符串中读取数据  
  if(reader.parse(str,root))  
  {  
      string value = root[name].asString();
      return value;  
  }  
    
} 
void G_ele(element_t g,string gc){
        char *p;
	int len = gc.length();
	p=new char[len+1];
	strcpy(p,gc.c_str());
        unsigned char * chargmp_g = (unsigned char *)p;
        element_from_bytes_compressed(g,chargmp_g);
}
void Z_ele(element_t y,string yc){
        mpz_t gmp_y;
        const char * chargmp_y = yc.c_str();
        mpz_init_set_str(gmp_y,chargmp_y,10);
        element_set_mpz(y,gmp_y);
}
int main(int argc, char **argv){
        string Ss = readFile(SsPath);
 	string AA = readFile(AAPath);
        string ww = readFile(wwPath);
	cout<<ww;
        string PP = readFile(PPPath);
        //cout<<PP;
	string Dd = readFile(DdPath);
        //cout<<Dd;
        //element_init_GT(A, pairing);

        pairing_t pairing;
        pbc_demo_pairing_init(pairing, argc, argv);
        /*
	string Aac=readStrJson(Aa,"AA");
        element_t Aa;
        element_init_Zr(Aa,pairing);
        Z_ele(Aa,Aac);
        element_printf("Aa=%B\n",Aa);
	*/

	element_t w[n+1];
	for(int i=0;i<n+1;i++){
        element_init_Zr(w[i],pairing);
 	string index = "";
        stringstream st;
        st << (i+1);
        st >> index;
        index ="w-" + index;
	string wc=readStrJson(ww,index);
 	Z_ele(w[i],wc);
        element_printf("w[%d]=%B\n",i,w[i]);
	}

	string gc=readStrJson(PP,"g");
        element_t g;
        element_init_G1(g,pairing);
        G_ele(g,gc);
       // element_printf("g=%B\n",g);

        element_t T[n+1];
        for(int i=0;i<n+1;i++){
  	element_init_G1(T[i], pairing);
        string index = "";
        stringstream st;
        st << (i+1);
        st >> index;
        index ="T-" + index;
        string Tc=readStrJson(Dd,index);
        G_ele(T[i],Tc);
        //element_printf("T[%d]=%B\n",i,T[i]);
        }

        element_t vs[n+1];
        for(int i=0;i<n+1;i++){
  	element_init_G1(vs[i], pairing);
        string index = "";
        stringstream st;
        st << (i+1);
        st >> index;
        index ="vs-" + index;
        string vsc=readStrJson(Ss,index);
        G_ele(vs[i],vsc);
        //element_printf("vs[%d]=%B\n",i,vs[i]);
        }
	
	element_t s1[n+1];
        for(int i=0;i<n+1;i++){
  	element_init_G1(s1[i], pairing);
        string index = "";
        stringstream st;
        st << (i+1);
        st >> index;
        index ="s1-" + index;
        string s1c=readStrJson(Ss,index);
        G_ele(s1[i],s1c);
       // element_printf("s1[%d]=%B\n",i,s1[i]);
        }

	element_t s2[n+1];
        for(int i=0;i<n+1;i++){
  	element_init_G1(s2[i], pairing);
        string index = "";
        stringstream st;
        st << (i+1);
        st >> index;
        index ="s2-" + index;
        string s2c=readStrJson(Ss,index);
        G_ele(s2[i],s2c);
        //element_printf("s2[%d]=%B\n",i,s2[i]);
        }

	element_t s3[n+1];
        for(int i=0;i<n+1;i++){
  	element_init_G1(s3[i], pairing);
        string index = "";
        stringstream st;
        st << (i+1);
        st >> index;
        index ="s3-" + index;
        string s3c=readStrJson(Ss,index);
        G_ele(s3[i],s3c);
        //element_printf("s3[%d]=%B\n",i,s3[i]);
        }

	element_t A;
	element_init_GT(A, pairing);
	element_set1(A);
        element_t AA1;
	element_init_GT(AA1, pairing);
	element_t AA2;
	element_init_GT(AA2, pairing);
	element_t AA3;
	element_init_GT(AA3, pairing);
	element_t tmp;
	element_init_GT(tmp, pairing);
	//element_pairing(A,g1,g2);
        for(int i=0;i<n+1;i++){
         element_pairing(AA1,s1[i],g);
	 element_pairing(AA2,s2[i],T[i]);
	 element_pairing(AA3,s3[i],vs[i]);
	 element_mul(tmp,AA1,AA2);
	 element_mul(tmp,tmp,AA3);

        element_t sub1,sub2,div,temp_n,mul;
	element_init_Zr(sub1, pairing);
	element_init_Zr(sub2, pairing);
	element_init_Zr(div, pairing);
	element_init_Zr(temp_n, pairing);
	element_init_Zr(mul, pairing);
	long s0=0;
	element_set_si(temp_n,(signed long int)s0);
	for(int j=0;j<n+1;j++){
	   element_random(sub1); 
	   element_random(sub2); 
	   element_random(div); 
	   element_random(mul);
   	   element_set1(mul);
		for(int k=0;k<n+1;k++){
		  if(k == j) continue;
		  else{
				 element_sub(sub1,temp_n,w[k]);
				 element_sub(sub2,w[j],w[k]);
				 element_div(div,sub1,sub2);
				 element_mul(mul,mul,div);
		      }
		}//for-k 
	  }//for-j	 
	//element_printf("mul[%d] = %B\n",i, mul);
	element_pow_zn(tmp,tmp,mul);
        element_mul(A,A,tmp);
	}
	element_printf("A = %B\n",A);
return 0;
}

