open System.IO

let toDelta (s:string) =
    let delta = s[1..] |> System.Int32.Parse
    match s[0] with 'L' -> -delta | _ -> delta

let Part1 (t,p) d =
    let n = p + d%100
    let np = if n >= 100 then n-100 elif n < 0 then n+100 else n
    if np = 0 then (t+1, np) else (t,np)
    
let Part2 (t,p) d =
    let c = abs d/100
    let n = p + d%100
    if n >= 100 then (t+c+1, n-100) elif n < 0 then (t+c+(match p with 0 -> 0 | _ -> 1), n+100) elif n=0 then (t+c+1, 0) else (t+c, n)

[<EntryPoint>]
let main argv =
    let data = File.ReadLines(argv[0]) |> Seq.map toDelta
    Seq.fold Part1 (0,50) data |> fst |> printfn "Part 1: %A"
    Seq.fold Part2 (0,50) data |> fst |> printfn "Part 2: %A"
    0