#include <stdio.h>
 #include <math.h>
 
 #define L 0.5 //アームの長さ L=L1=L2
 #define XTARGET 0.0 //目標x
 #define YTARGET 0.8 //目標y
 
 #define NP 4 //点の数
 
 double degtorad(double d) {return d * M_PI / 180;} //degreeからradianへ
 double radtodeg(double r) {return r * 180 / M_PI;} //radianからdegreeへ
 double t2tol(double t2) {return 2 * L * cos(t2 * 0.5);} //θ2から原点手先距離
 double ltot2(double l) {return 2 * acos(0.5 * l / L);} //原点手先距離からθ2
 double t12tot(double t1, double t2) {return t1 + t2 * 0.5;} //θ1,θ2から手先θ
 double tt2tot1(double t, double t2) {return t - t2 * 0.5;} //手先θ,θ2からθ1
 
 int main(void) {
       int i;
       double xmp[NP];
       double ymp[NP];
       double t1s, t2s; //アームのθ1θ2初期値・degree→radian
       double xas, yas; //アームの初期座標
       double t2; //θ2処理用
       
       scanf("%lf %lf",&t1s, &t2s);
       t1s = degtorad(t1s); t2s = degtorad(t2s);
       xas = cos(t12tot(t1s, t2s)) * t2tol(t2s);
       yas = sin(t12tot(t1s, t2s)) * t2tol(t2s);
       xmp[NP-1] = XTARGET; ymp[NP-1] = YTARGET;
       for (i = 1; i < NP; i++) {
             xmp[i - 1] = (xmp[NP-1] - xas) * i / NP + xas;
             ymp[i - 1] = (ymp[NP-1] - yas) * i / NP + yas;
       }
       for (i = 0; i < NP; i++) {
             t2 = ltot2(sqrt(xmp[i] * xmp[i] + ymp[i] * ymp[i]));
             printf("θ1 %f, θ2 %f, 手先座標(%f,%f)\n",
             radtodeg(tt2tot1(atan2(ymp[i],xmp[i]),t2)),
             radtodeg(t2), xmp[i], ymp[i]);
       }
       return 0;
 }