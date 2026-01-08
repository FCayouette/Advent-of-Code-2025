open System.IO

let Part1 (t, pos) (s:string) =
    let splits = List.fold(fun x i -> if s[i]='^' then x+1 else x) t pos
    let newPos = List.fold(fun nP i -> if s[i] = '.' then i::nP else [i-1; i+1] @ nP) [] pos
    (splits, List.distinct newPos)

let Part2 paths (s:string) =
    let Add (m:Map<int,int64>) x v = 
        if m.ContainsKey(x) then m.Add(x, m[x] + v)
        else m.Add(x, v)

    Map.fold(fun nPaths i v -> if s[i] = '.' then (Add nPaths i v) 
                               else (Add (Add nPaths (i-1) v) (i+1) v)) Map.empty paths
    
[<EntryPoint>]
let main argv =
    let lines = File.ReadAllLines(argv[0])
    let start = lines[0].ToCharArray() |> Array.findIndex(fun c -> c = 'S')
    let splits = Array.filter(fun (s:string) -> s.Contains('^')) lines[1..]
    Array.fold Part1 (0,[start]) splits |> fst |> printfn "Part 1: %A"
    Array.fold Part2 (Map [(start, 1L)]) splits |> Map.values |> Seq.sum |> printfn "Part 2: %A"
    0
