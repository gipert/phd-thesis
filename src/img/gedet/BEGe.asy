settings.outformat = "png";
settings.prc = false;
settings.render = 8;
settings.maxtile = (300,300);

from detector_db access *;

import three;
currentprojection = orthographic((3,0,-1));

GD02D.draw(angle1=0, angle2=240, pos=O, hi_pplus=true);
GD91A.draw(pos=40Z, hi_pplus=true);
