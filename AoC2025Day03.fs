open System.IO

let toIntArray (s:string) =
    s.ToCharArray() |> Array.map(fun c -> (int64)c - (int64)'0') |> Array.indexed

let rec Power p = if p = 1 then 10L else 10L * (Power (p-1))

let rec FindMax enum start depth =
    let searchEnd = (enum |> Array.length) - depth - 1
    let max = enum[start .. searchEnd] |> Array.maxBy snd
    if depth = 0 then snd max
    else (snd max) * (Power depth) + FindMax enum (1+fst max) (depth-1)

[<EntryPoint>]
let main argv = 
    let data = File.ReadLines(argv[0]) |> Seq.map toIntArray
    Seq.map(fun x -> FindMax x 0 1) data |> Seq.sum |> printfn("Part 1: %A")
    Seq.map(fun x -> FindMax x 0 11) data |> Seq.sum |> printfn("Part 2: %A")
    0