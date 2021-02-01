
push_size = 6;
push_case_size = 12;
margin = 0.4;
wall_thickness = 0.8;
$fn = 100;

module insert(x) {
translate([x*(push_case_size+wall_thickness),0,0]) difference() {
union() {    

color([1,0,0]) translate([0,0,3.6]) difference() {
  cube([push_case_size,push_case_size,1.8]); 
  translate([(push_case_size-(push_size+2*margin))/2,(push_case_size-(push_size+2*margin))/2,-1]) cube([push_size+2*margin,push_size+2*margin,6]);
}
translate([-wall_thickness,-wall_thickness,0]) difference() {
  cube([push_case_size+2*wall_thickness,push_case_size+2*wall_thickness,10]);
  translate([wall_thickness,wall_thickness,-1]) cube([push_case_size,push_case_size,push_case_size]);
}
}
translate([-2*wall_thickness,0,-1]) cube([push_case_size+4*wall_thickness,12+2*wall_thickness,4.6]);
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
// hole depth
bore_h = 7*p_ratio;
bore_d = 2.5*p_ratio;
angle = 22;

 
 /*
  Display module, used in other calculation
  +-------------------------------+____
  |O                             O|    4mm
  |       +-----------------+     |____
  |o      |                 |    o|------ CLK 
  |o      |                 |    o|------ DIO
  |o      |                 |    o|------ GND
  |o      |                 |    o|------ VCC (5V)
  |       +-----------------+     |_____
  |O                             O|     4mm
  +-------------------------------+_____
  |       |                 |     |
  | 10mm  |                 | 5mm |
 O : 3.2mm (d) : space between centers: 40mm (Horizontal) 17.5mm (Vertical)
 Space with edge: 1mm (center at 4.2mm from edge)
 o : 2.51mm between pin , 7.51 from edge   
 */
bore_pcb_hole_d = 3.3*p_ratio;
pcb_l = 46*p_ratio;
pcb_w = 23*p_ratio;
pcb_h = 2*p_ratio; 
// distance between PCB edge and hole diameter
bore_pcb_d = 1*p_ratio;

hole_z_check = 0;
module disp() {

    bore_center = bore_pcb_d+bore_pcb_hole_d/2;
    union() {
    cube([pcb_l+2*margin,pcb_w+2*margin,pcb_h]);
      translate([margin+bore_center,margin+bore_center,0]) cylinder(h=bore_h, d=bore_d);
      translate([margin+pcb_l-bore_center,margin+bore_center,0]) cylinder(h=bore_h, d=bore_d);
      translate([margin+bore_center,margin+pcb_w-bore_center,0]) cylinder(h=bore_h, d=bore_d);
      translate([margin+pcb_l-bore_center,margin+pcb_w-bore_center,0]) cylinder(h=bore_h, d=bore_d);
      color([1,0,0]) translate([margin+10*p_ratio,margin+4*p_ratio,pcb_h]) cube([30*p_ratio+2*margin,14*p_ratio+2*margin,case_total_h*p_ratio-0.3-pcb_h]);
    }   
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

  translate([8*p_ratio, 6*p_ratio,-hole_z_check]) cylinder(h=bore_h+hole_z_check, d=bore_d);
  translate([(60+8)*p_ratio, 6*p_ratio,-hole_z_check]) cylinder(h=bore_h+hole_z_check, d=bore_d);
  translate([(150-8)*p_ratio, 6*p_ratio,-hole_z_check]) cylinder(h=bore_h+hole_z_check, d=bore_d);

  translate([8*p_ratio, (22+6)*p_ratio,-hole_z_check]) cylinder(h=bore_h+hole_z_check, d=bore_d);
  translate([(60+8)*p_ratio, (22+6)*p_ratio,-hole_z_check]) cylinder(h=bore_h+hole_z_check, d=bore_d);
  translate([(150-8)*p_ratio, (22+6)*p_ratio,-hole_z_check]) cylinder(h=bore_h+hole_z_check, d=bore_d);
  translate([11*p_ratio, 5*p_ratio,-hole_z_check]) disp();
  // can be used for cabling
  translate([11*p_ratio+pcb_l+2*margin, 9*p_ratio,-hole_z_check]) cube([67.5*p_ratio-(11*p_ratio+pcb_l+2*margin)+wall_thickness+margin,push_case_size,4.6]);
   translate([80*l_ratio, case_w*p_ratio-12,case_total_h*p_ratio-0.8]) {
    linear_extrude(3) text("SIT/STAND", font="Arial Rounded MT Bold", size=9);
  }
}
translate([67.5*p_ratio, 9*p_ratio,0])button_case();
}
