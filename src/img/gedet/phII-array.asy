from "lib/detector_db" access *;

settings.outformat = "png";
settings.prc = false;
settings.render = 4;
settings.maxtile = (256,256);
settings.tex = "xelatex";

// garamond font
usepackage("ebgaramond", options="lining");
usepackage("unicode-math", options="math-style=ISO, bold-style=ISO");
texpreamble("\setmathfont{Garamond-Math.otf}");
defaultpen(fontsize(16pt));

import three;
currentprojection = orthographic((2,10,4));

void draw_label(string lab, real x, real y) {
    path3 l1 = (x,y,90) -- (x,y,170);
    draw(l1);
    label(Label("\texttt{" + lab + "}", position=EndPoint), l1);
}

label(rotate(90)*("\Huge\textsc{Phase II}"), (170,0,-280));

// string 1
draw_label("S1", 110, 0);
GD91A.draw(pos=(110,0,75.09));
GD35B.draw(pos=(110,0,17.05));
GD02B.draw(pos=(110,0,-38.17));
GD00B.draw(pos=(110,0,-96.27));
GD61A.draw(pos=(110,0,-152.72));
GD89B.draw(pos=(110,0,-210.57));
GD02D.draw(pos=(110,0,-275.75), flip=true);
GD91C.draw(pos=(110,0,-309.61));

// string 2
draw_label("S2", 55, 95.26);
ANG5.draw(pos=(55.00,95.26,29.00), 60, 300, hi_pplus=true);
RG1 .draw(pos=(55.00,95.26,-103.00));
ANG3.draw(pos=(55.00,95.26,-240.00));

// string 3
draw_label("S3", -55, 95.26);
GD02A.draw(pos=(-55.00,95.26,67.44), flip=true, 240, 480, hi_pplus=true);
GD32B.draw(pos=(-55.00,95.26,32.58));
GD32A.draw(pos=(-55.00,95.26,-34.40));
GD32C.draw(pos=(-55.00,95.26,-68.42));
GD89C.draw(pos=(-55.00,95.26,-126.13));
GD61C.draw(pos=(-55.00,95.26,-178.78));
GD76B.draw(pos=(-55.00,95.26,-241.72), flip=true);
GD00C.draw(pos=(-55.00,95.26,-276.68));

// string 4
draw_label("S4", -110, 0);
GD35C.draw(pos=(-110.00,0.00,67.84), flip=true);
GD76C.draw(pos=(-110.00,0.00,33.09));
GD89D.draw(pos=(-110.00,0.00,-25.55));
GD00D.draw(pos=(-110.00,0.00,-79.36));
GD79C.draw(pos=(-110.00,0.00,-133.89));
GD35A.draw(pos=(-110.00,0.00,-189.83));
GD91B.draw(pos=(-110.00,0.00,-245.87));
GD61B.draw(pos=(-110.00,0.00,-299.39));

// string 5
draw_label("S5", -55, -95.26);
RG2 .draw(pos=(-55.00,-95.26,-103.00));
ANG4.draw(pos=(-55.00,-95.26,-236.50));
ANG2.draw(pos=(-55.00,-95.26,30.00));

// string 6
draw_label("S6", 55, -95.26);
GD00A.draw(pos=(55.00,-95.26,67.30), flip=true);
GD02C.draw(pos=(55.00,-95.26,32.80));
GD79B.draw(pos=(55.00,-95.26,-22.48));
GD91D.draw(pos=(55.00,-95.26,-74.56));
GD32D.draw(pos=(55.00,-95.26,-127.94));
GD89A.draw(pos=(55.00,-95.26,-183.33));
ANG1 .draw(pos=(55.00,-95.26,-253.00));

// string 7
draw_label("S7", 0, 0);
GTF45 .draw(pos=(0.00,0.00,-189.00));
GTF32 .draw(pos=(0.00,0.00,-89.50));
GTF112.draw(pos=(0.00,0.00,26.50));
