settings.outformat = "png";
settings.prc = false;
settings.render = 8;
settings.maxtile = (256,256);

from detector_db access *;

import three;
currentprojection = orthographic((3,0,-1));

IC48A.draw(angle1=0, angle2=220, pos=O, hi_pplus=true);
IC48A.draw(pos=90Z, hi_pplus=true);
