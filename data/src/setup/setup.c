/*
create by:xiaov
description:setup函数
data:2017-10-25
annotate:此函数是基于对称的，故G1与G2相同
*/
#include "/usr/local/include/pbc/pbc.h"
#include "/usr/local/include/pbc/pbc_test.h"
#define n 10
#define m 10

int main(int argc, char **argv) {
  //定义变量
  pairing_t pairing;
  element_t g;
  element_t y;
  element_t g1,g2;
  element_t t[n];
  element_t z1;
  element_t z[m];
  element_t v1;
  element_t v[m];
  element_t A;
  //初始化变量
  pbc_demo_pairing_init(pairing, argc, argv);//用a.param初始化pairing
  
  element_init_Zr(y, pairing);
  element_init_Zr(z1, pairing);
  element_init_G2(v1, pairing);
  element_init_G2(g, pairing);
  element_init_G2(g1, pairing);
  element_init_G2(g2, pairing);
  for(int i=0;i<=n;i++){
  element_init_G2(t[i], pairing);
  }
  
  for(int i=0;i<=m-1;i++){
  element_init_G2(v[i], pairing);
  }
  for(int i=0;i<=m-1;i++){
  element_init_Zr(z[i], pairing);
  }
  element_init_GT(A, pairing);

  element_printf("Setup function\n");

  element_random(g);
  element_printf("system parameter g = %B\n", g);

  //定义文件变量--读写文件--追加
  FILE *fp;
  fp=fopen("../../setup_data/MK","w+");
  element_random(y);
  element_fprintf(fp,"y:%B\n", y);
  fclose(fp);

  FILE *fp1;
  fp1=fopen("../../setup_data/PP","a+");
  element_pow_zn(g1, g, y);
  element_fprintf(fp1,"g1:%B\n", g1);

  element_random(g2);
  element_fprintf(fp1,"g2:%B\n", g2);

  for(int i=0;i<n;i++){
  element_random(t[i]);
  element_fprintf(fp1,"t[%d+1]:%B\n", i,t[i]);
  }

  element_random(z1);
  element_pow_zn(v1, g,z1);
  element_fprintf(fp1,"v1:%B\n", v1);

  for(int i=0;i<m;i++){
  element_random(z[i]);
  element_pow_zn(v[i],g,z[i]);
  element_fprintf(fp1,"v[%d]:%B\n", i,v[i]);
  }
  
  element_pairing(A,g1,g2);
  element_fprintf(fp1,"A:%B\n",A);
  fclose(fp1);
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
