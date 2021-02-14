
push_size = 6;
margin = 0.3;

module button(off, txt) {
translate([15*off, 0,0]) 
difference() {
  difference() {
    cube([11.4,11.4,4]);
    translate([(12-(push_size+2*margin))/2,(12-(push_size+2*margin))/2,0]
)   {
        cube([push_size+2*margin,push_size+2*margin,2]);
    }
  }
  translate([2,1,3.5]) {
    linear_extrude(1) text(txt, font="Arial Rounded MT Bold");
  }
}
}

//button(0,"<");
button(1,">");
button(2,"1");
button(3,"2");
button(4,"3");
button(5,"~");
