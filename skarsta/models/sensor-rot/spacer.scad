high_d = 22;
small_d = 6;
distance = 41;
thickness = 3;
thickness_y = 3;
difference() {
   hull() {
     cylinder(d=high_d+2*thickness_y,h=thickness ,$fn=high_d*20);
     translate([distance, 0,0]) cylinder(h=thickness, d=small_d+2*thickness_y);
   }
   translate([0,0,-1]) cylinder(d=high_d,h=thickness+2 ,$fn=high_d*20);
   translate([distance,0,-1]) cylinder(d=small_d,h=thickness+2 ,$fn=high_d*20);
}
