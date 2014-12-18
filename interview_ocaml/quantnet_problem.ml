type 'a tree = Leaf | Node of 'a * 'a tree * 'a tree

(* slow depth *)
let rec depth = function
    | Leaf -> 0
    | Node (value, st1, st2) -> (max (depth st1) (depth st2)) + 1

(* tail recursive depth *)
let tail_depth =
    let aux tree acc = match tree with
        | Leaf -> acc
        | Node (_, st1, st2) ->

let big_tree = Node ("child1", 
    Node ("child1", Leaf, Leaf),
    Node ("child2", Leaf, Leaf))
