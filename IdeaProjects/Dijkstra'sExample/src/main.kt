/*
Robert Sato
March 24, 2020
Implementation of Dijkstra's Shortest Path Algorithm
 */

abstract class Random

// initialize a matrix of distances to run Dijkstra's on to infinite
fun initDistances(numVertices : Int) : IntArray {
    //println("initDistances; numVertices = $numVertices {")
    var distances : IntArray = IntArray(numVertices)
    for(vertex in 0 .. numVertices - 1) {
        distances[vertex] = Int.MAX_VALUE
    }
    //println("}")
    return distances
}

// initialize a matrix of previous nodes = -1
fun initPrevious(numVertices : Int) : IntArray {
    //println("initPrevious; numVertices = $numVertices {")
    var previous : IntArray = IntArray(numVertices)
    for(vertex in 0 .. numVertices - 1) {
        previous[vertex] = -1
    }
    //println("}")
    return previous
}

// initialize a matrix of distances to run Dijkstra's on
fun randDistances(height : Int, width : Int, max : Int) : Array<IntArray> {
    println("initDistances; height = $height, width = $width {")
    var matrix: Array<IntArray> = Array(height) { IntArray(width) }
    val rows = matrix.size
    val cols = matrix[0].size
    for(row in 0 .. rows - 1) {
        for(col in 0 .. cols - 1){
            if (col == row) {
                matrix[row][col] = 0
            }
            else {
                matrix[row][col] = (1 .. max).random()
            }
        }
    }
    println("}")
    return matrix
}

fun returnClosest2DVisited(distances : Array<IntArray>, vertex : Int, visited: IntArray) : Int{
    println("returnClosest2D(){")
    val width = distances[0].size
    var closest: Int = -1
    var distance = Int.MAX_VALUE
    for (col in 0 .. width - 1) {
        if (col != vertex && visited[col] != 1) {
            if(distances[vertex][col] < distance) {
                distance = distances[vertex][col]
                closest = col
            }
        }

    }
    if(closest == -1) {
        println("This vertex is a leaf and has no neighbors")
    }
    println("closest vertex to $vertex is $closest with distance $distance")
    println("}")
    return closest
}

//return vertex index of nearest neighbor in 2D graph
fun returnClosest2D(distances : Array<IntArray>, vertex : Int) : Int{
    println("returnClosest2D(){")
    val width = distances[0].size
    var closest: Int = -1
    var distance = Int.MAX_VALUE
    for (col in 0 .. width - 1) {
        if (col != vertex) {
            if(distances[vertex][col] < distance) {
                distance = distances[vertex][col]
                closest = col
            }
        }

    }
    if(closest == -1) {
        println("This vertex is a leaf and has no neighbors")
    }
    println("closest vertex to $vertex is $closest with distance $distance")
    println("}")
    return closest
}

//return vertex index of nearest neighbor in 1D array
fun returnClosest1D(distances : IntArray, vertex : Int) : Int{
    println("returnClosest1D(){")
    val width = distances.size
    var closest: Int = -1
    var distance = Int.MAX_VALUE
    for (col in 0 .. width - 1) {
        if (col != vertex) {
            if (distances[col] < distance) {
                distance = distances[col]
                closest = col
            }
        }
    }
    if(closest == -1) {
        println("This vertex is a leaf and has no neighbors")
    }
    else {
        println("closest vertex to $vertex is $closest with distance $distance")
    }
    println("}")
    return closest
}

fun updateDistances(distances : Array<IntArray>, start : Int, finish : Int, distance : Int) {
    println("updateDistances() {")
    distances[start][finish] = distance
    println("}")
}

fun printDistances(distances : IntArray, vertex : Int){
    println("printDistances(){")
    val width = distances.size
    for (col in 0 .. width - 1) {
        println("distance from $vertex to $col = " + distances[col])
    }
    println("}")
}

fun printMatrix(matrix: Array<IntArray>){
    println("printMatrix() {")
    val rows = matrix.size
    val cols = matrix[0].size
    for(row in 0 .. rows - 1) {
        for(col in 0 .. cols - 1){
            print(matrix[row][col])
            print(", ")
        }
        println()
    }
    println("}")
}

fun printArray(array : IntArray) {
    println("printArray() {")
    val rows = array.size
    for(vertex in 0 .. rows - 1) {
        print(array[vertex])
        print(", ")
    }
    println("}")
}

// returns true if each entry in visited == 1
fun checkVisited(visited: IntArray) : Boolean{
    for (i in (0 .. visited.size - 1)) {
        println("visited[$i] = " + visited[i])
        if (visited[i] !=  1) {
            return false
        }
    }
    return true
}

fun dijkstra(graph: Array<IntArray>, start: Int, current: Int, visited: IntArray, previous: IntArray) : IntArray {
    println("dijkstra(graph[][], $start, $current visited[], previous[]) {")
    printMatrix(graph)
    println("previous array:")
    printArray(previous)
    // 1. initialize all nodes with dist = infinity - starting node = 0
    val vertices = graph.size
    val source = current
    visited[current] = 1 //this node shortest path is computed
    if(checkVisited(visited) == true) {
        print("all nodes visited; return")
        println("}")
        return previous
    }
    //printArray(distances)
    //printArray(previous)
    // 2. calculate all temp distances of neighbor nodes of active node by summing distance w its weights on edge
    var temp : Int
    for (node in (0 .. vertices - 1)) {
        if (node != start && visited[node] != 1) { //not start node or already short path computed node
            // compute distance to all neighbors
            if (graph[start][node] < Int.MAX_VALUE) { // if valid distance
                temp = graph[start][current] + graph[current][node]
                // 3a. "update" if the calculated temp distance is less than the current dist, update the distances
                if (temp < graph[start][node]) {
                    println("$start -> $current -> $node ($temp) is better than $start -> $node (" + graph[start][node] + ")")
                    graph[start][node] = temp
                    // 3b. set the current node as ancestor
                    previous[node] = current
                }
            }
        }
    }
    // 4. min temp distances is new active node
    var closest : Int = returnClosest2DVisited(graph, start, visited)
    dijkstra(graph, start, closest, visited, previous)
    println("}")
    return previous
}


fun printPaths(previous: IntArray, start: Int, end: Int) {
    if(previous[end] != start) {
        print("$end <- ")
        printPaths(previous, start, previous[end])
    }
    else { println("$end <- $start") }
}

fun main(args: Array<String>) {
    //hyper parameters
    val vertices = 8
    val max = 1000

    //test initDistances
    var matrix = randDistances(vertices, vertices, max)

    //test printMatrix
    printMatrix(matrix)

    //test returnClosest
    //returnClosest2D(matrix, 3)

    //test updateDistances
    //updateDistances(matrix, 0, 4, 9)
    //printMatrix(matrix)

    //test dijkstra
    var startNode: Int = 0
    var previous = initPrevious(vertices)
    for (i in ( 0 .. previous.size - 1)) {
        previous[i] = startNode
    }
    var visited : IntArray = initPrevious(vertices) //used to init all bool visited to -1
    printArray(visited)
    println("Calling Dijkstra's on $startNode")

    var initMatrix = matrix

    val bestPaths = dijkstra(matrix, startNode, startNode, visited, previous)
    printMatrix(matrix)
    for (i in ( 0 .. previous.size - 1)) {
        println("Shortest path for $startNode to $i:")
        printPaths(bestPaths, startNode, i)
    }

}

// current problem is it does not compare neighbor + current dist to preexisting distances