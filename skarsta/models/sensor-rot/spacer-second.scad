high_d = 22;
small_d = 6;
pulley_d = 25;
distance = 40.5;
thickness = 5;
thickness_spacer = 18;
mount_h = 25;
// wall thickness added to the pulleys
thickness_y = 3;
capture_d = 16;
belt_h = 2;
belt_w = 11;
belt_pulley_d = 20;
// mounting holes
mounting_distance_axis_x = 6;
mounting_distance_axis_y1 = 5;
mounting_distance_axis_y2 = 9;
mounting_d = 3.2;
mounting_countersink_d = 6.2;
mounting_countersink_h = 3;

module mounting_hole() {}
difference() {
   union() {
       
   hull() {
     cylinder(d=high_d+2*thickness_y,h=thickness+thickness_spacer,$fn=high_d*20);
     translate([distance, 0,0]) cylinder(h=thickness+thickness_spacer, d=small_d+2*thickness_y);
   }
   hull() {
      translate([distance/2,-mount_h/3,0]) cube([13,2*mount_h/3,thickness]);
      translate([distance,-mount_h/2,0]) cube([13,mount_h,thickness]);
   }
   }
   # translate([-(pulley_d+6)/2,0,-1]) cylinder(d=2*(pulley_d+6),h=thickness+thickness_spacer+2 ,$fn=high_d*20);
   # translate([0,0,-1]) cylinder(d=pulley_d+10,h=7+1 ,$fn=high_d*20);
   // make sure the belt has a place to run
   # translate([0,-(belt_pulley_d-2)/2-5,thickness+belt_h]) cube([distance, 5, belt_w]);
   # translate([0,(belt_pulley_d-2)/2,thickness+belt_h]) cube([distance, 5, belt_w]);

   #translate([distance,0,-1]) cylinder(d=small_d,h=thickness+2 ,$fn=high_d*20);
   #translate([distance,0,thickness]) cylinder(d=pulley_d+6,h=thickness_spacer+2 ,$fn=high_d*20);
   #translate([distance,0,thickness-2]) cylinder(d=capture_d+0.4,h=2 ,$fn=high_d*20);

}
