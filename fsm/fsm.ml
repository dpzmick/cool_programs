open Core.Std

type state = | A | B | C
type symbol = | Sa | Sb | Sc

let a_transition = function
    | Sa -> A
    | Sb -> C
    | Sc -> B

let b_transition = function
    | Sa -> A
    | Sb -> B
    | Sc -> C

let c_transition = function
    | Sa -> A
    | Sb -> A
    | Sc -> A

let transition s = match s with
    | A -> a_transition
    | B -> b_transition
    | C -> c_transition

let rec run start sequence =
    match sequence with
    | []    -> failwith "Can't pass empty set"
    | [x]   -> transition start x
    | x::xs -> run (transition start x) xs
