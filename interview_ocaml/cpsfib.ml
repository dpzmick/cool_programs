(* http://www.cs.cornell.edu/Courses/cs3110/2011sp/recitations/rec26-cps/cps.htm *)

(* straight up normal recursion *)
let rec sum s = match s with
    | [] -> 0
    | x::xs -> x + sum xs

(* tail *)
let sum_tail s =
    let rec aux s acc =
        match s with
        | [] -> acc
        | x::xs -> aux xs (acc+x)
    in aux s 0

(* continuations
 * Creates a function as accumulator ((((x[0]) + x[1]) + x[2]) + ...)
 *)
let sum_cont s =
    let rec aux s func =
        match s with
        | [] -> func 0
        | x::xs -> aux xs (fun a -> func (x + a))
    in aux s (fun x -> x)

(* now let me try *)
let rec fib = function
    | 0 -> 1
    | 1 -> 1
    | n -> fib (n-1) + fib (n-2)

(* not quite tail, this performs way better, is this getting optimized as a tail
 * call? I think that would make sense
 *)
let fib_almost_tail n =
    let rec loop i acc0 acc1 =
        if i = n then acc0
        else loop (i+1) acc1 (acc0 + acc1)
    in loop 0 1 1

(* WOAH THIS IS REALLY COOL *)
let fib_cps n =
    let rec aux i fun1 fun2 =
        if i = n then fun1 1 1
        else aux (i+1) fun2 (fun a b -> fun1 a b + fun2 a b)
    in aux 0 (fun x y -> 1) (fun x y -> 1)

