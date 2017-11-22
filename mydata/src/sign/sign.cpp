#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "/usr/local/include/pbc/pbc.h"
#include "/usr/include/json/json.h"
#include "/usr/local/include/pbc/pbc_test.h"
using namespace std;
const string configPath = "../../data/config/config";
const string PPPath = "../../data/setup_data/PP";
const string MKPath = "../../data/setup_data/MK";
const string DdPath = "../../data/extract_data/Dd";
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

	string PP = readFile(PPPath);
        //cout<<PP;
        string Dd = readFile(DdPath);
        //cout<<Dd;
        pairing_t pairing;
        pbc_demo_pairing_init(pairing, argc, argv);
        

        string gc=readStrJson(PP,"g");
        element_t g;
        element_init_G1(g,pairing);
        G_ele(g,gc);
        //element_printf("g=%B\n",g);
        
        string v1c=readStrJson(PP,"v1");
        element_t v1;
        element_init_G1(v1,pairing);
        G_ele(v1,v1c);
        //element_printf("v1=%B\n",v1);
     
        long u1[m]={};
        element_t u[m];
	for(int i=0;i<m;i++)
	{
	  u1[i]=rand()&1;
          element_init_Zr(u[i], pairing);
          element_set_si(u[i],(signed long int)u1[i]);
          //element_printf("u[%d]=%B\n",i,u[i]);
	}
        
        
        element_t v[m];
        for(int i=0;i<m;i++){
  	element_init_G1(v[i], pairing);
        string index = "";
        stringstream st;
        st << (i+1);
        st >> index;
        index ="v-" + index;
        string vc=readStrJson(PP,index);
        G_ele(v[i],vc);
        //element_printf("v[%d]=%B\n",i,v[i]);
        }
 	element_t D[n+1];
        for(int i=0;i<n+1;i++){
  	element_init_G1(D[i], pairing);
        string index = "";
        stringstream st;
        st << (i+1);
        st >> index;
        index ="D-" + index;
        string Dc=readStrJson(Dd,index);
        G_ele(D[i],Dc);
        //element_printf("D[%d]=%B\n",i,D[i]);
        }
        
        element_t s[n+1];
        for(int i=0;i<n+1;i++){
         element_init_Zr(s[i],pairing);
	 element_random(s[i]);
         element_printf("s[%d]=%B\n",i,s[i]);
	}

        element_t d[n+1];
        element_t s2[n+1];
        for(int i=0;i<n+1;i++){
  	element_init_G1(d[i], pairing);
        element_init_G1(s2[i], pairing);
        string index = "";
        stringstream st;
        st << (i+1);
        st >> index;
        index ="d-" + index;
        string dc=readStrJson(Dd,index);
        G_ele(d[i],dc);
        element_set(s2[i],d[i]);
        //element_printf("s2[%d]=%B\n",i,s2[i]);
        }

        element_t s1[n+1];
        element_t vs[n+1];
        for(int i=0;i<n+1;i++){
         element_init_G1(s1[i],pairing);
         element_set1(s1[i]);
         element_init_G1(vs[i],pairing);
         element_set1(vs[i]);
         element_t tmp;
         element_init_G1(tmp,pairing);
         element_set1(tmp);
           for(int j=0;j<m;j++){
             //element_printf("v[%d]=%B\n",j,v[j]);
             element_pow_zn(tmp,v[j],u[j]);
             //element_printf("v[%d]=%B\n",j,v[j]);
           }
          element_mul(s1[i],s1[i],tmp);
          element_mul(s1[i],s1[i],v1);
          element_pow_zn(vs[i],s1[i],s[i]);
          element_mul(s1[i],D[i],vs[i]);
          //element_printf("s1[%d]=%B\n",i,s1[i]);
        }
      
	element_t s3[n+1];
	for(int i=0;i<n+1;i++){
         element_init_G1(s3[i],pairing);
         element_set1(s3[i]);
         element_neg(s[i],s[i]);
	 element_pow_zn(s3[i],g,s[i]);
	 element_printf("s3[%d]=%B\n",i,s3[i]);
	}
//to file
Json::Value root;
Json::StyledWriter sw;  
  //输出到文件  
  ofstream os;
  os.open("../../data/sign_data/Ss"); 
  for(int i=0;i<n+1;i++){
  string index1 = "";
  string index2 = "";
  string index3 = "";
  string index4 = "";
  stringstream st;
  st << (i+1);
  st >> index1;
  index2=index1;
  index3=index1;
  index4=index1;
  index1 ="s1-" + index1;
  index2 ="s2-" + index2;
  index3 ="s3-" + index3;
  index4 ="vs-" + index4;
  root[index1] = Json::Value((char*)transfer(s1[i]));
  root[index2] = Json::Value((char*)transfer(s2[i]));
  root[index3] = Json::Value((char*)transfer(s3[i]));
  root[index4] = Json::Value((char*)transfer(vs[i]));
  }
  os << sw.write(root);
  os.close(); 

        

  //清空变量空间
  element_clear(g);
  element_clear(v1);
  for(int i=0;i<m;i++){
  element_clear(v[i]);
  element_clear(u[i]);
  }
  for(int i=0;i<n+1;i++){
  element_clear(D[i]);
  element_clear(d[i]);
  element_clear(s1[i]);
  element_clear(vs[i]);
  element_clear(s2[i]);
  element_clear(s3[i]);
  element_clear(s[i]);
  }
  pairing_clear(pairing);
return 0;
}

