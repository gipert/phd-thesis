settings.outformat = "png";
settings.prc = false;
settings.render = 8;
settings.maxtile = (300,300);
settings.tex = "xelatex";

from detector_db access *;

import three;
currentprojection = orthographic((3,0,-1));

// garamond font
usepackage("ebgaramond", options="lining");
usepackage("unicode-math", options="math-style=ISO, bold-style=ISO");
texpreamble("\setmathfont{Garamond-Math.otf}");
defaultpen(fontsize(15pt));

GD02D.draw(angle1=0, angle2=240, pos=O, hi_pplus=true);
// GD91A.draw(pos=40Z, hi_pplus=true);

label("\textsc{BEGe}", -35Z);
