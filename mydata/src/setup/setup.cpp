/*
create by:xiaov
description:setup函数
data:2017-10-25
annotate:此函数是基于对称的，故G1与G2相同
*/
#include "/usr/local/include/pbc/pbc.h"
#include "/usr/local/include/pbc/pbc_test.h"
#include "/usr/include/json/json.h"

#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

const int n = 10;
const int m = 10;

unsigned char* transfer(element_t t){

  int leng = element_length_in_bytes(t);
  unsigned char *p = new unsigned char[leng + 1];
  int writeLength = element_to_bytes_compressed(p,t);
  return p;
}

int main(int argc, char **argv) {
  //定义变量
  pairing_t pairing;
  element_t g;
  element_t y;
  element_t g1,g2;
  element_t t[n+1];
  element_t z1;
  element_t z[m];
  element_t v1;
  element_t v[m];
  element_t A;
  //初始化变量
  pbc_demo_pairing_init(pairing, argc, argv);//用a.param初始化pairing
  
  element_init_Zr(y, pairing);
  element_init_Zr(z1, pairing);
  element_init_G1(v1, pairing);
  element_init_G1(g, pairing);
  element_init_G1(g1, pairing);
  element_init_G1(g2, pairing);
  for(int i=0;i<n+1;i++){
  element_init_G1(t[i], pairing);
  }
  
  for(int i=0;i<m;i++){
  element_init_Zr(z[i], pairing);
  element_init_G1(v[i], pairing);
  }
  element_init_GT(A, pairing);

  element_printf("Setup function\n");

  element_random(g);
  //element_printf("\"g\":\"%B\",\n", g);
  element_random(y);
  //element_printf("\"y\":\"%B\",\n", y);
  element_pow_zn(g1, g, y);
  //element_printf("\"g1\":\"%B\",\n", g1);
  element_random(g2);
  //element_printf("\"g2\":\"%B\",\n", y);
  element_random(z1);
  //element_printf("\"z1\":\"%B\",\n", z1);
  element_pow_zn(v1, g,z1);
  //element_printf("\"v1\":\"%B\",\n", v1);
  for(int i=0;i<n+1;i++){
  element_random(t[i]);
  //element_printf("\"t[%d]\":\"%B\",\n", i,t[i]);
  }
  for(int i=0;i<m;i++){
  element_random(z[i]);
  element_pow_zn(v[i],g,z[i]);
  //element_printf("\"v[%d]\":\"%B\",\n", i,v[i]);
  }
  element_pairing(A,g1,g2);
  //element_printf("\"A\":\"%B\",\n", A);
  
  //定义文件变量--读写文件--追加
  //根节点  
  cout<<"123"; 


  Json::Value root; 
  cout<<"123"; 
  Json::Value root1;  

  //根节点属性  
  Json::StyledWriter sw;  
  Json::StyledWriter sw1;  

  //输出到文件  
  ofstream os;
  ofstream os1;
  os.open("../../data/setup_data/PP"); 
  root["g"] = Json::Value((char*)transfer(g));
  root["g1"] = Json::Value((char*)transfer(g1));
  root["g2"] = Json::Value((char*)transfer(g2)); 
  for(int i=0;i<n+1;i++){
  string index = "";
  stringstream st;
  st << (i+1);
  st >> index;
  index ="t-" + index;
  root[index] = Json::Value((char*)transfer(t[i]));
  }
  root["v1"] = Json::Value((char*)transfer(v1));
  for(int i=0;i<m;i++){
  string index = "";
  stringstream st;
  st << (i+1);
  st >> index;
  index ="v-" + index;
  root[index] = Json::Value((char*)transfer(v[i]));
  }
  cout<<"123";
  //root["A"] = Json::Value((char*)transfer(A));
  os << sw.write(root);
  os.close(); 

/*
  os1.open("../../data/setup_data/MK");
  root["y"] = Json::Value((char*)transfer(y));
  os1 << sw1.write(root);
  os1.close(); 
*/
if(freopen("../../data/setup_data/MK","w+",stdout)==NULL){
    fprintf(stderr, "error2\n");
  }
  printf("{\n");
  element_printf("\"y\":\"%B\"\n",y);
  //end writing data to file MK 
  printf("}\n");

fclose(stdout);

if(freopen("../../data/setup_data/AA","w+",stdout)==NULL){
    fprintf(stderr, "error2\n");
  }
  printf("{\n");
  element_printf("\"A\":\"%B\"\n",A);
  //end writing data to file MK 
  printf("}\n");

fclose(stdout);

  //清空变量空间
  element_clear(g);
  element_clear(y);
  element_clear(g1);
  element_clear(g2);
  element_clear(z1);
  for(int i=0;i<=m-1;i++){
  element_clear(z[i]);
  }
  for(int i=0;i<=n;i++){
  element_clear(t[i]);
  }
  pairing_clear(pairing);
  return 0;
}
