settings.outformat = "png";
settings.prc = false;
settings.render = 10;
settings.maxtile = (300,300);
settings.tex = "xelatex";

// garamond font
usepackage("ebgaramond", options="lining");
usepackage("unicode-math", options="math-style=ISO, bold-style=ISO");
texpreamble("\setmathfont{Garamond-Math.otf}");
defaultpen(fontsize(7pt));

from "lib/paultolcolors" access *;
from "lib/detector_db" access *;
import three;
currentprojection = orthographic((5,0,1));

// predefined stuff
triple vert_sp = -40Z;
triple hor_sp  = 85Y;
triple label_zpos = -75Z;

surface edep = scale3(1.2)*unitsphere;
surface edeplar = scale3(1.3)*unitsphere;
surface alpha_v = scale3(1.2)*unitsphere;
surface beta_v = scale3(1.2)*unitsphere;

pen edepstyle = tolviborange;
pen edeplarstyle = tolvibcyan;
pen hole = linetype(new real[] {4, 2});
pen electron = linetype(new real[] {0, 2});
pen gamma = tolvibred;
pen alpha = tolbripurple;
pen beta = tolbrigreen;
pen optphoton = tolvibblue;

/*
 * signal
 */

GD91A.draw(angle1=0, angle2=240, pos=O, empty=true, hi_pplus=true);
GD02D.draw(pos=vert_sp, hi_pplus=true);

triple bb = (0, 0.4*GD91A.radius, -0.1*GD91A.height);
draw(bb{-Y} .. {-Z}(0, 0, -GD91A.height/2), hole);
draw(bb{Y} .. {-Z}(0, GD91A.radius*2/3, -GD91A.height/2), electron);
draw(shift(bb)*edep, edepstyle);
label("\tiny$\upbeta\upbeta$", bb, N);

/*
 * granularity
 */

triple top_det = O -100Z;
triple bot_det = O -100Z +vert_sp;

GD00A.draw(angle1=0  , angle2=240, pos=top_det, empty=true, flip=true, hi_pplus=true);
GD02C.draw(angle1=-60, angle2=180, pos=bot_det, empty=true, hi_pplus=true);

triple start_gamma = top_det -0.65*GD00A.radius*Y +GD00A.height*Z/2 +10Z;
triple edep_top = top_det -0.6*GD00A.radius*Y +0.2*GD00A.height*Z;
triple edep_bot = bot_det -0.3*GD02C.radius*Y +0.2*GD00A.height*Z;

label("\tiny$\upgamma$", start_gamma, N);
draw(start_gamma -- edep_top -- edep_bot, gamma);

draw(shift(edep_top)*edep, edepstyle);
draw(edep_top{Y} .. {Z}(top_det+GD00A.height*Z/2), hole);
draw(edep_top{-Y} .. {Z}(top_det+GD00A.height*Z/2-0.8*GD00A.radius*Y), electron);

draw(shift(edep_bot)*edep, edepstyle);
draw(edep_bot{Y-Z} .. {-Z}(bot_det-GD02C.height*Z/2), hole);
draw(edep_bot{-Y+Z} .. {Z}(bot_det+GD02C.height*Z/2-0.5*GD02C.radius*Y), electron);

/*
 * LAr veto
 */

// photon lines
// warning: assumes that the X axis is normal to the drawing
guide3 squiggly(path3 g, real stepsize=3, real slope=50) {
    real len = arclength(g);
    real step = len/round(len/stepsize);
    guide3 squig;
    for (real u = 0; u < len; u += step) {
        real a = arctime(g, u);
        real b = arctime(g, u + step/2);
        triple p = point(g, a);
        triple q = point(g, b);
        triple np = unit(rotate(slope, X) * dir(g,a));
        triple nq = unit(rotate(-slope, X) * dir(g,b));
        squig = squig .. p{np} .. q{nq};
    }
    squig = squig .. point(g, length(g)){unit(rotate(slope, X)*dir(g, length(g)))};
    return squig;
}

triple det_center = O -200Z;

GD91A.draw(angle1=-60, angle2=180, pos=det_center, empty=true, hi_pplus=true);

start_gamma = det_center +GD91A.radius*Y +GD91A.height*Z/2;
edep_top = det_center -0.7GD91A.radius*Y;
triple edep_lar = det_center -50Z -10Y;

label("\tiny$\upgamma$", start_gamma, NE);
draw(start_gamma -- edep_top -- edep_lar, gamma);
draw(edep_top{Y} .. {-Z}(det_center-GD91A.height*Z/2), hole);
draw(edep_top{-Y} .. {-Y}(det_center-GD91A.radius*Y), electron);
draw(shift(edep_top)*edep, edepstyle);
draw(shift(edep_lar)*edeplar, edeplarstyle);

draw(squiggly(edep_lar -- edep_lar + 25Z+25Y), optphoton);
draw(squiggly(edep_lar -- edep_lar + 20Z-20Y), optphoton);
draw(squiggly(edep_lar -- edep_lar + -8Z+20Y), optphoton);
draw(squiggly(edep_lar -- edep_lar + 3Z+40Y), optphoton);
