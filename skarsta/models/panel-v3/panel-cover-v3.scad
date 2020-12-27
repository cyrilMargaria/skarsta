
push_size = 6;
margin = 0.4;
wall_thickness = 0.8;
$fn = 100;

module insert(x) {
translate([x*(12+wall_thickness),0,0]) difference() {
union() {    

color([1,0,0]) translate([0,0,3.6]) difference() {
  cube([12,12,1.8]); 
  translate([(12-(push_size+2*margin))/2,(12-(push_size+2*margin))/2,-1]) cube([push_size+2*margin,push_size+2*margin,6]);
}
translate([-wall_thickness,-wall_thickness,0]) difference() {
  cube([12+2*wall_thickness,12+2*wall_thickness,10]);
  translate([wall_thickness,wall_thickness,-1]) cube([12,12,12]);
}
}
translate([-2*wall_thickness,0,-1]) cube([12+4*wall_thickness,12+2*wall_thickness,4.6]);
}
}
module button_case() {
  for (i=[0:5]) {
   insert(i);
   }
}

//button_case();

case_total_h = 10;
case_w = 35;
// print ratio, not for the buttons!
p_ratio = 1.015;
bore_d = 8*p_ratio;
angle = 22;

module disp() {
    // For some official one, need to check with the one I have :-(
    bore_d = 6*p_ratio;
    pcb_l = 66*p_ratio;
    pcb_w = 27.5*p_ratio;
    pcb_h = 2*p_ratio; 
    cube([pcb_l+2*margin,pcb_w+2*margin,pcb_h]);
    translate([margin+3*p_ratio,margin+3.05*p_ratio,0]) cylinder(h=bore_d+pcb_h, d=3*p_ratio);
    translate([margin+pcb_l-3*p_ratio,margin+3.05*p_ratio,0]) cylinder(h=bore_d+pcb_h, d=3*p_ratio);
    translate([margin+3*p_ratio,margin+pcb_w-3.05*p_ratio,0]) cylinder(h=bore_d+pcb_h, d=3*p_ratio);
    translate([margin+pcb_l-3*p_ratio,margin+pcb_w-3.05*p_ratio,0]) cylinder(h=bore_d+pcb_h, d=3*p_ratio);
   color([1,0,0]) translate([margin+6.51*p_ratio,margin+4.04*p_ratio,pcb_h]) cube([50.2*p_ratio+2*margin,19*p_ratio+2*margin,case_total_h*p_ratio-0.3-pcb_h]);

}
union() {
difference() {
  // rounded square
  l_ratio = 1.008;
  // l _ratio checked on the printing sw..
  union() {
     difference() {
       cube([150*l_ratio, case_w*p_ratio,case_total_h*p_ratio]);
       translate([0,0,-2]) cube([9*l_ratio, 9*l_ratio,case_total_h*p_ratio+4]);
       translate([(150-9)*l_ratio,0,-2]) cube([9*l_ratio, 9*l_ratio,case_total_h*p_ratio+4]);
      }
    translate([9*l_ratio,9*l_ratio,0]) cylinder(h=case_total_h*p_ratio,r=9*l_ratio);
    translate([(150-9)*l_ratio,9*l_ratio,0]) cylinder(h=case_total_h*p_ratio,r=9*l_ratio); 
  }
  // chamfer at 16 degree
  translate([-10,case_w*p_ratio-tan(angle)*case_total_h*p_ratio,0]) rotate([-angle,0,0]) cube([160*p_ratio, 10*p_ratio,2*case_total_h*p_ratio]);
  // empty space for buttons 
  translate([67.5*p_ratio, 9*p_ratio,-2]) cube([75.5*p_ratio, 12*p_ratio, case_total_h*p_ratio+4]);
  // holes
  hole_z_check = 1;
  translate([8*p_ratio, 6*p_ratio,-hole_z_check]) cylinder(h=bore_d+hole_z_check, d=3*p_ratio);
  translate([(60+8)*p_ratio, 6*p_ratio,-hole_z_check]) cylinder(h=bore_d+hole_z_check, d=3*p_ratio);
  translate([(150-8)*p_ratio, 6*p_ratio,-hole_z_check]) cylinder(h=bore_d+hole_z_check, d=3*p_ratio);

  translate([8*p_ratio, (22+6)*p_ratio,-hole_z_check]) cylinder(h=bore_d+hole_z_check, d=3*p_ratio);
  translate([(60+8)*p_ratio, (22+6)*p_ratio,-hole_z_check]) cylinder(h=bore_d+hole_z_check, d=3*p_ratio);
  translate([(150-8)*p_ratio, (22+6)*p_ratio,-hole_z_check]) cylinder(h=bore_d+hole_z_check, d=3*p_ratio);
}
translate([67.5*p_ratio, 9*p_ratio,0])button_case();
}
//disp();