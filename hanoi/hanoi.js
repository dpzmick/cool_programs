
var hanoi_game = function(num_disks, animator) {
    // TODO do this functionally
    var disks = Array();
    for (var i = num_disks; i > 0; i--) {
        disks.push(i);
    }

    var pegs = Array(3);
    pegs[0] = disks;
    pegs[1] = Array();
    pegs[2] = Array();

    for (var i = num_disks - 1 ; i >= 0; i--) {
        animator.move_disk(i, -1, 0);
    }

    var hanoi_inner = function(pegs, src, dest, intermediate) {
        if (pegs == 0) {
            return;
        }

        hanoi_inner(pegs - 1, src, intermediate, dest);
        move(src, dest);
        hanoi_inner(pegs - 1, intermediate, dest, src);
    }

    var move = function(src, dest) {
        console.log("Moving peg from " + src + " to " + dest);
        disk = pegs[src].pop();
        pegs[dest].push(disk);
        animator.move_disk(disk-1, src, dest);
    }

    this.solve = function() {
        hanoi_inner(disks.length, 0, 2, 1);
    }

    this.finish_animation = function() {
        animator.finish_animation();
    }

}

/*******************
 *  drawing stuff  *
 *******************/

var hanoi_vis = function(html_element, peg_width, disk_width, width, height, num_disks) {
    this.width = width;
    this.height = height;

    var right_left_pad = peg_width + 100;
    var peg_spacing = (width - 3*peg_width - 2*right_left_pad) / 3;

    var pegs = Array();
    var disks = Array();

    var peg_counts = Array();

    var canvas = SVG(html_element).size(this.width, this.height);
    this.done = true;

    var stack = Array();

    var draw_pegs = function() {
        var peg_loc = right_left_pad;
        for (var i = 0; i < 3; i++) {
            pegs[i] = canvas.rect(peg_width, height).attr({ fill: '#f06' });
            pegs[i].move(peg_loc, 0);

            peg_counts[i] = 0;

            peg_loc += peg_spacing;
        }
    }

    // draws the disks off screen, they can later be moved into the appropriate
    // locations
    var draw_disks = function() {
        position_x = -1000;
        position_y = -1000;

        for (var i = 0; i < num_disks; i++) {
            disks[i] = canvas.rect(disk_width*(i+1), 10).attr({ fill: '#000000' });
        }
    }

    var xcoord_for_disk = function(peg, d) {
        return right_left_pad + peg_spacing * peg - d*5;
    }
    var ycoord_for_disk = function(peg) {
        return height - peg_counts[peg] * 10;
    }

    this.move_disk = function(d, src, dest) {
        if ( !this.done ) {
            stack.push( {disk: d, src: src, dest: dest} );
            return;
        }

        this.done = false; // mutex?

        if (src != -1) {
            peg_counts[src] -= 1;
        }

        peg_counts[dest] += 1;
        disks[d].animate(20, '>', 0).move(xcoord_for_disk(dest, d), ycoord_for_disk(dest)).after(
                function() { this.done = true; console.log("done"); });
    }

    this.finish_animation = function() {
        if (stack.length == 0) {
            return;
        }
        var dest = stack[0].dest;
        var src = stack[0].src;
        var d = stack[0].disk;

        stack.shift();

        if (src != -1) {
            peg_counts[src] -= 1;
        }

        peg_counts[dest] += 1;
        var outer_this = this;
        disks[d].animate(20, '>', 0).move(xcoord_for_disk(dest, d), ycoord_for_disk(dest)).after(
                function() { outer_this.finish_animation(); });
    }

    draw_pegs();
    draw_disks();

    return this;
}

disks = 10
D = new hanoi_vis('drawing', 10, 10, 1000, 300, disks);
G = new hanoi_game(disks, D);
G.solve();
G.finish_animation();
