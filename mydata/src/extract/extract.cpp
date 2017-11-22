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
void ele_nx(element_t div,element_t sub1,element_t sub2,element_t w,element_t l,element_t i,pairing_t pairing){
                 element_printf("div = %B,sub1=%B,sub2=%B,w=%B,l=%B,i=%B\n",div,sub1,sub2,w,l,i);
         	 element_sub(sub1,w,l);
                 element_sub(sub2,i,l);
                 element_div(div,sub1,sub2);
                 element_printf("div = %B\n",div);
}

void ele_T(element_t tt,element_t g2,element_t t,int i,pairing_t pairing){
         element_t temp_n;
         element_init_Zr(temp_n, pairing);
         long x=pow(i,n);
         element_set_si(temp_n,(signed long int)x);
         element_pow_zn(tt,g2,temp_n);//T[i]==tt=g2^(x^n)
}

int main(int argc, char **argv){

	string config = readFile(configPath);
        //cout<<config;
	string PP = readFile(PPPath);
        //cout<<PP;
        string MK = readFile(MKPath);
        //cout<<MK;
        
        pairing_t pairing;
        pbc_demo_pairing_init(pairing, argc, argv);
        
        string yc=readStrJson(MK,"y");
        element_t y;
        element_init_Zr(y,pairing);
        Z_ele(y,yc);
        element_printf("y=%B\n",y);

        string gc=readStrJson(PP,"g");
        element_t g;
        element_init_G1(g,pairing);
        G_ele(g,gc);
        element_printf("g=%B\n",g);
        
        string g2c=readStrJson(PP,"g2");
        element_t g2;
        element_init_G1(g2,pairing);
        G_ele(g2,g2c);
        element_printf("g2=%B\n",g2);

        element_t t[n+1];
        for(int i=0;i<n+1;i++){
  	element_init_G1(t[i], pairing);
        string index = "";
        stringstream st;
        st << (i+1);
        st >> index;
        index ="t-" + index;
        string tc=readStrJson(PP,index);
        G_ele(t[i],tc);
        }
//--------EXTRACT------d--------
element_t r[n+1];
element_t rr[n+1];
element_t d[n+1];
  for(int i=0;i<n+1;i++){
   //初始化变量
   element_init_Zr(r[i], pairing);
   element_init_Zr(rr[i], pairing);
   element_init_G1(d[i], pairing);
   element_random(r[i]);
   element_random(rr[i]);
   element_random(d[i]);
   element_neg(rr[i],r[i]);
   element_pow_zn(d[i],g,rr[i]);
}//r[i]
/*
//to file
Json::Value root;
Json::StyledWriter sw;  
  //输出到文件  
  ofstream os;
  os.open("../../data/extract_data/Dd"); 
  for(int i=0;i<n+1;i++){
  string index = "";
  stringstream st;
  st << (i+1);
  st >> index;
  index ="d-" + index;
  root[index] = Json::Value((char*)transfer(d[i]));
  }
  os << sw.write(root);
  os.close(); 
*/
//--------EXTRACT------d--------
//--------EXTRACT------D--------
element_t q[n+1];
element_t D[n+1];
element_t T[n+1];
element_t TT[n+1];
element_t w[n+1];
for(int i=0;i<n+1;i++){
   element_init_Zr(w[i],pairing);
   element_random(w[i]);
}
for(int i=0;i<n+1;i++){
   //初始化变量
   element_init_Zr(q[i], pairing);
   element_init_G1(D[i], pairing);
   element_init_G1(T[i], pairing);
   element_init_G1(TT[i], pairing);
   element_random(D[i]);
   element_random(T[i]);
   element_random(TT[i]);
   ele_T(T[i],g2,t[i],i,pairing);
   element_printf("T[%d] = %B\n",i, T[i]);
   
   element_t temp_n;
   element_init_Zr(temp_n, pairing);

   element_t sub1,sub2,div,mul;
   element_init_Zr(sub1, pairing);
   element_init_Zr(sub2, pairing);
   element_init_Zr(div, pairing);
   element_init_Zr(mul, pairing);
   for(int j=0;j<n+1;j++){
   element_random(sub1); 
   element_random(sub2); 
   element_random(div); 
   element_random(mul);
   element_set1(mul);
 	for(int k=0;k<n+1;k++){
	  if(k == j) continue;
          else if(k == i) continue;
          else{
                 element_sub(sub1,w[i],w[k]);
                 element_sub(sub2,w[j],w[k]);
                 element_div(div,sub1,sub2);
                 element_mul(mul,mul,div);
                 element_printf("mul[%d] = %B\n",k, mul);
              }
	 }//for
       element_pow_zn(t[j],t[j],mul);
       element_printf("t[%d] = %B\n",j, t[j]);
    }//for
  element_mul(T[i],T[i],t[i]);
  element_printf("T[%d] = %B\n",i, T[i]);
  element_pow_zn(TT[i],T[i],r[i]);

   if(i==0) element_set(q[i],y);
   else     element_random(q[i]);
   element_pow_zn(D[i],g2,q[i]);
   element_printf("D[%d] = %B\n",i, D[i]);

   element_mul(D[i],TT[i],D[i]);
   element_printf("D[%d] = %B\n",i, D[i]);
}
Json::Value root1;
Json::StyledWriter sw1;  
  //输出到文件  
  ofstream os1;
  os1.open("../../data/extract_data/Dd"); 
  for(int i=0;i<n+1;i++){
  string index = "";
  stringstream st;
  st << (i+1);
  st >> index;
  string index1 = index;
  string index2 = index;
  index ="d-" + index;
  index1 ="D-" + index1;
  index2 ="T-" + index2;
  root1[index] = Json::Value((char*)transfer(d[i]));
  root1[index1] = Json::Value((char*)transfer(D[i]));
  root1[index2] = Json::Value((char*)transfer(T[i]));
  }
  os1 << sw1.write(root1);
  os1.close(); 

if(freopen("../../data/extract_data/ww","w+",stdout)==NULL){
    fprintf(stderr, "error2\n");
  }
  printf("{\n");
  for(int i=0;i<n+1;i++){
  if(i==n) element_printf("\"w-%d\":\"%B\"",i+1,w[i]);
  else element_printf("\"w-%d\":\"%B\",\n",i+1,w[i]);
  }
  //end writing data to file MK 
  printf("}\n");

fclose(stdout);
//--------EXTRACT------D--------
//清空变量空间
  element_clear(g);
  element_clear(y);
  element_clear(g2);
  for(int i=0;i<n+1;i++){
  element_clear(d[i]);
  element_clear(D[i]);
  element_clear(r[i]);
  element_clear(T[i]);
  element_clear(w[i]);
  }
  pairing_clear(pairing);
return 0;
}

