settings.outformat = "png";
settings.prc = false;
settings.render = 8;
settings.maxtile = (300,300);
settings.tex = "xelatex";

usepackage("fontspec");
texpreamble("\setmainfont[BoldFont={Fira Sans SemiBold}]{Fira Sans Book}");
texpreamble("\setmonofont{Fira Code}[Contextuals=Alternate, Scale=MatchLowercase]");
texpreamble("\usepackage[mathrm=sym]{unicode-math}");
texpreamble("\setmathfont{Fira Math}");
defaultpen(fontsize(10pt));

from "lib/paultolcolors" access *;
from "lib/detector_db" access *;
import three;
currentprojection = orthographic((5,0,1));

triple vert_sp = -40Z;
triple hor_sp  = 85Y;

GD91A.draw(angle1=0, angle2=240, pos=O, empty=true, hi_pplus=true);
GD02D.draw(pos=vert_sp, hi_pplus=true);

label("BEGe", O-70Z);

SemiCoax dummy = SemiCoax("SCOAXDUMMY",
    height=68, radius=34,
    groove_depth=2, groove_inner_r=15, groove_outer_r=20,
    borehole_depth=41.5, borehole_radius=6.00,
    is_passivated=true);

triple det_center = O+hor_sp+vert_sp/2 -2Z;

dummy.draw(angle1=0, angle2=240, pos=det_center, hi_pplus=true, empty=true);

label("semi-coaxial", O+hor_sp-70Z);

InvCoax invdummy = InvCoax("ICDUMMY",
    height=68, radius=30,
    groove_depth=2, groove_inner_r=6, groove_outer_r=10,
    borehole_depth=50, borehole_radius=6.00,
    is_passivated=true);

det_center = O+2*hor_sp+vert_sp/2 -2Z;
invdummy.draw(angle1=-60, angle2=180, pos=det_center, empty=true, hi_pplus=true);

label("inverted-coaxial", O+2*hor_sp-70Z);
