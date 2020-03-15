settings.outformat = "png";
settings.prc = false;
settings.render = 8;
settings.maxtile = (300,300);
settings.tex = "xelatex";

// garamond font
usepackage("ebgaramond", options="lining");
usepackage("unicode-math", options="math-style=ISO, bold-style=ISO");
texpreamble("\setmathfont{Garamond-Math.otf}");

from "../../tex/tolcolors.asy" access *;
from detector_db access *;
import three;
currentprojection = orthographic((5,0,1));

// predefined stuff
triple vert_sp = -40Z;
triple hor_sp  = 85Y;

surface edep = scale3(1.2)*unitsphere;
surface edeplar = scale3(1.3)*unitsphere;

pen edepstyle = tolviborange;
pen edeplarstyle = tolvibcyan;
pen hole = linetype(new real[] {4, 4});
pen electron = linetype(new real[] {0, 4});
pen gamma = tolvibred;

/*
 * signal
 */

GD91A.draw(angle1=0, angle2=240, pos=O, empty=true);
GD02D.draw(pos=vert_sp);

triple bb = (0, 0.4*GD91A.radius, -0.1*GD91A.height);
draw(bb{-Y} .. {-Z}(0, 0, -GD91A.height/2), hole);
draw(bb{Y} .. {-Z}(0, GD91A.radius*2/3, -GD91A.height/2), electron);
draw(shift(bb)*edep, edepstyle);
label("\tiny$\upbeta\upbeta$", bb, N);

/*
 * granularity
 */

triple top_det = O+hor_sp;
triple bot_det = O+hor_sp+vert_sp;

GD00A.draw(angle1=0  , angle2=240, pos=top_det, empty=true, flip=true, hi_pplus=true);
GD02C.draw(angle1=-60, angle2=180, pos=bot_det, empty=true);

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
 * PSD
 */

SemiCoax dummy = SemiCoax("GTF32",
    height=70, radius=37,
    groove_depth=2, groove_inner_r=15, groove_outer_r=20,
    borehole_depth=41.5, borehole_radius=6.00,
    is_passivated=true);

triple det_center = O+2*hor_sp+vert_sp/2 -3Z;

dummy.draw(angle1=0, angle2=240, pos=det_center, hi_pplus=true, empty=true);

/*
 * LAr veto
 */

det_center = O+3*hor_sp;

GD91A.draw(angle1=-60, angle2=180, pos=det_center, empty=true);

start_gamma = det_center +GD91A.radius*Y +GD91A.height*Z/2;
edep_top = det_center -0.5GD91A.radius*Y;
triple edep_lar = det_center -50Z -10Y;

label("\tiny$\upgamma$", start_gamma, NE);
draw(start_gamma -- edep_top -- edep_lar, gamma);
draw(shift(edep_top)*edep, edepstyle);
draw(shift(edep_lar)*edeplar, edeplarstyle);
