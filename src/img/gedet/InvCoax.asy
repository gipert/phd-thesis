settings.outformat = "png";
settings.prc = false;
settings.render = 8;
settings.maxtile = (256,256);
settings.tex = "xelatex";

from detector_db access *;

import three;
currentprojection = orthographic((3,0,-1));

// garamond font
usepackage("ebgaramond", options="lining");
usepackage("unicode-math", options="math-style=ISO, bold-style=ISO");
texpreamble("\setmathfont{Garamond-Math.otf}");
defaultpen(fontsize(15pt));

IC48A.draw(angle1=0, angle2=240, pos=O, hi_pplus=true);
// IC48A.draw(pos=90Z, hi_pplus=true);

label("\textsc{InvCoax}", -65Z);
