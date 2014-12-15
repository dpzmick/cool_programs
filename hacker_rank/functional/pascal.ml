open Core.Std

let next_row row =
    List.map2_exn ~f:(+) (0::row) (row @ [0])

let pascal k =
    let rec pascal_loop i acc =
        if i = k then acc
        else
            let new_row = next_row (List.last_exn acc) in
            pascal_loop (i+1) (List.append acc [new_row])
    in pascal_loop 1 [[1]] (* Indexing is wonky *)

let read_int_exn () =
    match In_channel.input_line In_channel.stdin with
    | Some b -> int_of_string (Bytes.to_string b)
    | _      -> failwith "idk"

let () =
    let row_print = List.iter ~f:(printf "%d ") in
    List.iter ~f:(fun r -> row_print r; printf "\n") (pascal (read_int ()))
