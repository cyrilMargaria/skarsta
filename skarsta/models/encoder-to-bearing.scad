$fn=100;
// printing factor for shrinkage
f = 1.015;

module encoder_shaft() {
    // 6mm with a 
    difference() {
    rotate([0,90,0]) cylinder(h=6, d=6);
    translate([0,-3,1.5]) cube([6,6,3]);
    }
 }
 
 scale([f,f,f])  difference() {   
   rotate([0,90,0]) union() {
        cylinder(h=(6+3), d=10);
        translate([0,0,6+3])cylinder(h=(7+6), d=8);
   }
   encoder_shaft();
 }