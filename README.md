# リンクアームの運動学
情報処理演習で出たプログラミング課題

```c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define L1 0.5 //アーム1の長さ
#define L2 0.5 //アーム2の長さ
#define POINT_QTY 4 //中間点の数

typedef struct {
        double x;
        double y;
} pointXY; //XYを格納する構造体

typedef struct {
        double deg1;
        double deg2;
        double rad1;
        double rad2;
} degRad;//角度情報を格納する構造体

double deg2Rad (double deg) {
        return deg * M_PI / 180;
}
double rad2Deg (double rad) {
        return rad * 180 / M_PI;
}

void rad2Deg2 (degRad *p) {
        p->deg1 = p->rad1 * 180 / M_PI;
        p->deg2 = p->rad2 * 180 / M_PI;
}

pointXY calDistFrom2Point(pointXY p1,pointXY p2){
        pointXY xy = {fabs(p1.x-p2.x),fabs(p1.y-p2.y)};
        return xy;
}

int main(void){
        degRad p1;//初期の関節角度
        printf("Please input two values as degree(InputEX:45 90)>\r\n");
        scanf("%lf %lf",&p1.deg1, &p1.deg2);//取得
        printf(" - deg:%lf deg2:%lf\r\n",p1.deg1,p1.deg2);
        pointXY targetXY = {0.0,0.8};//目標座標
        pointXY originXY = {//スタート座標
                L1*cos(deg2Rad(p1.deg1)) + L2*cos(deg2Rad(p1.deg1 + p1.deg2)),
                L1*sin(deg2Rad(p1.deg1)) + L2*sin(deg2Rad(p1.deg1 + p1.deg2))
        };
        printf(" - origin(%lf,%lf)\r\n",originXY.x,originXY.y);//スタート座標の計算結果表示

        //---------------順運動学で先端の中間点の座標を求める---------------
        pointXY distOriginTarget = calDistFrom2Point(targetXY,originXY);//スタート座標と目標座標の距離を計算
        // printf("  - distOriginTarget(%lf,%lf)\r\n",distOriginTarget.x,distOriginTarget.y);
        pointXY midPInterval = {distOriginTarget.x / POINT_QTY,distOriginTarget.y / POINT_QTY};//中間点座標の"間隔"を算出(POINT_QTY個)
        // printf("  - midPInterval(%lf,%lf)\r\n",midPInterval.x,midPInterval.y);
        pointXY midPoint[POINT_QTY];//中間点の座標を格納する変数を宣言
        midPoint[0].x = originXY.x + midPInterval.x;//1つめの中間点の座標計算(X)
        midPoint[0].y = originXY.y + midPInterval.y;//1つめの中間点の座標計算(Y)
        for (int i = 1; i < POINT_QTY; i++) {
                midPoint[i].x = midPoint[i-1].x + midPInterval.x;//2つめ以降の中間点の座標計算(X)
                midPoint[i].y = midPoint[i-1].y + midPInterval.y;//2つめ以降の中間点の座標計算(Y)
        }
        //---------------逆運動学でθ1とθ2を求める---------------
        degRad jointAngles[POINT_QTY];//関節の角度を入れておく変数の宣言
        for (int i = 0; i < POINT_QTY; i++) {//中間点の個数分だけ逆運動学で角度の計算をする
                double D = (pow(midPoint[i].x,2) + pow(midPoint[i].y,2) + L1*L1 - L2*L2)/(2*L1);//D レポート2の定義より
                double E = (pow(midPoint[i].x,2) + pow(midPoint[i].y,2) - L1*L1 + L2*L2)/(2*L1);//E レポート2の定義より
                jointAngles[i].rad1 = atan((-sqrt(pow(midPoint[i].x,2) + pow(midPoint[i].y,2) - pow(D,2)))/D) + atan2(midPoint[i].y,midPoint[i].x);//関節の角度1の計算
                jointAngles[i].rad2 = atan((sqrt(pow(midPoint[i].x,2) + pow(midPoint[i].y,2) - pow(E,2)))/E) - atan((-sqrt(pow(midPoint[i].x,2) + pow(midPoint[i].y,2) - pow(D,2)))/D);//関節の角度2の計算
                rad2Deg2(&jointAngles[i]);//radianからdegreeに変換
        }
        //---------------研鑽結果の表示---------------
        for (int i = 0; i < POINT_QTY; i++) {
                printf(" - midPoint[%d](%lf,%lf)    deg1:%lf    deg2:%lf\r\n",i,midPoint[i].x,midPoint[i].y,jointAngles[i].deg1,jointAngles[i].deg2);
        }
        return 0;
}
```
