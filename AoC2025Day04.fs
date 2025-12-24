open System.IO

let GetRolls (s:string) =
    s.ToCharArray() |> Array.indexed |> Array.filter(fun (_,c) -> c='@') |> Array.map fst

let GenerateNeighbors(x,y) = 
    Array.map(fun (nx,ny) -> (x+nx, y+ny)) [|(-1,-1); (-1,0); (-1,1); (0,-1); (0,1); (1,-1); (1,0); (1,1)|] |> Set

let rec Remove rolls =
    let removeable = Set.filter(fun r -> (Set.intersect (GenerateNeighbors r) rolls |> Set.count) < 4) rolls
    if Set.count removeable = 0 then rolls
    else Remove (Set.difference rolls removeable)

[<EntryPoint>]
let main argv =
    let data = File.ReadAllLines(argv[0]) |> Array.map GetRolls |> Array.indexed |> Array.map(fun (x,ys) -> Array.map(fun y -> (x,y)) ys) |> Array.map Set
    let initialRolls = Array.fold(fun t sets -> Set.union t sets) Set.empty data
    let accessible = Set.filter(fun r -> (Set.intersect (GenerateNeighbors r) initialRolls |> Set.count) < 4) initialRolls 
    Set.count accessible |> printfn "Part 1: %A"
    let unremoveable = Remove (Set.difference initialRolls accessible)
    Set.difference initialRolls unremoveable |> Set.count |> printfn "Part 2: %A"
    0
