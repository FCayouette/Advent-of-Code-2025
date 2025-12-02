open System.IO
open System.Text.RegularExpressions

let ToRange (s:string) =
    let range = s.Split('-')
    System.Int64.Parse(range[0]), System.Int64.Parse(range[1])

let IsInvalid (exp:Regex) v =
    let s = string v
    if exp.IsMatch(s) then v else 0L

let FindInvalids exp (l,h) =
    Seq.map(fun x -> IsInvalid exp x) [l..h] |> Seq.sum

[<EntryPoint>]
let main argv=
    let ranges = File.ReadAllText(argv[0]).Split(',') |> Array.map ToRange 
    ranges |> Array.map(fun r -> FindInvalids (Regex(@"^(\d+)\1$")) r) |> Array.sum |> printfn("Part 1: %A") 
    ranges |> Array.map(fun r -> FindInvalids (Regex(@"^(\d+)\1+$")) r) |> Array.sum |> printfn("Part 2: %A")
    0