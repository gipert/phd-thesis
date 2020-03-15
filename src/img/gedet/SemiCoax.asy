settings.outformat = "png";
settings.prc = false;
settings.render = 8;
settings.maxtile = (256,256);

from detector_db access *;

import three;
currentprojection = orthographic((3,0,-1));

ANG5.draw(angle1=0, angle2=240, pos=O, hi_pplus=true);
ANG5.draw(pos=120Z, hi_pplus=true);
