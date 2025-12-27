open System.IO

let ToRange (s:string) =
    let sp = s.Split('-') |> Array.map System.Int64.Parse
    (sp[0], sp[1])

let Merge m (s,e) =
    if (Array.length m  = 0) then [|(s,e)|] else
        if snd (Array.last m) < s then Array.append m [|s,e|] else
            let i = Array.length m - 1
            m[i] <- (fst m[i], max (snd m[i]) e)
            m

[<EntryPoint>]
let main argv =
    let rawRange, rawVal = File.ReadAllLines(argv[0]) |> Array.filter(fun (s:string) -> s.Length > 0) |> Array.partition(fun (s:string) -> s.Contains('-'))
    let ranges = Array.map ToRange rawRange |> Array.sort |> Array.fold Merge [||]
    Array.map System.Int64.Parse rawVal |> Array.filter(fun x -> not(Array.tryFind(fun (s,e) -> s<=x && x<= e) ranges = None)) |> Array.length |> printfn "Part 1: %A" 
    printfn "Part 2: %A" <| Array.fold(fun t (s,e) -> t + (e-s+1L)) 0L ranges
    0
