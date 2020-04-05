settings.outformat = "png";
settings.prc = false;
settings.render = 8;
settings.maxtile = (256,256);
settings.tex = "xelatex";

from "lib/detector_db" access *;

import three;
currentprojection = orthographic((3,0,-1));

// garamond font
usepackage("ebgaramond", options="lining");
usepackage("unicode-math", options="math-style=ISO, bold-style=ISO");
texpreamble("\setmathfont{Garamond-Math.otf}");
defaultpen(fontsize(15pt));

ANG5.draw(angle1=0, angle2=240, pos=O, hi_pplus=true);
// ANG5.draw(pos=120Z, hi_pplus=true);

label("\textsc{SemiCoax}", -75Z);
