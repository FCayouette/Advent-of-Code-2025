open System.IO

let ToNumbers (s:string) =
    let n, t = s.ToCharArray() |> Array.fold(fun (n,v) c -> if c=' ' || c='\n' then 
                                                                if v > 0L then (Array.append n [|v|], 0L) 
                                                                else (n,0L)
                                                            else (n, v*10L+(int64)(c-'0'))) ([||], 0L)
    if t > 0 then Array.append n [|t|] else n

let Part1 nums ops = 
    Array.map2(fun n o -> if o = '+' then Array.sum n else Array.fold (*) 1L n) nums ops |> Array.sum

let Part2 raw ops =
    let rawNums = Array.map(fun (s:string) -> s.ToCharArray()) raw |> Array.transpose
    let nums = Array.map(fun a -> Array.fold(fun v c -> if c = ' ' then v else v*10L+(int64)(c-'0')) 0L a) rawNums
    let grouped,work = Array.fold(fun (g,w) i -> if i = 0L then (Array.append g [|w|], [||])
                                                 else (g, Array.append w [|i|])) ([||], [||]) nums
    let groups = Array.append grouped [|work|]
    Part1 groups ops

[<EntryPoint>]
let main argv = 
    let raw = File.ReadAllLines(argv[0])
    let ops = raw[raw.Length-1].ToCharArray()|> Array.filter(fun c -> c ='+' || c='*')
    let p1Nums = Array.map ToNumbers raw[0..raw.Length-2] |> Array.transpose
    Part1 p1Nums ops |> printfn "Part 1: %A"
    Part2 raw[0..raw.Length-2] ops |> printfn "Part 2: %A"
    0

