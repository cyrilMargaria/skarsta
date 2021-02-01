module encoder_shaft() {
    // 6mm diameter, 6mm flat, 6mm 
    difference() {
        // make 11mm total len to have 6 + 3 + 2 (alignent plate is 3mm) 
    rotate([0,90,0]) cylinder(h=11, d=6, $fn=6*20);
    translate([0,-3,1.5]) cube([6,6,3]);
    }
 }
 
 
 encoder_shaft();