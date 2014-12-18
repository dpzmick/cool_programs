open Core.Std

(* slow *)
let rec append_list x y = match x with
    | []    -> y
    | x::xs -> x :: (append_list xs y)

(* faster? *)
let append_tail =
    let rec tail_loop acc x y =
        match x with
        | x::xs -> tail_loop (acc @ [x]) xs y (* I don't want to use this append *)
        | []    ->
                match y with
                | []    -> acc
                | y::ys -> tail_loop (acc @ [y]) [] ys
    in tail_loop []
