

margin = 0.2;

// Model of a KY040 module for arduino
module ky040() {
    pcb_w = 19 + 2*margin;
    pcb_h = 26.2+ 2*margin;
    encoder_axis_pcb_offset = 11;
    encoder_housing = 12+2*margin;
    union() {
    translate([-encoder_axis_pcb_offset-margin,-pcb_h+encoder_axis_pcb_offset-margin,0]) cube([pcb_w,pcb_h,2]);
    translate([-encoder_housing/2,-encoder_housing/2,2]) cube([encoder_housing,encoder_housing,4.5+margin]);
    translate([0,0,2]) cylinder(d=7+margin,h=11.5+margin, $fn=11.5*20);
    translate([0,0,2+11.5])cylinder(d=6+margin,h=10+margin, $fn=11.5*20);
    translate([-8,5,-1]) cylinder(d=3,h=30, $fn=11.5*20);
    translate([-8,-9,-1]) cylinder(d=3,h=30, $fn=11.5*20);
    }
}

difference() {
translate([-9.5,-13,0]) union() {
   translate([0,0,8]) cube([15,30,7]);
   cube([24,30,8]);

}
rotate([0,0,180]) color([1,0,0]) ky040();
}