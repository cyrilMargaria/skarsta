$fn=100;
// printing factor for shrinkage
f = 1.015;


 
scale([f,f,f]) {
   difference() {   
   rotate([0,90,0]) union() {
        cylinder(h=4, d=10);
        translate([0,0,4]) cylinder(h=3+4, d=12);
   }
   translate([-1,0,0]) rotate([0,90,0]) cylinder(d=7,h=8,$fn=6);
   translate([7,0,0]) rotate([0,90,0]) cylinder(d=8,h=4);
}
}