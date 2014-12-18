let rec last = function
    | []    -> None
    | [x]   -> Some x
    | _::xs -> last xs

let rec last_two = function
    | [] | [_]  -> None
    | [x;y]  -> Some (x,y)
    | _::xs  -> last_two xs

let rec at idx lst = match (idx, lst) with
    | (0, x::_)  -> Some x
    | (_, [])    -> None
    | (_, x::xs) -> at (idx - 1) xs

(* theirs *)
let rec their_at k = function
    | [] -> None
    | h :: t -> if k = 1 then Some h else at (k-1) t

let length lst =
    let rec tail_loop acc = function
        | []    -> acc
        | _::xs -> tail_loop (acc + 1) xs
    in tail_loop 0 lst

let rev lst =
    let rec tail_rev acc = function
        | [] -> acc 
        | x::xs -> tail_rev (x::acc) xs
    in tail_rev [] lst

let is_palindrome list = list = List.rev list

(* I do not understand this data structure *)
type 'a node =
    | One of 'a
    | Many of 'a node list

let flatten =
    let rec flatten_node = function
        | One e -> [e]
        | Many es ->
                List.fold es ~init:[] ~f:(fun acc e -> acc @ (flatten_node e))
    in List.fold ~init:[] ~f:(fun acc e -> acc @ (flatten_node e))

let compress lst =
    let rec aux acc last_el = function
        | [] -> acc
        | x::xs -> if Some x <> last_el
            then aux (x::acc) (Some x) xs
            else aux acc last_el xs
    in List.rev (aux [] None lst)

let their_compress = function
    | a :: (b :: t) -> if a = b then compress t else a :: (compress t)
    | smaller -> smaller
